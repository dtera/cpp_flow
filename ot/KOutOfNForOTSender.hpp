//
// Created by zhaohuiqiang on 2021/12/12.
//
#pragma once
#pragma ide diagnostic ignored "cert-err34-c"
#pragma ide diagnostic ignored "google-default-arguments"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "common/exceptions/OTError.hpp"
#include "common/util/BitOp.hpp"
#include "ot/BaseOTSender.hpp"

template<class M>
class KOutOfNForOTSender : public BaseOTSender<M> {
public:
    explicit KOutOfNForOTSender(vector<M> &ms);

    explicit KOutOfNForOTSender(vector<M> &ms, const unsigned int &seed);

    void decrypt(vector<string> &encrypted_y_ops, vector<string> &decrypted_y_ops,
                 int reqLimitNum = 0, const bool &secureMode = true) override;
};


//================public begin================
template<class M>
KOutOfNForOTSender<M>::KOutOfNForOTSender(vector<M> &ms): BaseOTSender<M>(ms) {}

template<class M>
KOutOfNForOTSender<M>::KOutOfNForOTSender(vector<M> &ms, const unsigned int &seed): BaseOTSender<M>(ms) {
    this->rbs.clear();
    gen_n_random_str(this->key_size / 8 - 11, seed, this->rbs, ms.size());
}

template<class M>
void KOutOfNForOTSender<M>::decrypt(vector<string> &encrypted_y_ops, vector<string> &decrypted_y_ops,
                                    int reqLimitNum, const bool &secureMode) {
    //cout << "decrypt::publicKey: \n" << this->pub_key << endl;
    //cout << "decrypt::privateKey: \n" << this->pri_key << endl;
    //println_str2int_vector_vector(encrypted_y_ops, "decrypt::encrypted_y_ops");
    if ((secureMode && encrypted_y_ops.size() > 2 * reqLimitNum) || encrypted_y_ops.size() < 2 ||
        atoi(encrypted_y_ops[2].data()) > reqLimitNum) {
        auto reqNum = secureMode ? to_string(encrypted_y_ops.size() / 2) : encrypted_y_ops[2];
        auto err = "The Number " + reqNum + " of requested data must be between 1 and " +
                to_string(reqLimitNum);
        throw OTError(err);
    }

    auto op1 = encrypted_y_ops[0];
    auto op2 = encrypted_y_ops[1];
    if (secureMode) {
        for (int i = 1; i < encrypted_y_ops.size() / 2; ++i) {
            op1 = str_and(op1, encrypted_y_ops[2 * i]);
            op2 = str_and(op2, encrypted_y_ops[2 * i + 1]);
        }
    }
    //cout << "decrypt::op1: " << op1 << endl;
    //cout << "decrypt::op2: " << op2 << endl;

    for (int i = 0; i < this->n; i++) {
        auto t1 = str_and_not(op1, this->rbs[i]);
        auto t2 = str_and(op2, this->rbs[i]);
        auto t3 = str_or(t1, t2);
        auto encrypted_y = str_xor(t3, this->rbs[i]);

        auto decrypted_y = rsa_pri_decrypt(encrypted_y, this->pri_key);

        auto decrypted_y_op = str_xor(decrypted_y, this->mbs[i]);
        decrypted_y_ops.push_back(decrypted_y_op);

        /*println_str2int_vector(t1, "decrypt::t1[" + to_string(i) + "]");
        println_str2int_vector(t2, "decrypt::t2[" + to_string(i) + "]");
        println_str2int_vector(t3, "decrypt::t3[" + to_string(i) + "]");
        println_str2int_vector(encrypted_y, "decrypt::encrypted_y[" + to_string(i) + "]");
        println_str2int_vector(decrypted_y, "decrypt::decrypted_y[" + to_string(i) + "]");*/
    }
    //println_str2int_vector_vector(decrypted_y_ops, "decrypt::decrypted_y_ops");
}
//================public end==================


//================protected begin================

//================protected end==================
