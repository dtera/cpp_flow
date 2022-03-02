//
// Created by zhaohuiqiang on 2021/12/27.
//

#pragma once
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "performance-inefficient-string-concatenation"

#include <hiredis/hiredis.h>
#include <string>
#include <vector>

#include "common/cli/KVCli.h"
#include "common/util/Utils.h"
#include "ot/Message.hpp"

using namespace std;
using namespace oatpp;

template<typename M>
class RedisCli : public KVCli<M> {
private:
    redisContext *pRedisContext{};
    redisReply *pRedisReply{};
public:
    explicit RedisCli(const string &host = "localhost", const int &port = 6379, const string &password = "");

    ~RedisCli();

    void connect(const string &host = "localhost", const int &port = 6379);

    void get(string &key, string &value);

    void mGet(std::vector<std::string> &keys, std::vector<std::string> &values);

    template<typename F>
    void mGet(std::vector<std::string> &keys, vector<M> &values, F f);

    void hGetAll(std::vector<std::string> &keys, vector<M> &values);

    void set(string &key, string &value);

    void setEx(string &key, string &value, const int &expiredSeconds = 3600);

};

template<typename M>
RedisCli<M>::RedisCli(const string &host, const int &port, const string &password) {
    this->connect(host, port);
    // AUTH with password
    this->pRedisReply = (redisReply *) redisCommand(this->pRedisContext, "AUTH %s", password.c_str());
    printf("AUTH: %s\n", this->pRedisReply->str);
    freeReplyObject(this->pRedisReply);
}

template<typename M>
RedisCli<M>::~RedisCli() {
    delete pRedisContext;
    delete pRedisReply;
}

template<typename M>
void RedisCli<M>::connect(const string &host, const int &port) {
    this->pRedisContext = redisConnect(host.c_str(), port);
    if (this->pRedisContext != nullptr && this->pRedisContext->err) {
        string err = "connect error: ";
        err += this->pRedisContext->errstr;
        err += "\n";
        throw runtime_error(err);
    }
}

template<typename M>
void RedisCli<M>::get(string &key, string &value) {
    this->pRedisReply = (redisReply *) redisCommand(this->pRedisContext, "GET %s", key.c_str());
    if (this->pRedisReply->str != nullptr) {
        value = this->pRedisReply->str;
    }
    freeReplyObject(this->pRedisReply);
}

template<typename M>
void RedisCli<M>::mGet(std::vector<std::string> &keys, std::vector<std::string> &values) {
    auto ks = join_vector(keys, " ");
    this->pRedisReply = (redisReply *) redisCommand(this->pRedisContext, "MGET %s", ks.c_str());
    auto items = this->pRedisReply->element;
    for (int i = 0; i < this->pRedisReply->elements; ++i) {
        values.emplace_back(items[i]->str);
    }
    freeReplyObject(this->pRedisReply);
}

template<typename M>
template<typename F>
void RedisCli<M>::mGet(std::vector<std::string> &keys, vector<M> &values, F f) {
    auto cmd = "MGET " + join_vector(keys, " ");
    this->pRedisReply = (redisReply *) redisCommand(this->pRedisContext, cmd.c_str());
    auto items = this->pRedisReply->element;
    for (int i = 0; i < this->pRedisReply->elements; ++i) {
        M m;
        if (items[i]->str != nullptr) {
            string t = items[i]->str;
            m = f(t);
        }
        values.emplace_back(m);
    }
    freeReplyObject(this->pRedisReply);
}

template<typename M>
void RedisCli<M>::hGetAll(std::vector<std::string> &keys, std::vector<M> &values) {
    for (int i = 0; i < keys.size(); ++i) {
        auto cmd = "HGETALL %s" + join_vector(keys, " ");
        this->pRedisReply = (redisReply *) redisCommand(this->pRedisContext, "HGETALL %s", keys[i].c_str());
        auto items = this->pRedisReply->element;
        Fields<String> fs;
        fs = {};
        for (int j = 0; j < this->pRedisReply->elements / 2; ++j) {
            if (items[2 * j]->str != nullptr) {
                fs->push_back({items[2 * j]->str, items[2 * j + 1]->str});
            }
        }
        freeReplyObject(this->pRedisReply);

        string k = keys[i];
        if (fs->empty()) {
            string v;
            get(k, v);
            fs->push_back({"value", v});
        }
        fs->push_front({"key", k});
        M m(fs);
        values.emplace_back(m);
    }
}

template<typename M>
void RedisCli<M>::set(string &key, string &value) {
    redisCommand(this->pRedisContext, "SET %s %s", key.c_str(), value.c_str());
}

template<typename M>
void RedisCli<M>::setEx(string &key, string &value, const int &expiredSeconds) {
    redisCommand(this->pRedisContext, "SET %s %s EX %d", key.c_str(), value.c_str(), expiredSeconds);
}

