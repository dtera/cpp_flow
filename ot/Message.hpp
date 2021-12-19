//
// Created by zhaohuiqiang on 2021/12/14.
//
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include <vector>

#include "Constant.h"

using namespace std;

class DFMessage {
private:
    vector<string> row;
public:
    byte *toByteArray() {
        return nullptr;
    }

};

class StrMessage {
private:
    string str;
public:
    explicit StrMessage(string &str) : str(str) {}

    byte *toByteArray() {
        return (byte *) str.c_str();
    }

    string &to_str() {
        return str;
    }

    friend ostream &operator<<(ostream &out, StrMessage &strMessage);

};

ostream &operator<<(ostream &out, StrMessage &strMessage) {
    //out << "(" << strMessage.str  << ", " << strMessage.toByteArray() << ")";
    out << strMessage.toByteArray();
    return out;
}
