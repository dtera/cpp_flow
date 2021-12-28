//
// Created by zhaohuiqiang on 2021/12/28.
//
#pragma once
#pragma ide diagnostic ignored "UnusedParameter"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <map>
#include <string>

using namespace std;

class ParamsTool {
private:
    static map<string, string> data;
public:
    static void fromArgs(int argc, const char * argv[]) {

    }
};
