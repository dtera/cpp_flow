//
// Created by zhaohuiqiang on 2022/1/17.
//
#pragma once

#include <string>

namespace http::server3::mime_types {

    /// Convert a file extension into a MIME type.
    std::string extension_to_type(const std::string &extension);

}

