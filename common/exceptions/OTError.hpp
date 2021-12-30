//
// Created by zhaohuiqiang on 2021/12/29.
//
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include<iostream>
#include<stdexcept>

using namespace std;

class OTError : public exception {
private:
    string m_p;
public:
    explicit OTError(char *c) {
        m_p = c;
    }

    explicit OTError(string &s) {
        m_p = s;
    }

    virtual string &what() {
        return m_p;
    }
};
