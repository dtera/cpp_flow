//
// Created by zhaohuiqiang on 2021/12/14.
//
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

#include <vector>

#include "Constant.h"
#include "../common/util/Utils.h"

using namespace std;

class DFMessage {
private:
    vector<string> row;
    const string sep = ":";
public:

    byte *toByteArray() {
        return nullptr;
    }

    string to_str() {
        return join_vector(row, sep);
    }

    friend ostream &operator<<(ostream &out, DFMessage &dfMessage);

};

ostream &operator<<(ostream &out, DFMessage &dfMessage) {
    out << dfMessage.to_str();
    return out;
}

class StrMessage {
private:
    string str;
public:
    explicit StrMessage() = default;

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

StrMessage str2strMessage(string &t) {
    StrMessage strMessage(t);
    return strMessage;
}