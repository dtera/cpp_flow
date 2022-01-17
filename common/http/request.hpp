//
// Created by zhaohuiqiang on 2022/1/17.
//
#pragma once

#include <string>
#include <vector>

#include "header.hpp"

namespace http::server3 {

    /// A request received from a client.
    struct request {
        std::string method;
        std::string uri;
        int http_version_major;
        int http_version_minor;
        std::vector<header> headers;
    };

}
