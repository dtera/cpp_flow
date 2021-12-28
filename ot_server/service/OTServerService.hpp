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
#include "../../common/util/OatppUtils.hpp"
#include "../../common/util/Func.h"
#include "../../common/RedisCli.hpp"
#include "../../ot/KOutOfNForOTSender.hpp"
#include "../../ot/Message.hpp"

using namespace oatpp;

class OTServerService {
private:
    String publicKey;
    //map<string, KOutOfNForOTSender<StrMessage>> otSenderMap;
    map<string, KOutOfNForOTSender<FieldsMessage>> otSenderMap;
    RedisCli redisCli;
public:
    OTServerService();

    String &getPublicKey();

    void setRandomMsgs(const Object<OTServerReqDTO<String>> &reqDTO, Vector<String> &rms);

    void setDecryptedYOps(const Object<OTServerReqDTO<Vector<Int8>>> &reqDTO, Vector<Vector<Int8>> &decryptedYOps);
};

OTServerService::OTServerService() {
    BaseOTSender<StrMessage>::gen_keypair();
    publicKey = BaseOTSender<StrMessage>::get_pub_key();
    OATPP_LOGD("OTServerService", " publicKey \n%s", publicKey->data());
}

String &OTServerService::getPublicKey() {
    return publicKey;
}

void OTServerService::setRandomMsgs(const Object<OTServerReqDTO<String>> &reqDTO, Vector<String> &rms) {
    /*string s1 = "abc", s2 = "def", s3 = "ghi", s4 = "jkl", s5 = "mno";
    vector<StrMessage> ms = {StrMessage(s1), StrMessage(s2), StrMessage(s3),
                             StrMessage(s4), StrMessage(s5)};
    KOutOfNForOTSender<StrMessage> otSender(ms);*/
    /*vector<StrMessage> ms;
    str2str_message_fp f = str2strMessage;
    redisCli.mGet(reqDTO->params, ms, f);
    KOutOfNForOTSender<StrMessage> otSender(ms);*/
    vector<FieldsMessage> ms;
    redisCli.hGetAll(reqDTO->params, ms);
    KOutOfNForOTSender<FieldsMessage> otSender(ms);
    otSenderMap.insert(make_pair(reqDTO->sessionToken, otSender));
    vector_to_oatppVector(otSender.get_rs(), rms);
}

void OTServerService::setDecryptedYOps(const Object<OTServerReqDTO<Vector<Int8>>> &reqDTO,
                                       Vector<Vector<Int8>> &decryptedYOps) {
    //KOutOfNForOTSender<StrMessage> otSender = otSenderMap.at(reqDTO->sessionToken);
    KOutOfNForOTSender<FieldsMessage> otSender = otSenderMap.at(reqDTO->sessionToken);
    vector<string> encrypted_y_ops, decrypted_y_ops;

    //oatppVector_to_vector(reqDTO->params, encrypted_y_ops);
    int_Vector_Vector2vector_str(reqDTO->params, encrypted_y_ops);

    otSender.decrypt(encrypted_y_ops, decrypted_y_ops);
    //vector_to_oatppVector(decrypted_y_ops, decryptedYOps);
    vector_str2int_Vector_Vector(decrypted_y_ops, decryptedYOps);

    otSenderMap.erase(reqDTO->sessionToken);
    cout << "debug" << endl;
}
