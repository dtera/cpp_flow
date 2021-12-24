//
// Created by zhaohuiqiang on 2021/12/17.
//

#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <string>
#include "../../ot/Message.hpp"

using namespace std;

typedef string (*str2str_fp)(string &t);

typedef StrMessage (*str2str_message_fp)(string &t);
