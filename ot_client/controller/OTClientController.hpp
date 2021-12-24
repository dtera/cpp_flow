#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EmptyDeclOrStmt"
#pragma ide diagnostic ignored "NotImplementedFunctions"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"


#include "../../common/dto/DTOs.hpp"

#include "../service/OTClientService.hpp"


#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * OT Client Api Controller.
 */
class OTClientController : public oatpp::web::server::api::ApiController {
private:
    OTClientService oTClientService;
public:
    /**
     * Constructor with object mapper.
     * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
     */
    explicit OTClientController (OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    :oatpp::web::server::api::ApiController(objectMapper) {
    }

    ENDPOINT("GET", "/", root,
             BODY_DTO(Object<OTClientReqDTO>, reqDTO)) {
        String res = "hello world";
        return createDtoResponse(Status::CODE_200, res);
    }

    ENDPOINT("POST", "/getMessages", getMessages,
             BODY_DTO(Object<OTClientReqDTO>, reqDTO)) {
        auto resDto = ResponseDTO<Vector<String>>::createShared();
        Vector<String> messages;
        oTClientService.setMessages(reqDTO, messages, true);
        return createDtoResponse(Status::CODE_200, success(resDto, messages));
    }

    ENDPOINT("POST", "/getMessagesOfChosen", getMessagesOfChosen,
             BODY_DTO(Object<OTClientReqDTO>, reqDTO)) {
        auto resDto = ResponseDTO<Vector<String>>::createShared();
        Vector<String> messages;
        oTClientService.setMessages(reqDTO, messages, false);
        return createDtoResponse(Status::CODE_200, success(resDto, messages));
    }

};


#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen
