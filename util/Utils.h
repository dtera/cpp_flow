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

#include "../ot/Constant.h"

string rsa_pri_decrypt(const string &in, const string &priKey);

string rsa_pub_encrypt(const string &in, const string &pubKey);

int gen_rsa_keypair(char *&pubKey, char *&priKey, const bool &writeToFile = false,
                    const int &keySize = DEFAULT_KEY_SIZE, const char *const rsaKeysPath = RSA_KEYS_PATH.data(),
                    const char *const pubKeyFileName = RSA_PUBLIC_KEY_FILE_NAME.data(),
                    const char *const priKeyFileName = RSA_PRIVATE_KEY_FILE_NAME.data());

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
