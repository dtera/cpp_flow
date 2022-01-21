//
// Created by zhaohuiqiang on 2022/1/14.
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "common/http/http_client.hpp"

using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {
    try {
        boost::asio::io_context io_context;
        http_client c(io_context, "localhost", "/getPublicKey");
        io_context.run();
    }
    catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}
