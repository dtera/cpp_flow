//
// Created by zhaohuiqiang on 2021/12/12.
//
#pragma once
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <vector>
//#include <openssl/md5.h>

#include "Constant.h"

#include "../util/Utils.h"

using namespace std;

template<class M>
class BaseOTSender {
protected:
    // n size of message
    vector<M> ms;
    // length of message
    int n;
    // n size of message bytes
    //byte **mbs;
    //string *mbs;
    vector<string> mbs;
    // n size of random message bytes corresponding to choices
    //byte **rbs;
    //string *rbs;
    vector<string> rbs;

    // public key
    static char *pub_key;
    // private key
    static char *pri_key;
    // key size, must be devided by 8
    static const int key_size = DEFAULT_KEY_SIZE;

    static void init();

public:
    explicit BaseOTSender(vector<M> &ms);

    ~BaseOTSender() = default;

    vector<string> &get_rs();

    virtual void decrypt(vector<string> &encrypted_y_ops, vector<string> &decrypted_y_ops) = 0;

    static char *get_pub_key();

    static void gen_keypair();

};


//================public begin================
template<class M>
BaseOTSender<M>::BaseOTSender(vector<M> &ms): ms(ms), n(ms.size())/*, mbs(nullptr), rbs(nullptr)*/ {
    if (this->pub_key == nullptr || this->pri_key == nullptr) {
        this->init();
    }
    int random_size = key_size / 8 - 11;
    //this->mbs = new byte[n];
    //this->rbs = new byte[n][random_size];
    /*
    this->mbs = (byte **) malloc(n);
    this->rbs = (byte **) malloc(n * random_size);
    memset(this->mbs, 0, n);
    memset(this->rbs, 0, n * (key_size / 8 - 11));
    this->mbs = new string[n];
    this->rbs = new string[n];
    */
    for (int i = 0; i < n; i++) {
        /*mbs[i] = choices[i].toByteArray();
        rbs[i] = (byte *) gen_random_str(random_size).c_str();
        mbs[i] = ms[i].to_str();
        rbs[i] = gen_random_str(random_size);*/
        mbs.push_back(ms[i].to_str());
        rbs.push_back(gen_random_str(random_size));
    }
}

/*template<class M>
BaseOTSender<M>::~BaseOTSender() {
    free(mbs);
    free(rbs);
    // if (this->mbs != nullptr) {
    //     delete this->mbs;
    // }
    // if (this->rbs != nullptr) {
    //     delete this->rbs;
    // }
}*/

template<class M>
vector<string> &BaseOTSender<M>::get_rs() {
    return rbs;
}

template<class M>
char *BaseOTSender<M>::get_pub_key() {
    //return FileUtil.readUtf8String(getRsaKeyPath(RSA_PUBLIC_KEY_FILE_NAME));
    return pub_key;
}

template<class M>
void BaseOTSender<M>::gen_keypair() {
    gen_rsa_keypair(pub_key, pri_key, true);
}
//================public end==================


//================protected begin================
template<class M>
char *BaseOTSender<M>::pub_key = nullptr;

template<class M>
char *BaseOTSender<M>::pri_key = nullptr;

template<class M>
void BaseOTSender<M>::init() {
    gen_keypair();
}
//================protected end==================