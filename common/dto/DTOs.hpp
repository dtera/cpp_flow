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
class ResponseDTO : public oatpp::DTO {
    DTO_INIT(ResponseDTO, DTO)

    DTO_FIELD(UInt16, code)= 200;
    DTO_FIELD(String, message)= "success";
    DTO_FIELD(T, data);
};

template <class V>
class KvDTO : public oatpp::DTO {
    DTO_INIT(KvDTO, DTO)

    DTO_FIELD(String, key);
    DTO_FIELD(V, value);
};

#include OATPP_CODEGEN_END(DTO)



// ========================Begin common function to construct ResponseDTO========================
template <typename D>
oatpp::Object<ResponseDTO<D>> &of(oatpp::Object<ResponseDTO<D>> &dto, D &data,
                                  const oatpp::Int32 &code, const oatpp::String &message) {
    dto->code = code;
    dto->message = message;
    dto->data = data;
    return dto;
}

template <typename D>
oatpp::Object<ResponseDTO<D>> &success(oatpp::Object<ResponseDTO<D>> &dto, D &data) {
    return of(dto, data, 200, "success");
}

template <typename D>
oatpp::Object<ResponseDTO<D>> &error(oatpp::Object<ResponseDTO<D>> &dto, D &data) {
    return of(dto, data, 500, "Internal Server Error");
}
// ========================End common function to construct ResponseDTO==========================

// ===========================Begin common function to construct KvDTO===========================
template <typename V>
oatpp::Object<KvDTO<V>> &ofKV(oatpp::Object<KvDTO<V>> &dto, const oatpp::String& key, V &value) {
    dto->key = key;
    dto->value = value;
    return dto;
}
// ===========================End common function to construct KvDTO=============================