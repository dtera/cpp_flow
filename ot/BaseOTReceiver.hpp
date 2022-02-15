//
// Created by zhaohuiqiang on 2021/12/12.
//
#pragma once
#pragma ide diagnostic ignored "google-default-arguments"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <vector>
//#include <openssl/md5.h>

#include "Constant.h"

#include "../common/util/Utils.h"
#include "../common/util/BitOp.hpp"

using namespace std;

class BaseOTReceiver {
protected:
    // which data you want to receive
    vector<int> choices;
    // random key
    vector<string> ybs;
    // random message size
    const int random_size = key_size / 8 - 11;

    // public key
    static char *pub_key;
    // key size, must be devided by 8
    static const int key_size = DEFAULT_KEY_SIZE;

public:
    explicit BaseOTReceiver(vector<int> &choices);

    ~BaseOTReceiver() = default;

    void set_choices(vector<int> &cs);

    void init();

    template<typename M, typename F>
    void get_messages(vector<string> &decrypted_y_ops, vector<M> &decrypted_ms,
                      F f = nullptr, bool only_chosen = false);

    template<typename M, typename F>
    void get_messages_of_chosen(vector<string> &decrypted_y_ops, vector<M> &decrypted_ms, F f = nullptr);

    virtual void encrypt(vector<string> &rs, vector<string> &encrypted_y_ops, const string &publicKey = "") = 0;

    static void set_pub_key(char *pk);

};


//================public begin================

BaseOTReceiver::BaseOTReceiver(vector<int> &choices) : choices(choices)/*, ybs(nullptr)*/ {
    init();
}

/*BaseOTReceiver::~BaseOTReceiver() {
    free(ybs);
    *//*if (this->ybs != nullptr) {
        delete this->ybs;
    }*//*
}*/

void BaseOTReceiver::set_choices(vector<int> &cs) {
    this->choices = cs;
    init();
}

void BaseOTReceiver::init() {
    auto len = 1; //choices.size();
    //ybs = new string[len];
    ybs.clear();

    for (int i = 0; i < len; i++) {
        //ybs[i] = gen_random_str(random_size);
        ybs.push_back(gen_random_str(random_size));
    }
}

template<typename M, typename F>
void BaseOTReceiver::get_messages(vector<string> &decrypted_y_ops, vector<M> &decrypted_ms, F f, bool only_chosen) {
    auto y = ybs[0];
    if (only_chosen) {
        for (int choice : choices) {
            auto t = str_xor(decrypted_y_ops[choice], y);
            M m(t);
            if (f != nullptr) {
                m = f(t);
            }
            decrypted_ms.push_back(m);
        }
    } else {
        for (int i = 0; i < decrypted_y_ops.size(); i++) {
            string t;
            if (binary_search(choices.begin(), choices.end(), i)) {
                t = str_xor(decrypted_y_ops[i], y);
            } else {
                t = gen_random_str(random_size);
            }
            M m(t);
            if (f != nullptr) {
                m = f(t);
            }
            decrypted_ms.push_back(m);
        }
    }

}

template<typename M, typename F>
void BaseOTReceiver::get_messages_of_chosen(vector<string> &decrypted_y_ops, vector<M> &decrypted_ms, F f) {
    get_messages(decrypted_y_ops, decrypted_ms, f, true);
}

void BaseOTReceiver::set_pub_key(char *pk) {
    if (pub_key == nullptr) {
        pub_key = pk;
    }
}
//================public end==================


//================protected begin================
char *BaseOTReceiver::pub_key = nullptr;
//================protected end==================
