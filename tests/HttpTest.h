//
// Created by zhaohuiqiang on 2022/1/14.
//
#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "UnusedParameter"

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>

#include "common/http/HttpClient.hpp"
#include "common/http/HttpClientOld.hpp"
#include "common/http/server.hpp"
#include "common/util/Utils.h"
#include "mmdcotservicehttpbroker.pb.h"

void httpServerTest() {
    try {
        // Initialise the server.
        http::server3::server s("0.0.0.0", "80", ".", 1);
        // Run the server until stopped.
        s.run();
    }
    catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << "\n";
    }
}

void httpClientTest() {
    try {
        HttpClient client(8080, "localhost");
        client.get("/getPublicKey");
        auto headers = client.getHeaders();
        auto content = client.getContent();
        cout << "\nGET /getPublicKey" << endl;
        println_map(headers, "headers");
        cout << content << endl;

        otServicePB::Response response;
        //response.ParseFromString(content);
        auto status = client.setPbMessage(response);
        cout << status << endl;
        cout << response.code() << endl;
        cout << response.message() << endl;
        cout << response.data() << endl;

        HttpClient cli(8081, "localhost:8081");
        string reqBody = "{\n"
                         "  \"1001\": 1,\n"
                         "  \"1002\": 0,\n"
                         "  \"1003\": 1,\n"
                         "  \"1004\": 0,\n"
                         "  \"1005\": 1\n"
                         "}";
        cli.post("/getMessagesOfChosen", reqBody);
        cout << "\nPOST /getMessagesOfChosen" << endl;
        println_map(cli.getHeaders(), "headers");
        cout << cli.getContent() << endl;

        otServicePB::OTClientRequest req;
        auto uinWithLabelMap = req.mutable_uinwithlabelmap();
        uinWithLabelMap->insert({1001, 1});
        uinWithLabelMap->insert({1002, 0});
        uinWithLabelMap->insert({1003, 1});
        uinWithLabelMap->insert({1004, 0});
        uinWithLabelMap->insert({1005, 1});

        cli.post("/getMessages", req, true);
        cout<< "\nPOST /getMessages" << endl;
        println_map(cli.getHeaders(), "headers");
        cout<< cli.getContent() << endl;

    } catch (std::exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
}

void httpClientOldTest() {
    try {
        HttpClientOld client(8080, "localhost");
        client.get("/getPublicKey");
        auto headers = client.getHeaders();
        auto content = client.getContent();
        cout << "\n[HttpClientOld]GET /getPublicKey" << endl;
        println_map(headers, "headers");
        cout << content << endl;

        otServicePB::Response response;
        //response.ParseFromString(content);
        auto status = client.setPbMessage(response);
        cout << "status: " << status << endl;
        cout << "code: " << response.code() << endl;
        cout << "message: " << response.message() << endl;
        cout << "data: \n" << response.data() << endl;

        HttpClientOld cli(8081, "localhost");
        string reqBody = "{\n"
                         "  \"1001\": 1,\n"
                         "  \"1002\": 0,\n"
                         "  \"1003\": 1,\n"
                         "  \"1004\": 0,\n"
                         "  \"1005\": 1\n"
                         "}";
        cli.post("/getMessagesOfChosen", reqBody);
        cout << "\n[HttpClientOld]POST /getMessagesOfChosen" << endl;
        println_map(cli.getHeaders(), "headers");
        cout << cli.getContent() << endl;

        otServicePB::OTClientRequest req;
        auto uinWithLabelMap = req.mutable_uinwithlabelmap();
        uinWithLabelMap->insert({1001, 1});
        uinWithLabelMap->insert({1002, 0});
        uinWithLabelMap->insert({1003, 1});
        uinWithLabelMap->insert({1004, 0});
        uinWithLabelMap->insert({1005, 1});

        cli.post("/getMessages", req, true);
        cout<< "\n[HttpClientOld]POST /getMessages" << endl;
        println_map(cli.getHeaders(), "headers");
        cout<< cli.getContent() << endl;

    } catch (std::exception &e) {
        cout << "Exception: " << e.what() << endl;
    }
}
