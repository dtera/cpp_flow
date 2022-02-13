//
// Created by zhaohuiqiang on 2021/12/23.
//
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EmptyDeclOrStmt"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include <regex>

#include "common/exceptions/OTError.hpp"
#include "common/http/HttpClient.hpp"
#include "common/util/Func.h"
#include "common/ParamsTool.hpp"
#include "mmdcotservicehttpbroker.pb.h"
#include "ot/Message.hpp"
#include "ot/KOutOfNForOTReceiver.hpp"

using namespace otServicePB;

class OTClientService {
private:
    shared_ptr<HttpClient> httpClient;
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
    auto publicKey = response.data();
    cout << "publicKey: \n" << publicKey << endl;

    BaseOTReceiver::set_pub_key(publicKey.data());
}

void OTClientService::setMessages(const OTClientRequest &req, OTClientResponse &res, const bool &onlyGetChosen) {
    map<unsigned int, unsigned int> uinWithLabelMap;
    for (auto &pair: req.uinwithlabelmap()) {
        uinWithLabelMap.insert({pair.first, pair.second});
    }

    auto sessionToken = gen_random_str(tokenLen);
    OTServerRequest otServerReq;
    otServerReq.set_sessiontoken(sessionToken);
    vector<int> choices;

    int i = 0;
    // set choices and dataKeys
    for (auto &pair: uinWithLabelMap) {
        if (pair.second == 1) {
            choices.push_back(i);
        }
        //cout << "[uinWithLabelMap]" << pair.first << ": " << pair.second << endl;
        otServerReq.mutable_params()->Add(to_string(pair.first));
        i++;
    }
    otServerReq.set_k(choices.size());

    OTServerResponseWrapper otServerRes;
    // 1. get random messages from ot sender
    std::string otServerReqStr;
    MessageToJsonString(otServerReq, &otServerReqStr);
    cout << "otServerReqStr: \n" << otServerReqStr << endl;
    httpClient->post("/getRandomMsgs", otServerReq);
    cout << "[POST]random messages: \n" << httpClient->getContent() << endl;
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
    //println_vector(rms, "rms");
    otServerReq.mutable_params()->Clear();
    // 2. otReceiver encrypt key y with publicKey and random msg
    vector<string> encrypted_y_ops;
    KOutOfNForOTReceiver otReceiver(choices);
    otReceiver.encrypt(rms, encrypted_y_ops);
    // 3. otSender decrypt the encrypted key and send the decryptedYXorMs to otReceiver
    OTServerRequest otServerReq2;
    otServerReq2.set_function("OT_GetDecryptedYOps");
    otServerReq2.set_sessiontoken(sessionToken);
    otServerReq2.set_k(choices.size());
    for (auto &encrypted_y_op: encrypted_y_ops) {
        otServerReq2.mutable_params()->Add(encrypted_y_op.data());
    }
    OTServerResponseWrapper otServerRes2;
    otServerReqStr = "";
    MessageToJsonString(otServerReq2, &otServerReqStr);
    cout << "otServerReqStr: \n" << otServerReqStr << endl;
    httpClient->post(reqPath, otServerReq2);
    cout << "[POST]decryptedYOps: \n" << httpClient->getContent() << endl;

    vector<string> decrypted_y_ops;
    for (auto &value: otServerRes2.result().values()) {
        decrypted_y_ops.emplace_back(value);
    }
    //println_vector(decrypted_y_ops, "decrypted_y_ops");
    // 4. otReceiver obtain the chosen message
    /*vector<string> decrypted_ms;
    str2str_fp f = nullptr;
    otReceiver.get_messages(decrypted_y_ops, decrypted_ms, f, onlyGetChosen);
    vector_to_oatppVector(decrypted_ms, messages);*/
    vector<FieldsMessage> decrypted_ms;
    str2fields_message_fp f = nullptr; // str2fieldsMessage;
    otReceiver.get_messages(decrypted_y_ops, decrypted_ms, f, onlyGetChosen);
    for (auto decrypted_m: decrypted_ms) {
        for (auto fs: *decrypted_m.getFields()) {
            vector<string> kv;
            boost::split(kv, fs, boost::is_any_of(":"),
                         boost::token_compress_on);
            res.mutable_resmap()->insert({kv[0], kv[1]});
        }
    }
    //for_each(decrypted_ms.begin(), decrypted_ms.end(), println<string>);

    std::string otClientResStr;
    MessageToJsonString(res, &otClientResStr);
    cout << "otClientResStr: \n" << otClientResStr << endl;
}
