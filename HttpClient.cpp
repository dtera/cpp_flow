//
// Created by zhaohuiqiang on 2022/1/14.
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedParameter"

#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/text_format.h>
#include <iostream>
#include <string>

#include "common/http/http_client.hpp"
#include "common/util/Utils.h"

#include "mmdcotservicehttpbroker.pb.h"

using namespace std;
using namespace google::protobuf::util;

int main(int argc, char *argv[]) {
    try {
        http_client client(8080, "localhost");
        client.get("/getPublicKey");
        auto headers = client.getHeaders();
        auto content = client.getContent();
        cout<< "\nGET /getPublicKey" << endl;
        println_map(headers, "headers");
        cout<< content << endl;

        otServicePB::Response response;
        //response.ParseFromString(content);
        auto status = JsonStringToMessage(content, &response);
        cout<< response.code() << endl;
        cout<< response.message() << endl;
        cout<< response.data() << endl;

        http_client cli(8081, "localhost:8081");
        string reqBody = "{\n"
                         "  \"1001\": 1,\n"
                         "  \"1002\": 0,\n"
                         "  \"1003\": 1,\n"
                         "  \"1004\": 0,\n"
                         "  \"1005\": 1\n"
                         "}";
        cli.post("/getMessagesOfChosen", reqBody);
        cout<< "\nPOST /getMessagesOfChosen" << endl;
        println_map(cli.getHeaders(), "headers");
        cout<< cli.getContent() << endl;

    } catch (std::exception &e) {
        cout << "Exception: " << e.what() << endl;
    }

    return 0;
}
