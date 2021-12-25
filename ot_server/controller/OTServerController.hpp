#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EmptyDeclOrStmt"
#pragma ide diagnostic ignored "NotImplementedFunctions"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <vector>
#include <map>

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include "../../common/dto/DTOs.hpp"

#include "../service/OTServerService.hpp"


#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * OT Server Api Controller.
 */

class OTServerController : public oatpp::web::server::api::ApiController {
private:
    OTServerService otServerService;
public:
    /**
     * Constructor with object mapper.
     * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
     */
    explicit OTServerController (OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    :oatpp::web::server::api::ApiController(objectMapper) {
            BaseOTSender<string>::gen_keypair();
    }

    /*ENDPOINT("GET", "/", root) {
        auto resDto = ResponseDTO<oatpp::Object<KvDTO<Float32>>>::createShared();
        string ms = "hello world";
        auto data = StrMessage(ms);
        auto kvDto = KvDTO<Float32>::createShared();
        Float32 v = 20.3;
        kvDto = ofKV(kvDto, "1234456", v);
        return createDtoResponse(Status::CODE_200, success(resDto, kvDto));
    }*/

    ENDPOINT("GET", "/getPublicKey", getPublicKey) {
        //auto resDto = ResponseDTO<String>::createShared();
        String pk;
        otServerService.setPublicKey(pk);
        return createDtoResponse(Status::CODE_200, pk);
        //return createDtoResponse(Status::CODE_200, success(resDto, pk));
    }

    ENDPOINT("POST", "/getRandomMsgs", getRandomMsgs,
             BODY_DTO(Object<OTServerReqDTO>, reqDTO)) {
        auto resDto = ResponseDTO<Vector<String>>::createShared();
        Vector<String> rms;
        otServerService.setRandomMsgs(reqDTO, rms);
        return createDtoResponse(Status::CODE_200, success(resDto, rms));
    }

    ENDPOINT("POST", "/getDecryptedYOps", getDecryptedYOps,
             BODY_DTO(Object<OTServerReqDTO>, reqDTO)) {
        auto resDto = ResponseDTO<Vector < String>>::createShared();
        try {
            Vector <String> decryptedYOps;
            otServerService.setDecryptedYOps(reqDTO, decryptedYOps);
            return createDtoResponse(Status::CODE_200, success(resDto, decryptedYOps));
        } catch (const logic_error &e) {
            String err = e.what();
            OATPP_LOGE("OTServerService", " logic_error ==> %s", err->data());
            auto errDto = ResponseDTO<String>::createShared();
            return createDtoResponse(Status::CODE_500, error(errDto, err));
        }
    }

};


#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen
