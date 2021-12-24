//
// Created by zhaohuiqiang on 2021/12/23.
//
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EmptyDeclOrStmt"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include <map>
#include <oatpp/core/Types.hpp>

#include "../../common/dto/OTServerDTOs.hpp"
#include "../../ot/KOutOfNForOTSender.hpp"
#include "../../ot/Message.hpp"

using namespace oatpp;

class OTServerService {
private:
    map<string, KOutOfNForOTSender<StrMessage>> otSenderMap;
public:
    OTServerService();

    void setPublicKey(String &publicKey);

    void setRandomMsgs(const Object <OTServerReqDTO> &reqDTO, Vector <String> &rms);

    void setDecryptedYOps(const Object <OTServerReqDTO> &reqDTO, Vector <String> &decryptedYOps);
};

OTServerService::OTServerService() {
    BaseOTSender<string>::gen_keypair();
}

void OTServerService::setPublicKey(String &publicKey) {
    publicKey = BaseOTSender<string>::get_pub_key();
    OATPP_LOGD("OTServerService", " publicKey \n%s", publicKey->data());
}

void OTServerService::setRandomMsgs(const Object <OTServerReqDTO> &reqDTO, Vector <String> &rms) {
    string s1 = "abc", s2 = "def", s3 = "ghi";
    vector<StrMessage> ms = {StrMessage(s1), StrMessage(s2), StrMessage(s3)};
    KOutOfNForOTSender<StrMessage> otSender(ms);
    otSenderMap.insert(make_pair(reqDTO->sessionToken, otSender));
    rms = {};
    for (auto &rm: otSender.get_rs()) {
        rms->push_back(rm);
    }
}

void OTServerService::setDecryptedYOps(const Object <OTServerReqDTO> &reqDTO, Vector <String> &decryptedYOps) {
    KOutOfNForOTSender<StrMessage> otSender = otSenderMap.at(reqDTO->sessionToken);
    vector<string> encrypted_y_ops, decrypted_y_ops;
    for (auto &encrypted_y_op: *reqDTO->params) {
        encrypted_y_ops.push_back(encrypted_y_op);
    }

    otSender.decrypt(encrypted_y_ops, decrypted_y_ops);
    decryptedYOps = {};
    for (auto &decrypted_y_op: decrypted_y_ops) {
        decryptedYOps->push_back(decrypted_y_op);
    }
    otSenderMap.erase(reqDTO->sessionToken);
}
