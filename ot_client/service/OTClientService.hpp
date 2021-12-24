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

using namespace oatpp;


class OTClientService {
private:
    string publicKey;
    shared_ptr<OTServerApiClient> otServer;
public:
    OTClientService();

    void setMessages(const Object <OTClientReqDTO> &reqDTO, Vector <String> &messages, const bool &onlyGetChosen);

};

OTClientService::OTClientService() {
    auto reqExecutor = createHttpRequestExecutor(8000);
    /* Create ObjectMapper for serialization of DTOs  */
    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    /* create API client */
    otServer = OTServerApiClient::createShared(reqExecutor, objectMapper);
    publicKey = otServer->getPublicKey()->readBodyToString();
    publicKey = regex_replace(publicKey, regex("\""), "");
    publicKey = regex_replace(publicKey, regex("\\\\n"), "\n");
    OATPP_LOGD("OTClientService", " publicKey \n%s", publicKey.data());
}

void OTClientService::setMessages(const Object <OTClientReqDTO> &reqDTO, Vector <String> &messages,
                                  const bool &onlyGetChosen) {
    messages = {publicKey};
}
