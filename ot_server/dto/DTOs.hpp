#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
#pragma ide diagnostic ignored "OCUnusedTemplateParameterInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
template <class T>
class CommonDTO : public oatpp::DTO {
  DTO_INIT(CommonDTO, DTO)

  DTO_FIELD(Int32, code)= 200;
  DTO_FIELD(String, message)= "success";
  DTO_FIELD(T, data);

};

#include OATPP_CODEGEN_END(DTO)

template <typename T, typename D>
oatpp::Object<T> &of(oatpp::Object<T> &dto, D &data, const oatpp::Int32 &code, const oatpp::String &message) {
    dto->code = code;
    dto->message = message;
    dto->data = data;
    return dto;
}

template <typename T, typename D>
oatpp::Object<T> &success(oatpp::Object<T> &dto, D &data) {
    return of(dto, data, 200, "success");
}

template <typename T, typename D>
oatpp::Object<T> &error(oatpp::Object<T> &dto, D &data) {
    return of(dto, data, 500, "error");
}

