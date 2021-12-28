//
// Created by zhaohuiqiang on 2021/12/23.
//
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EmptyDeclOrStmt"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include <regex>
#include <oatpp/core/Types.hpp>


#include "../../ot/Message.hpp"
#include "../../ot/KOutOfNForOTReceiver.hpp"
#include "../../common/api/OTServerApiClient.hpp"
#include "../../common/dto/OTClientDTOs.hpp"
#include "../../common/util/OatppUtils.hpp"
#include "../../common/util/Func.h"

using namespace oatpp;


class OTClientService {
private:
    shared_ptr<server::api::ApiController::ObjectMapper> objectMapper;
    shared_ptr<OTServerApiClient> otServer;
    String publicKey;
    static const unsigned int tokenLen = 32;
public:
    explicit OTClientService(shared_ptr<server::api::ApiController::ObjectMapper> &objectMapper);

    //void setMessages(const Fields<UInt8> &uinWithLabelMap, Vector<String> &messages, const bool &onlyGetChosen);

    void setMessages(const Fields<UInt8> &uinWithLabelMap, Vector<Fields<String>> &messages, const bool &onlyGetChosen);

};

OTClientService::OTClientService(shared_ptr<server::api::ApiController::ObjectMapper> &objectMapper) {
    auto reqExecutor = createHttpRequestExecutor(8000);
    // Create ObjectMapper for serialization of DTOs
    //this->objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    this->objectMapper = objectMapper;
    // create API client
    this->otServer = OTServerApiClient::createShared(reqExecutor, objectMapper);
    // get public key and cache it
    /*this->publicKey = otServer->getPublicKey()->readBodyToString();
    this->publicKey = regex_replace(publicKey, regex("\""), "");
    this->publicKey = regex_replace(publicKey, regex("\\\\n"), "\n");
    this->publicKey = regex_replace(publicKey, regex("\\\\/"), "/");*/
    this->publicKey = otServer->getPublicKey()->readBodyToDto<Object<ResponseDTO<String>>>(objectMapper)->data;
    // set public key for ot receiver
    BaseOTReceiver::set_pub_key(publicKey.get()->data());
    OATPP_LOGD("OTClientService", " publicKey \n%s", publicKey.get()->data());
}

void OTClientService::setMessages(const Fields<UInt8> &uinWithLabelMap, Vector<Fields<String>> &messages,
                                  const bool &onlyGetChosen) {
    messages = {};
    uinWithLabelMap->sort(pair_less());

    auto sessionToken = gen_random_str(tokenLen);
    auto otServerReqDTO = OTServerReqDTO<String>::createShared();
    otServerReqDTO->sessionToken = sessionToken;
    otServerReqDTO->params = {};
    vector<int> choices;

    int i = 0;
    // set choices and dataKeys
    for (auto &pair: *uinWithLabelMap) {
        if (pair.second.getValue(-1) == 1) {
            choices.push_back(i);
        }
        otServerReqDTO->params->push_back(pair.first);
        i++;
    }

    // 1. get random messages from ot sender
    auto randomMsgs = otServer->getRandomMsgs(otServerReqDTO)->
            readBodyToDto<Vector<String>>(objectMapper);
    vector<string> rms;
    oatppVector_to_vector(randomMsgs, rms);
    // 2. otReceiver encrypt key y with publicKey and random msg
    vector<string> encrypted_y_ops;
    KOutOfNForOTReceiver otReceiver(choices);
    otReceiver.encrypt(rms, encrypted_y_ops);
    //vector_to_oatppVector(encrypted_y_ops, otServerReqDTO->params);
    auto otServerReqDTO2 = OTServerReqDTO<Vector<Int8>>::createShared();
    otServerReqDTO2->sessionToken = sessionToken;
    vector_str2int_Vector_Vector(encrypted_y_ops, otServerReqDTO2->params);
    // 3. otSender decrypt the encrypted key and send the decryptedYXorMs to otReceiver
    auto decryptedYOps = otServer->getDecryptedYOps(otServerReqDTO2)->
            readBodyToDto<Vector<Vector<Int8>>>(objectMapper);
    vector<string> decrypted_y_ops;
    //oatppVector_to_vector(decryptedYOps, decrypted_y_ops);
    int_Vector_Vector2vector_str(decryptedYOps, decrypted_y_ops);
    // 4. otReceiver obtain the chosen message
    /*vector<string> decrypted_ms;
    str2str_fp f = nullptr;
    otReceiver.get_messages(decrypted_y_ops, decrypted_ms, f, onlyGetChosen);
    vector_to_oatppVector(decrypted_ms, messages);*/
    vector<FieldsMessage> decrypted_ms;
    str2fields_message_fp f = nullptr; // str2fieldsMessage;
    otReceiver.get_messages(decrypted_y_ops, decrypted_ms, f, onlyGetChosen);
    for (auto decrypted_m: decrypted_ms) {
        messages->push_back(decrypted_m.getFields());
    }
    //for_each(decrypted_ms.begin(), decrypted_ms.end(), println<string>);

}
