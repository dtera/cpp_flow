//
// Created by zhaohuiqiang on 2021/12/27.
//

#pragma once
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <string>
#include <hiredis/hiredis.h>
#include "util/Utils.h"

using namespace std;

class RedisCli {
private:
    redisContext *pRedisContext{};
    redisReply *pRedisReply{};
public:
    explicit RedisCli(const string &host = "localhost", const int &port = 6379);

    ~RedisCli();

    void connect(const string &host = "localhost", const int &port = 6379);

    void get(string &key, string &value);

    void mGet(vector<string> &keys, vector<string> &values);

    template<typename M, typename F>
    void mGet(oatpp::Vector<oatpp::String> &keys, vector<M> &values, F f);

    void set(string &key, string &value);

    void setEx(string &key, string &value, const int &expiredSeconds = 3600);

};

RedisCli::RedisCli(const string &host, const int &port) {
    this->connect(host, port);
}

RedisCli::~RedisCli() {
    this->pRedisContext = nullptr;
    this->pRedisReply = nullptr;
}

void RedisCli::connect(const string &host, const int &port) {
    this->pRedisContext = redisConnect(host.c_str(), port);
    if (this->pRedisContext != nullptr && this->pRedisContext->err) {
        string err = "connect error: ";
        err += this->pRedisContext->errstr;
        err += "\n";
        throw runtime_error(err);
    }
}

void RedisCli::get(string &key, string &value) {
    this->pRedisReply = (redisReply *) redisCommand(this->pRedisContext, "GET %s", key.c_str());
    value = this->pRedisReply->str;
    freeReplyObject(this->pRedisReply);
}

void RedisCli::mGet(vector<string> &keys, vector<string> &values) {
    auto ks = join_vector(keys, " ");
    this->pRedisReply = (redisReply *) redisCommand(this->pRedisContext, "MGET %s", ks.c_str());
    auto items = this->pRedisReply->element;
    for (int i = 0; i < this->pRedisReply->elements; ++i) {
        values.emplace_back(items[i]->str);
    }
    freeReplyObject(this->pRedisReply);
}

template<typename M, typename F>
void RedisCli::mGet(oatpp::Vector<oatpp::String> &keys, vector<M> &values, F f) {
    auto cmd = "MGET " + join_Vector(keys, " ");
    this->pRedisReply = (redisReply *) redisCommand(this->pRedisContext, cmd.c_str());
    auto items = this->pRedisReply->element;
    for (int i = 0; i < this->pRedisReply->elements; ++i) {
        string t = items[i]->str;
        M m = f(t);
        values.emplace_back(m);
    }
    freeReplyObject(this->pRedisReply);
}

void RedisCli::set(string &key, string &value) {
    redisCommand(this->pRedisContext, "SET %s %s", key.c_str(), value.c_str());
}

void RedisCli::setEx(string &key, string &value, const int &expiredSeconds) {
    redisCommand(this->pRedisContext, "SET %s %s EX %d", key.c_str(), value.c_str(), expiredSeconds);
}
