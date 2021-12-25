//
// Created by zhaohuiqiang on 2021/12/24.
//
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"
#pragma ide diagnostic ignored "EmptyDeclOrStmt"
#pragma ide diagnostic ignored "UnusedLocalVariable"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <oatpp/network/ConnectionPool.hpp>
#include <oatpp/network/tcp/client/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>

using namespace oatpp;
using namespace oatpp::network;
using namespace oatpp::web;

std::shared_ptr<oatpp::web::client::RequestExecutor>
createHttpRequestExecutor(const unsigned short &port = 80, const std::string &host = "localhost",
                          const unsigned short &maxConn = 10, const unsigned short &maxIdle = 5) {
    OATPP_LOGD("OTClientService", " Using Oat++ native HttpRequestExecutor.");
    /* create connection provider */
    auto connectionProvider = tcp::client::ConnectionProvider::createShared(
            {host, port, oatpp::network::Address::IP_4});
    /* create connection pool */
    /*auto connectionPool = std::make_shared<ClientConnectionPool>(
            connectionProvider,
            maxConn,
            std::chrono::seconds(maxIdle)
    );*/
    /* create retry policy */
    auto retryPolicy = std::make_shared<client::SimpleRetryPolicy>(3, std::chrono::seconds(1));
    /* create request executor */
    auto requestExecutor = client::HttpRequestExecutor::createShared(
            connectionProvider, retryPolicy);
    return requestExecutor;
}

struct pair_less {
    bool operator()(const std::pair<String, UInt8> &p1, const std::pair<String, UInt8> &p2) {
        return *(p1.first.get()) < *(p2.first.get());
    }
};

template<typename T1, typename T2>
void oatppVector_to_vector(Vector <T1> &v1, vector<T2> &v2) {
    for (auto &t: v1) {
        v2.push_back(t);
    }
}