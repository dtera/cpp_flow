//
// Created by zhaohuiqiang on 2021/12/13.
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma clang diagnostic ignored "-Wdangling-gsl"
#pragma ide diagnostic ignored "misc-no-recursion"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

#include "Utils.h"


string rsa_pri_decrypt(const string &in, const string &pri_key) {
    BIO *pub = BIO_new(BIO_s_mem());
    BIO_write(pub, pri_key.c_str(), pri_key.length());
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
        } else {
            out.append(gen_random_str(DEFAULT_KEY_SIZE / 8 - 11));
        }
    }

    free(sub_text);
    RSA_free(pRsa);

    return out;

}

string rsa_pub_encrypt(const string &in, const string &pub_key) {
    BIO *pub = BIO_new(BIO_s_mem());
    BIO_write(pub, pub_key.c_str(), pub_key.length());
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

int gen_rsa_keypair(char *&pub_key, char *&pri_key, const bool &write_to_file, const int &key_size,
                    const char *const rsa_keys_path,
                    const char *const pub_key_file_name, const char *const pri_key_file_name) {
    if (access(rsa_keys_path, F_OK)) {
        mkdir(rsa_keys_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    const char *pub_key_file_path = (string(rsa_keys_path) + "/" + pub_key_file_name).data();
    const char *pri_key_file_path = (string(rsa_keys_path) + "/" + pri_key_file_name).data();
    FILE *pub_file = fopen(pub_key_file_path, "r");
    FILE *pri_file = fopen(pri_key_file_path, "r");
    if (pub_file != nullptr && pri_file != nullptr) {
        cout << "Read rsa_keypair from " << pub_key_file_path << " and " << pri_key_file_path << endl;
        fread_all(pub_key, pub_file);
        fread_all(pri_key, pri_file);
        return 0;
    }

    RSA *pRsa = RSA_generate_key(key_size, RSA_F4, nullptr, nullptr);
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

    pub_key = (char *) malloc(pubLen);
    pri_key = (char *) malloc(priLen);

    BIO_read(pub, pub_key, pubLen);
    BIO_read(pri, pri_key, priLen);

    if (write_to_file) {
        pub_file = fopen(pub_key_file_path, "w");
        if (pub_file == nullptr) {
            cout << "BIO_new_file " << pub_key_file_path << " error" << endl;
            return -4;
        }
        fputs(pub_key, pub_file);
        fclose(pub_file);

        pri_file = fopen(pri_key_file_path, "w");
        if (pri_file == nullptr) {
            cout << "BIO_new_file " << pri_key_file_path << " error" << endl;
            return -5;
        }
        fputs(pri_key, pri_file);
        fclose(pri_file);
    }

    RSA_free(pRsa);
    BIO_free_all(pub);
    BIO_free_all(pri);

    return 0;
}

void fread_all(char *&out, FILE *p_file) {
    fseek(p_file, 0, SEEK_END);
    auto len = ftell(p_file);
    out = (char *) malloc(len);
    rewind(p_file);

    string buf;
    char line[256];
    while (!feof(p_file) && !ferror(p_file)) {
        strcpy(line, "\n");
        fgets(line, sizeof(line), p_file);
        buf += line;
    }
    copy(buf.begin(), buf.end(), out);
    fclose(p_file);
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
    for (char c: in) {
        res.push_back(c);
    }
    return res;
}

inline void split(const string &in, vector<string> &out, const string &sep) {
    auto i = in.find_first_not_of(sep, 0);
    auto j = in.find_first_of(sep, i);
    while (string::npos != i || string::npos != j) {
        auto t = in.substr(i, j - i);
        boost::trim(t);
        out.push_back(t);
        i = in.find_first_not_of(sep, j);
        j = in.find_first_of(sep, i);
    }
}

inline vector<string> regex_split(const string &in, const string &reg_sep) {
    regex regex_sep(reg_sep);
    auto res  = sregex_token_iterator(in.begin(), in.end(), regex_sep, -1);
    vector<string> out(res, sregex_token_iterator());
    return out;
}

void ini_to_map(boost::property_tree::ptree &pt, map<string, string> &data, const string &prefix, const string &sep) {
    for (auto p: pt) {
        auto key = (prefix.empty() ? "" : prefix + sep) + p.first;
        if (p.second.empty()) {
            data.insert({key, p.second.data()});
        } else {
            ini_to_map(p.second, data, key, sep);
        }
    }
}

void println_str2int_vector(const string &s, const string &name, const string &sep) {
    println_vector(str2int_vector(s), name);
}

void println_str2int_vector_vector(const vector<string> &vs, const string &name, const string &sep) {
    for (int i = 0; i < vs.size(); i++) {
        println_vector(str2int_vector(vs[i]), name + "[" + to_string(i) + "]");
    }
}