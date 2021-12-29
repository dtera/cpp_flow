#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

template<class T>
class OTServerReqDTO : public oatpp::DTO {
    DTO_INIT(OTServerReqDTO, DTO)

    DTO_FIELD(String, sessionToken);
    DTO_FIELD(UInt16, k);
    DTO_FIELD(Vector<T>, params);
};

#include OATPP_CODEGEN_END(DTO)
