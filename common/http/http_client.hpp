//
// Created by zhaohuiqiang on 2022/1/21.
//
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-avoid-bind"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/text_format.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "common/util/Utils.h"

using boost::asio::ip::tcp;
using namespace google::protobuf::util;
using namespace std;

class http_client {
public:
    explicit http_client(const int port = 80, std::string host = "localhost", std::string http_version = "1.1")
            : query(host, boost::lexical_cast<std::string>(port),
                    boost::asio::ip::resolver_query_base::numeric_service),
              resolver_(io_context), socket_(io_context),
              port(port), host(std::move(host)), http_ver(std::move(http_version)) {}

    void get(const std::string &path) {
        // Form the request. We specify the "Connection: close" header so that the
        // server will close the socket after transmitting the response. This will
        // allow us to treat all data up until the EOF as the content.
        std::ostream request_stream(&request_);
        request_stream << "GET " << path << " HTTP/" << http_ver << "\r\n";
        request_stream << "Host: " << host << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";

        // Start an asynchronous resolve to translate the server and service names
        // into a list of endpoints.
        resolver_.async_resolve(query,
                                boost::bind(&http_client::handle_resolve, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::results));
        io_context.run();
        io_context.reset();
    }

    void post(const std::string &path, const std::string &reqBody) {
        // Form the request. We specify the "Connection: close" header so that the
        // server will close the socket after transmitting the response. This will
        // allow us to treat all data up until the EOF as the content.
        std::ostream request_stream(&request_);
        request_stream << "POST " << path << " HTTP/" << http_ver << "\r\n";
        request_stream << "Host: " << host << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Content-Type: application/json\r\n";
        request_stream << "Content-Length: "<< reqBody.length() << "\r\n";
        request_stream << "Accept-Encoding: gzip, deflate, br\r\n";
        request_stream << "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36\r\n";
        request_stream << "Connection: close\r\n\r\n";
        request_stream << reqBody.c_str();

        // Start an asynchronous resolve to translate the server and service names
        // into a list of endpoints.
        resolver_.async_resolve(query,
                                boost::bind(&http_client::handle_resolve, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::results));
        io_context.run();
        io_context.reset();
    }

    std::string &getHttpVersion() { return http_version; }

    unsigned int &getStatusCode() { return status_code; }

    std::string &getStatusMessage() { return status_message; }

    std::map<std::string, std::string> &getHeaders() { return headers; }

    std::string &getContent() { return content; }

    Status setPbMessage(google::protobuf::Message *message) {
        return JsonStringToMessage(content, message);
    }

private:
    const std::string host;
    const int port;
    const std::string http_ver;

    boost::asio::io_context io_context;
    tcp::resolver::query query;
    tcp::resolver resolver_;
    tcp::socket socket_;
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;

    std::string http_version;
    unsigned int status_code{};
    std::string status_message;
    std::map<std::string, std::string> headers;
    std::string content;

    void handle_resolve(const boost::system::error_code &err,
                        const tcp::resolver::results_type &endpoints) {
        if (!err) {
            // Attempt a connection to each endpoint in the list until we
            // successfully establish a connection.
            boost::asio::async_connect(socket_, endpoints,
                                       boost::bind(&http_client::handle_connect, this,
                                                   boost::asio::placeholders::error));
        } else {
            std::cout << "Error: " << err.message() << "\n";
        }
    }

    void handle_connect(const boost::system::error_code &err) {
        if (!err) {
            // The connection was successful. Send the request.
            boost::asio::async_write(socket_, request_,
                                     boost::bind(&http_client::handle_write_request, this,
                                                 boost::asio::placeholders::error));
        } else {
            std::cout << "Error: " << err.message() << "\n";
        }
    }

    void handle_write_request(const boost::system::error_code &err) {
        if (!err) {
            // Read the response status line. The response_ stream buf will
            // automatically grow to accommodate the entire line. The growth may be
            // limited by passing a maximum size to the stream buf constructor.
            boost::asio::async_read_until(socket_, response_, "\r\n",
                                          boost::bind(&http_client::handle_read_status_line, this,
                                                      boost::asio::placeholders::error));
        } else {
            std::cout << "Error: " << err.message() << "\n";
        }
    }

    void handle_read_status_line(const boost::system::error_code &err) {
        if (!err) {
            // Check that response is OK.
            std::istream response_stream(&response_);
            response_stream >> http_version;
            response_stream >> status_code;
            std::getline(response_stream, status_message);
            if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
                std::cout << "Invalid response\n";
                return;
            }
            if (status_code != 200) {
                std::cout << "Response returned with status code ";
                std::cout << status_code << "\n";
                return;
            }

            // Read the response headers, which are terminated by a blank line.
            boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
                                          boost::bind(&http_client::handle_read_headers, this,
                                                      boost::asio::placeholders::error));
        } else {
            std::cout << "Error: " << err << "\n";
        }
    }

    void handle_read_headers(const boost::system::error_code &err) {
        if (!err) {
            // Process the response headers.
            std::istream response_stream(&response_);
            std::string line;
            std::vector<std::string> kv;
            headers.clear();
            while (std::getline(response_stream, line) && line != "\r") {
                //cout << line << endl;
                boost::split(kv, line.c_str(), boost::is_any_of(":"),
                             boost::token_compress_on);
                //println_vector(kv, "kv");
                boost::trim(kv[1]);
                headers.insert({kv[0], kv[1]});
                kv.clear();
            }

            // Write whatever content we already have to output.
            content = "";
            if (response_.size() > 0) {
                //std::cout << &response_;
                while (std::getline(response_stream, line)) {
                    //cout << "line: " << line << endl;
                    content += line;
                }
            }

            // Start reading remaining data until EOF.
            boost::asio::async_read(socket_, response_,
                                    boost::asio::transfer_at_least(1),
                                    boost::bind(&http_client::handle_read_content, this,
                                                boost::asio::placeholders::error));
        } else {
            std::cout << "Error: " << err << "\n";
        }
    }

    void handle_read_content(const boost::system::error_code &err) {
        if (!err) {
            // Write all the data that has been read so far.
            std::cout << "response_: " << &response_;

            // Continue reading remaining data until EOF.
            boost::asio::async_read(socket_, response_,
                                    boost::asio::transfer_at_least(1),
                                    boost::bind(&http_client::handle_read_content, this,
                                                boost::asio::placeholders::error));
        } else if (err != boost::asio::error::eof) {
            std::cout << "Error: " << err << "\n";
        }
    }

};