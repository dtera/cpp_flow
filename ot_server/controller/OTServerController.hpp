#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <vector>
#include "../../ot/KOutOfNForOTSender.hpp"
#include "../../ot/Message.hpp"
#include "../dto/DTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * OT Server Api Controller.
 */

class OTServerController : public oatpp::web::server::api::ApiController {
private:

public:
    /**
     * Constructor with object mapper.
     * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
     */
    explicit OTServerController (OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    :oatpp::web::server::api::ApiController(objectMapper) {}

    ENDPOINT("GET", "/", root) {
        auto dto = CommonDTO<String>::createShared();

        string s1 = "abc", s2 = "def", s3 = "ghi";
        vector<StrMessage> ms = {StrMessage(s1), StrMessage(s2), StrMessage(s3)};
        KOutOfNForOTSender<StrMessage> otSender(ms);
        string pk = BaseOTSender<StrMessage>::get_pub_key();

        return createDtoResponse(Status::CODE_200, success(dto, pk));
    }

    // TODO Insert Your endpoints here !!!

};


#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen
