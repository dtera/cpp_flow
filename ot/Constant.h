//
// Created by zhaohuiqiang on 2021/12/12.
//
#pragma once
#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <string>

using namespace std;

const int DEFAULT_KEY_SIZE = 1024;
const string ASYMMETRIC_ALGORITHM = "RSA/ECB/PKCS1Padding";
const string RSA_KEYS_PATH = "rsa_keys";
const string RSA_PUBLIC_KEY_FILE_NAME = "id_rsa.pub";
const string RSA_PRIVATE_KEY_FILE_NAME = "id_rsa";

typedef unsigned char byte;

#pragma clang diagnostic pop
