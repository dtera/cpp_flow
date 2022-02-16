//
// Created by zhaohuiqiang on 2021/12/23.
//
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma ide diagnostic ignored "EmptyDeclOrStmt"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include <regex>

#include "common/exceptions/OTError.hpp"
#include "common/http/HttpClient.hpp"
#include "common/ParamsTool.hpp"
#include "common/util/Func.h"
#include "common/util/PbUtils.hpp"
#include "common/util/Utils.h"
#include "mmdcotservicehttpbroker.pb.h"
#include "ot/Message.hpp"
#include "ot/KOutOfNForOTReceiver.hpp"

using namespace otServicePB;

class OTClientService {
private:
    shared_ptr<HttpClient> httpClient;
    string publicKey;
    string reqPath;
    static const unsigned int tokenLen = 32;
public:
    explicit OTClientService();

    void setMessages(const OTClientRequest &req, OTClientResponse &res, const bool &onlyGetChosen = false);

};

OTClientService::OTClientService() {
    this->httpClient = std::make_shared<HttpClient>(ParamsTool::getShort("otServerPort", 8080),
                                                    ParamsTool::get("otServerHost", "localhost"));

    httpClient->get("/getPublicKey");
    otServicePB::Response response;
    //response.ParseFromString(content);
    auto status = httpClient->setPbMessage(response);
    publicKey = response.data();
    cout << "publicKey: \n" << publicKey << endl;

    BaseOTReceiver::set_pub_key(publicKey.data());
}

void OTClientService::setMessages(const OTClientRequest &req, OTClientResponse &res, const bool &onlyGetChosen) {
    clock_t start_time = clock();
    /*std::string requestStr;
    MessageToJsonString(req, &requestStr);
    cout << "otClientReqStr: \n" << requestStr << endl;*/

    map<unsigned int, unsigned int> uinWithLabelMap;
    for (auto &pair: req.uinwithlabelmap()) {
        uinWithLabelMap.insert({pair.first, pair.second});
    }

    //auto sessionToken = gen_random_str(tokenLen);
    auto sessionToken = rand();
    OTServerRequest otServerReq;
    otServerReq.set_function("OT_GetRandomMsgs");
    otServerReq.set_sessiontoken(sessionToken);

    int i = 0;
    vector<int> choices;
    // set choices and dataKeys
    for (auto &pair: uinWithLabelMap) {
        if (pair.second == 1) {
            choices.push_back(i);
        }
        //cout << "[uinWithLabelMap]" << pair.first << ": " << pair.second << endl;
        otServerReq.mutable_keys()->Add(to_string(pair.first));
        i++;
    }
    otServerReq.set_k(choices.size());

    // 1. get random messages from ot sender
    /*std::string requestStr;
    MessageToJsonString(otServerReq, &requestStr);
    cout << "[OTClientService:OT_GetRandomMsgs]requestStr: \n" << requestStr << endl;*/
    httpClient->post(reqPath, otServerReq);
    //cout << "[OTClientService]random messages res: \n" << httpClient->getContent() << endl;
    OTServerResponseWrapper otServerRes;
    auto status = httpClient->setPbMessage(otServerRes);
    auto error = otServerRes.error();
    auto result = otServerRes.result();
    if (error.code() == -8) {
        auto msg = error.message();
        throw OTError(msg);
    }
    vector<string> rms;
    for (auto &value: result.values()) {
        rms.emplace_back(value);
    }
    //println_vector(rms, "[OTClientService]rms");

    // 2. otReceiver encrypt key y with publicKey and random msg
    vector<string> encrypted_y_ops;
    KOutOfNForOTReceiver otReceiver(choices);
    otReceiver.encrypt(rms, encrypted_y_ops, publicKey);
    //println_vector(encrypted_y_ops, "[OTClientService]encrypted_y_ops");

    // 3. otSender decrypt the encrypted key and send the decryptedYOps to otReceiver
    otServerReq.set_function("OT_GetDecryptedYOps");
    for (auto &encrypted_y_op: encrypted_y_ops) {
        otServicePB::Bytes *bytes = otServerReq.add_bytes();
        string2bytes(encrypted_y_op, bytes);
    }
    /*requestStr = "";
    MessageToJsonString(otServerReq, &requestStr);
    cout << "[OTClientService:OT_GetDecryptedYOps]requestStr: \n" << requestStr << endl;*/
    httpClient->post(reqPath, otServerReq);
    OTServerResponseWrapper otServerRes2;
    status = httpClient->setPbMessage(otServerRes2);
    /*std::string responseStr;
    MessageToJsonString(otServerRes2, &responseStr);
    cout << "[OTClientService:OT_GetDecryptedYOps]responseStr: \n" << responseStr << endl;*/
    vector<string> decrypted_y_ops;

    /*auto content = httpClient->getContent();
    cout << "[OTClientService]decryptedYOps res: \n" << content << endl;

    auto s = R"({"error": {"code":0,"message":"ok"},"result": {"function":"OT_GetDecryptedYOps","values":[)";
    boost::replace_all(content, s, "");
    boost::replace_all(content, "]}}", "");
    boost::replace_all(content, "\"", "");
    boost::split(decrypted_y_ops, content, boost::is_any_of(","), boost::token_compress_on);*/
    for (auto & bytes: otServerRes2.result().bytes()) {
        string decrypted_y_op;
        bytes2string(bytes, decrypted_y_op);
        decrypted_y_ops.emplace_back(decrypted_y_op);
    }
    //println_vector(decrypted_y_ops, "[OTClientService]decrypted_y_ops");
    // 4. otReceiver obtain the chosen message
    /*vector<string> decrypted_ms;
    str2str_fp f = nullptr;
    otReceiver.get_messages(decrypted_y_ops, decrypted_ms, f, onlyGetChosen);
    vector_to_oatppVector(decrypted_ms, messages);*/
    vector<FieldsMessage> decrypted_ms;
    str2fields_message_fp f = nullptr; // str2fieldsMessage;
    otReceiver.get_messages(decrypted_y_ops, decrypted_ms, f, onlyGetChosen);
    for (auto decrypted_m: decrypted_ms) {
        //cout << "[OTClientService:get_messages]decrypted_m: " << decrypted_m << endl;
        for (auto fs: *decrypted_m.getFields()) {
            res.mutable_resmap()->insert({fs.first, fs.second});
        }
    }
    //for_each(decrypted_ms.begin(), decrypted_ms.end(), println<string>);

    clock_t end_time = clock();
    auto reqTimeMillis = (float) (end_time - start_time) / CLOCKS_PER_SEC * 1000;
    res.set_reqtimemillis(reqTimeMillis);
}
