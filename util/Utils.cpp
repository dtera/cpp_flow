//
// Created by zhaohuiqiang on 2021/12/13.
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma clang diagnostic ignored "-Wdangling-gsl"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "Utils.h"

string rsa_pri_decrypt(const string &in, const string &priKey) {
    BIO *pub = BIO_new(BIO_s_mem());
    BIO_write(pub, priKey.c_str(), priKey.length());
    RSA *pRsa = PEM_read_bio_RSAPrivateKey(pub, nullptr, nullptr, nullptr);
    if (pRsa == nullptr) {
        RSA_free(pRsa);
        return "";
    }
    BIO_free_all(pub);
    int key_len = RSA_size(pRsa);

    char *sub_text = (char *) malloc(key_len + 1);
    memset(sub_text, 0, key_len + 1);
    string out;
    for (int i = 0; i < in.length(); i += key_len) {
        string sub_str = in.substr(i, key_len);
        memset(sub_text, i, key_len + 1);
        int out_len = RSA_private_decrypt(sub_str.length(), (unsigned char *) (sub_str.c_str()),
                                          (unsigned char *) (sub_text), pRsa, RSA_PKCS1_PADDING);
        if (out_len >= 0) {
            out.append(string(sub_text, out_len));
        }
    }

    free(sub_text);
    RSA_free(pRsa);

    return out;

}

string rsa_pub_encrypt(const string &in, const string &pubKey) {
    BIO *pub = BIO_new(BIO_s_mem());
    BIO_write(pub, pubKey.c_str(), pubKey.length());
    RSA *pRsa = PEM_read_bio_RSAPublicKey(pub, nullptr, nullptr, nullptr);
    if (pRsa == nullptr) {
        RSA_free(pRsa);
        return "";
    }
    BIO_free_all(pub);
    int key_len = RSA_size(pRsa);
    int block_len = key_len - 11;

    char *sub_text = (char *) malloc(key_len + 1);
    memset(sub_text, 0, key_len + 1);
    string out;
    for (int i = 0; i < in.length(); i += block_len) {
        string sub_str = in.substr(i, block_len);
        memset(sub_text, 0, key_len + 1);
        int out_len = RSA_public_encrypt(sub_str.length(), (unsigned char *) (sub_str.c_str()),
                                         (unsigned char *) (sub_text), pRsa, RSA_PKCS1_PADDING);
        if (out_len >= 0) {
            out.append(string(sub_text, out_len));
        }
    }

    free(sub_text);
    RSA_free(pRsa);

    return out;
}

int gen_rsa_keypair(char *&pubKey, char *&priKey, const bool &writeToFile, const int &keySize,
                    const char *const rsaKeysPath,
                    const char *const pubKeyFileName, const char *const priKeyFileName) {
    RSA *pRsa = RSA_generate_key(keySize, RSA_F4, nullptr, nullptr);
    BIO *pub = BIO_new(BIO_s_mem());
    BIO *pri = BIO_new(BIO_s_mem());

    if (pRsa == nullptr) {
        cout << "RSA_generate_key error" << endl;
        return -1;
    }
    if (PEM_write_bio_RSAPublicKey(pub, pRsa) == 0) {
        cout << "write public key error" << endl;
        return -2;
    }
    if (PEM_write_bio_RSAPrivateKey(pri, pRsa, nullptr, nullptr, 0, nullptr, nullptr) == 0) {
        cout << "write private key error" << endl;
        return -3;
    }

    size_t pubLen = BIO_pending(pub);
    size_t priLen = BIO_pending(pri);

    pubKey = (char *) malloc(pubLen);
    priKey = (char *) malloc(priLen);

    BIO_read(pub, pubKey, pubLen);
    BIO_read(pri, priKey, priLen);

    if (writeToFile) {
        if (access(rsaKeysPath, F_OK)) {
            mkdir(rsaKeysPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
        const char *pubKeyFilePath = (string(rsaKeysPath) + "/" + pubKeyFileName).data();
        const char *priKeyFilePath = (string(rsaKeysPath) + "/" + priKeyFileName).data();

        FILE *pubFile = fopen(pubKeyFilePath, "w");
        if (pubFile == nullptr) {
            cout << "BIO_new_file " << pubKeyFilePath << " error" << endl;
            return -4;
        }
        fputs(pubKey, pubFile);
        fclose(pubFile);

        FILE *priFile = fopen(priKeyFilePath, "w");
        if (priFile == nullptr) {
            cout << "BIO_new_file " << priKeyFilePath << " error" << endl;
            return -5;
        }
        fputs(priKey, priFile);
        fclose(priFile);
    }

    RSA_free(pRsa);
    BIO_free_all(pub);
    BIO_free_all(pri);

    return 0;
}

string gen_random_str(const int &len, const bool &visible_char) {
    srand(time(nullptr) + rand());
    string rs;
    if (visible_char) {
        char extra[] = {'+', '=', '/', '#', '$', '%', '&', '_'};
        for (int i = 0; i < len; i++) {
            int r = rand() % 16;
            if (r == 0) {
                rs += '0' + rand() % 10;
            } else if (r == 1 || r == 2 || r == 3 || r == 4 || r == 5 || r == 6 || r == 7) {
                rs += 'a' + rand() % 26;
            } else if (r == 8 || r == 9 || r == 10 || r == 11 || r == 12 || r == 13 || r == 14) {
                rs += 'A' + rand() % 26;
            } else {
                rs += extra[rand() % (sizeof(extra) / sizeof(char))];
                //rs += '!' + rand() % 94; //('~' - '!' + 1) = 94
            }
        }
    } else {
        for (int i = 0; i < len; i++) {
            rs += rand() % 128; //('~' - '!' + 1) = 94
        }
    }
    return rs;
}

vector<int> str2int_vector(const string &in) {
    vector<int> res;
    for (char c : in) {
        res.push_back(c);
    }
    return res;
}

void println_str2int_vector(const string &s, const string &name, const string &sep) {
    println_vector(str2int_vector(s), name);
}

void println_str2int_vector_vector(const vector<string> &vs, const string &name, const string &sep) {
    for (int i = 0; i < vs.size(); i++) {
        println_vector(str2int_vector(vs[i]), name + "[" + to_string(i) + "]");
    }
}