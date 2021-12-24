#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class OTClientReqDTO : public oatpp::DTO {
    DTO_INIT(OTClientReqDTO, DTO)

    DTO_FIELD(Fields<String>, uinWithLabelMap);
};

#include OATPP_CODEGEN_END(DTO)
