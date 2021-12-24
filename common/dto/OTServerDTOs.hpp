#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class OTServerReqDTO : public oatpp::DTO {
    DTO_INIT(OTServerReqDTO, DTO)

    DTO_FIELD(String, sessionToken);
    DTO_FIELD(Vector<String>, params);
};

#include OATPP_CODEGEN_END(DTO)
