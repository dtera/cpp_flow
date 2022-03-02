//
// Created by zhaohuiqiang on 2021/12/27.
//
#pragma once
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <string>
#include <vector>

using namespace std;

template<typename M>
class KVCli {
public:
    virtual ~KVCli() = default;

    virtual void hGetAll(std::vector<std::string> &keys, vector<M> &values) = 0;
};
