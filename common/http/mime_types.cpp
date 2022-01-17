//
// Created by zhaohuiqiang on 2022/1/17.
//

#include "mime_types.hpp"

namespace http::server3::mime_types {

    struct mapping {
        const char *extension;
        const char *mime_type;
    } mappings[] =
            {
                    {"gif",   "image/gif"},
                    {"htm",   "text/html"},
                    {"html",  "text/html"},
                    {"jpg",   "image/jpeg"},
                    {"png",   "image/png"},
                    {nullptr, nullptr} // Marks end of list.
            };

    std::string extension_to_type(const std::string &extension) {
        for (mapping *m = mappings; m->extension; ++m) {
            if (m->extension == extension) {
                return m->mime_type;
            }
        }

        return "text/plain";
    }

}

