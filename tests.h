#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <iostream>
#include <string>
#include <functional>

#include "ot/KOutOfNForOTSender.hpp"
#include "ot/KOutOfNForOTReceiver.hpp"
#include "ot/Message.hpp"
#include "util/Utils.h"
#include "util/Func.h"


string ot_test() {
    string s1 = "abc", s2 = "def", s3 = "ghi", s4 = "jkl", s5 = "mno";
    vector<StrMessage> str_ms = {StrMessage(s1), StrMessage(s2), StrMessage(s3),
                                 StrMessage(s4), StrMessage(s5)};
    //for_each(str_ms.begin(), str_ms.end(), println<StrMessage>);

    KOutOfNForOTSender<StrMessage> otSender(str_ms);
    // 1. otSender send publicKey and random msg to otReceiver
    auto pk = BaseOTSender<StrMessage>::get_pub_key();
    auto rs = otSender.get_rs();
    //cout << pk << endl;
    BaseOTReceiver::set_pub_key(pk);
    vector<int> choices = {2};
    KOutOfNForOTReceiver otReceiver(choices);
    // 2. otReceiver encrypt key y with publicKey and random msg
    vector<string> encrypted_y_ops;
    otReceiver.encrypt(rs, encrypted_y_ops);
    //println_vector(encrypted_y_ops, "encrypted_y_ops");
    // 3. otSender decrypt the encrypted key and send the decryptedYXorMs to otReceiver
    vector<string> decrypted_y_ops;
    otSender.decrypt(encrypted_y_ops, decrypted_y_ops);
    //println_vector(decrypted_y_ops, "decrypted_y_ops");
    // 4. otReceiver obtain the chosen message
    vector<string> decrypted_ms;
    str2str_fp f = nullptr;
    otReceiver.get_messages(decrypted_y_ops, decrypted_ms, f);
    for_each(decrypted_ms.begin(), decrypted_ms.end(), println<string>);

    return decrypted_ms[choices[0]];
    /*for (int i = 0; i < str_ms.size(); i++) {
        cout << rs[i] << endl;
    }*/

}

void bit_op_test() {
    string s1 = "abc-ghi", s2 = "def-cvx-kjk343242";
    string s = str_xor(s1, s2);
    cout << "str_xor(s1, s2): " << s << endl;
    s = str_xor(s1, s);
    cout << "str_xor(s1, s): " << s << endl;

    s = str_and(s1, s2);
    cout << "str_and(s1, s2): " << s << endl;
    s = str_or(s1, s2);
    cout << "str_or(s1, s2): " << s << endl;
    s = str_not(s1);
    cout << "str_not(s1): " << s << endl;

}

void rsa_test() {
    char a[] = "abc-def-ghi-jkl12324354646453413131435345456454645646464645645667608-7423440-976542324534677842428976";
    char b[] = "dad-gfd-hre-oqe73489324798249812830980479832579835190840918075893425018018509185093485075680350918501";
    //cout << &a << endl << endl;
    //int len = sizeof(a) / sizeof(char);
    //cout << index_of(a, len, 'l') << endl;

    char *pubKey = nullptr;
    char *priKey = nullptr;
    gen_rsa_keypair(pubKey, priKey, true);
    cout << pubKey << endl;
    cout << priKey << endl;

    string encrypted_str = rsa_pub_encrypt(string(a) + b, pubKey);
    cout << encrypted_str << endl;
    string decrypted_str = rsa_pri_decrypt(encrypted_str, priKey);
    cout << decrypted_str << endl;
}

void random_str_test() {
    const string s1 = gen_random_str(100);
    const string s2 = gen_random_str(100);
    const string s3 = gen_random_str(100);
    const string s4 = gen_random_str(100, false);

    cout << s1 << endl;
    cout << s2 << endl;
    cout << s3 << endl;
    cout << s4 << endl;
}

