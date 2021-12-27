//
// Created by zhaohuiqiang on 2021/12/13.
//
#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-avoid-const-params-in-decls"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
/*
#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "ws2_32.lib")
*/

#include <iostream>
#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>
#include <ctime>
#include <string>
#include <vector>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <oatpp/core/Types.hpp>

#include "../../ot/Constant.h"

string rsa_pri_decrypt(const string &in, const string &pri_key);

string rsa_pub_encrypt(const string &in, const string &pub_key);

int gen_rsa_keypair(char *&pub_key, char *&pri_key, const bool &write_to_file = false,
                    const int &key_size = DEFAULT_KEY_SIZE, const char *const rsa_keys_path = RSA_KEYS_PATH.data(),
                    const char *const pub_key_file_name = RSA_PUBLIC_KEY_FILE_NAME.data(),
                    const char *const pri_key_file_name = RSA_PRIVATE_KEY_FILE_NAME.data());

void fread_all(char *&out, FILE *p_file);

string gen_random_str(const int &len, const bool &visible_char = true);

vector<int> str2int_vector(const string &in);

void println_str2int_vector(const string &s, const string &name, const string &sep = ", ");

void println_str2int_vector_vector(const vector<string> &vs, const string &name, const string &sep = ", ");

template<typename T>
int index_of(const T *ts, const int &len, const T &t) {
    for (int i = 0; i < len; i++) {
        if (ts[i] == t) {
            return i;
        }
    }
    return -1;
}

template<typename T>
void println(T &t) {
    cout << t << endl;
}

template<typename T>
void println_vector(const vector<T> &vs, const string &name, const string &sep = ", ") {
    cout << name << "(len:" << vs.size() << ")--> [";
    if (vs.size() != 0) {
        cout << vs[0];
    }
    for (int i = 1; i < vs.size(); i++) {
        cout << sep << vs[i];
    }
    cout << "]" << endl;
}

template<typename T>
string join_vector(const vector<T> &vs, const string &sep = ",", const string &start = "", const string &end = "") {
    string res = start;
    if (vs.size() != 0) {
        res += vs[0];
    }
    for (int i = 1; i < vs.size(); i++) {
        res += sep + vs[i];
    }
    res += end;
    return res;
}

template<typename T>
string join_Vector(const oatpp::Vector<T> &vs, const string &sep = ",",
                   const string &start = "", const string &end = "") {
    string res = start;
    if (!vs->empty()) {
        res += vs[0];
    }
    for (int i = 1; i < vs->size(); i++) {
        res += sep + vs[i];
    }
    res += end;
    return res;
}
