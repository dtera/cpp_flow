//
// Created by zhaohuiqiang on 2021/12/12.
//
#pragma once
#pragma ide diagnostic ignored "google-default-arguments"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "ot/BaseOTReceiver.hpp"

class KOutOfNForOTReceiver : public BaseOTReceiver {
public:
    explicit KOutOfNForOTReceiver(vector<int> &choices);

    void encrypt(vector<string> &rs, vector<string> &encrypted_y_ops, const string &publicKey = "") override;
};


//================public begin================
KOutOfNForOTReceiver::KOutOfNForOTReceiver(vector<int> &choices) : BaseOTReceiver(choices) {}

void KOutOfNForOTReceiver::encrypt(vector<string> &rs, vector<string> &encrypted_y_ops, const string &publicKey) {
    //cout << "encrypt::publicKey: \n" << pub_key << endl;

    auto encrypted_y = rsa_pub_encrypt(ybs[0], publicKey.empty() ? pub_key : publicKey);
    auto encrypted_y_not = str_not(encrypted_y);
    auto co_and_rs = str_co_and(rs, &choices);
    auto co_and_rs_not = str_co_and(rs, &choices, true);

    auto t1 = str_or(encrypted_y, co_and_rs);
    //auto t2 = str_or(encrypted_y_not, co_and_rs_not);
    string t2;
    str_bit_bin_op(encrypted_y_not, co_and_rs_not, t2, c_or, true, true,
                   char_255, char_self);
    encrypted_y_ops.push_back(t1);
    encrypted_y_ops.push_back(t2);
    encrypted_y_ops.push_back(to_string(choices.size()));
    //encrypted_y_ops.push_back(str_or(encrypted_y_not, co_and_rs_not));

    /*println_vector(rs, "encrypt::rs");
    println_vector(ybs, "encrypt::ybs");
    println_str2int_vector_vector(ybs, "encrypt::ybs");
    println_str2int_vector(encrypted_y, "encrypt::encrypted_y");
    println_str2int_vector(encrypted_y_not, "encrypt::encrypted_y_not");
    println_str2int_vector(co_and_rs, "encrypt::co_and_rs");
    println_str2int_vector(co_and_rs_not, "encrypt::co_and_rs_not");
    println_str2int_vector_vector(encrypted_y_ops, "encrypt::encrypted_y_ops");
    cout << endl;*/
}
//================public end==================


//================protected begin================

//================protected end==================