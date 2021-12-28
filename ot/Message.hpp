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

//=========================FieldsMessage=========================
class FieldsMessage {
private:
    oatpp::Fields<oatpp::String> fields;
    string fieldSep = ",";
    string kvSep = ":";
public:
    explicit FieldsMessage() : fields({}) {}

    explicit FieldsMessage(oatpp::Fields<oatpp::String> &fields) : fields(fields) {}

    explicit FieldsMessage(string &str) : fields({}) {
        string k, v;
        bool isTurnOfKey = true;
        for (auto c: str) {
            if (c == fieldSep[0]) {
                fields->push_back({k, v});
                isTurnOfKey = true;
                k = "";
                v = "";
            } else if (c == kvSep[0]) {
                isTurnOfKey = false;
            } else {
                if (isTurnOfKey) {
                    k += c;
                } else {
                    v += c;
                }
            }
        }
        fields->push_back({k, v});
    }

    string to_str() {
        return join_Fields(fields, fieldSep, kvSep);
    }

    oatpp::Fields<oatpp::String> &getFields() {
        return fields;
    }

    friend ostream &operator<<(ostream &out, FieldsMessage &fieldsMessage);

};


ostream &operator<<(ostream &out, FieldsMessage &fieldsMessage) {
    out << fieldsMessage.to_str();
    return out;
}

FieldsMessage str2fieldsMessage(string &t) {
    FieldsMessage fieldsMessage(t);
    return fieldsMessage;
}

//=========================StrMessage=========================
class StrMessage {
private:
    string str;
public:
    explicit StrMessage() = default;

    explicit StrMessage(string &str) : str(str) {}

    string &to_str() {
        return str;
    }

    friend ostream &operator<<(ostream &out, StrMessage &strMessage);

};

ostream &operator<<(ostream &out, StrMessage &strMessage) {
    out << strMessage.to_str();
    return out;
}

StrMessage str2strMessage(string &t) {
    StrMessage strMessage(t);
    return strMessage;
}