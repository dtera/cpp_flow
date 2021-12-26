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
    OTClientService *oTClientService;
public:
    /**
     * Constructor with object mapper.
     * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
     */
    explicit OTClientController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
            : oatpp::web::server::api::ApiController(objectMapper) {
        oTClientService = new OTClientService(objectMapper);
    }

    ~OTClientController() override {
        free(oTClientService);
    }

    ENDPOINT("GET", "/", root,
             BODY_DTO(Object<OTClientReqDTO>, reqDTO)) {
        String res = "hello world";
        return createDtoResponse(Status::CODE_200, res);
    }

    ENDPOINT("POST", "/getMessages", getMessages,
             BODY_DTO(Fields<UInt8>, uinWithLabelMap)) {
        clock_t start_time = clock();
        auto resDto = ResponseDTO<Vector<String>>::createShared();
        Vector<String> messages;
        oTClientService->setMessages(uinWithLabelMap, messages, false);
        clock_t end_time = clock();
        auto reqTimeMillis = (float) (end_time - start_time) / CLOCKS_PER_SEC * 1000;
        resDto->reqTimeMillis = reqTimeMillis;
        return createDtoResponse(Status::CODE_200, success(resDto, messages));
    }

    ENDPOINT("POST", "/getMessagesOfChosen", getMessagesOfChosen,
             BODY_DTO(Fields<UInt8>, uinWithLabelMap)) {
        clock_t start_time = clock();
        auto resDto = ResponseDTO<Vector<String>>::createShared();
        Vector<String> messages;
        oTClientService->setMessages(uinWithLabelMap, messages, true);
        clock_t end_time = clock();
        auto reqTimeMillis = (float) (end_time - start_time) / CLOCKS_PER_SEC * 1000;
        resDto->reqTimeMillis = reqTimeMillis;
        return createDtoResponse(Status::CODE_200, success(resDto, messages));
    }

};


#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen
