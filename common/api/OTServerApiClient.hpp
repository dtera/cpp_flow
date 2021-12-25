#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "../dto/OTServerDTOs.hpp"

/* Begin Api Client code generation */
#include OATPP_CODEGEN_BEGIN(ApiClient)

/**
 * OT Server API client.
 * Use this client to call application APIs.
 */
class OTServerApiClient : public oatpp::web::client::ApiClient {

  API_CLIENT_INIT(OTServerApiClient)

  API_CALL("GET", "/getPublicKey", getPublicKey)

  API_CALL("POST", "/getRandomMsgs", getRandomMsgs, BODY_DTO(Object<OTServerReqDTO>, reqDTO))

  API_CALL("POST", "/getDecryptedYOps", getDecryptedYOps, BODY_DTO(Object<OTServerReqDTO>, reqDTO))

};

/* End Api Client code generation */
#include OATPP_CODEGEN_END(ApiClient)
