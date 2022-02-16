//
// Created by zhaohuiqiang on 2022/2/15.
//
#pragma once
#include "mmdcotservicehttpbroker.pb.h"

void string2bytes(std::string &s, otServicePB::Bytes *bytes) {
    for (char c: s) {
        bytes->mutable_bts()->Add(c);
    }
}

void bytes2string(const otServicePB::Bytes &bytes, std::string &s) {
    for (auto c: bytes.bts()) {
        s += (char)c;
    }
}
