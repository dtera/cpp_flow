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

#include "common/util/Utils.h"


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
                    const string &rsa_keys_path,
                    const string &pub_key_file_name, const string &pri_key_file_name) {
    if (access(rsa_keys_path.data(), F_OK)) {
        mkdir(rsa_keys_path.data(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    string pub_key_file_path = string(rsa_keys_path) + "/" + pub_key_file_name;
    string pri_key_file_path = string(rsa_keys_path) + "/" + pri_key_file_name;
    FILE *pub_file = fopen(pub_key_file_path.data(), "r");
    FILE *pri_file = fopen(pri_key_file_path.data(), "r");
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
        pub_file = fopen(pub_key_file_path.data(), "w");
        if (pub_file == nullptr) {
            cout << "BIO_new_file " << pub_key_file_path.data() << " error" << endl;
            return -4;
        }
        fputs(pub_key, pub_file);
        fclose(pub_file);

        pri_file = fopen(pri_key_file_path.data(), "w");
        if (pri_file == nullptr) {
            cout << "BIO_new_file " << pri_key_file_path.data() << " error" << endl;
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

void gen_n_random_str(const int &len, const unsigned int &seed, vector<string> &rs, const int &n) {
    for (int i = 0; i < n; ++i) {
        auto r = gen_random_str(len, seed + i, false);
        rs.emplace_back(r);
    }
}

string gen_random_str(const int &len, const unsigned int &seed, const bool &seed_with_time, const bool &visible_char) {
    auto sr = (seed == 0 ? arc4random() : seed) + (seed_with_time ? time(nullptr) : 0);
    srand(sr);
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

bool str_escape_contains(string &s, const char c) {
    char p = ' ';
    for (char a: s) {
        if (a == c && p != '\\') {
            return true;
        }
        p = a;
    }
    return false;
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
    auto res = sregex_token_iterator(in.begin(), in.end(), regex_sep, -1);
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

void read_json2pb(const string &path, Message &msg) {
    ptree tree;
    json_parser::read_json(path, tree);
    stringstream ss;
    json_parser::write_json(ss, tree);

    util::JsonStringToMessage(ss.str(), &msg);
}

double sigmoid(double x, double max_x) {
    if (x < -max_x) {
        return 0.0;
    } else if (x > max_x) {
        return 1.0;
    } else {
        return 1.0 / (1.0 + exp(-x));
    }
}

void pb_softmax(RepeatedField<double> *values) {
    auto len = values->size();
    auto alpha = *max_element(values->begin(), values->end());
    double denominator = 0;

    for (int i = 0; i < len; ++i) {
        values->Set(i, exp(values->Get(i) - alpha));
        denominator += values->Get(i);
    }

    for (int i = 0; i < len; ++i) {
        values->Set(i, values->Get(i) / denominator);
    }
}

int pb_argmax(RepeatedField<double> *values) {
    int maxIndex = 0;
    double max = values->Get(maxIndex);
    for (int i = 1; i < values->size(); i++) {
        if (values->Get(i) > max) {
            maxIndex = i;
            max = values->Get(i);
        }
    }
    return maxIndex;
}

void load_dir_names_from_path(string &path, vector<string> &dir_names) {
    dir_names.clear();
    boost::filesystem::path f_path(path);
    boost::filesystem::directory_iterator iter(f_path);
    while (iter != boost::filesystem::directory_iterator()) {
        if (boost::filesystem::is_directory(iter->path())) {
            string stem = iter->path().stem().string();
            string extension = iter->path().extension().string();
            dir_names.emplace_back(stem + extension);
        }
        ++iter;
    }
}

[[maybe_unused]] string msg_to_json_str(Message &message) {
    string output;
    google::protobuf::util::MessageToJsonString(message, &output);
    return output;
}

[[maybe_unused]] int read_csv(const string &path,
                              const function<void(vector<string>)> consumer) { // NOLINT
    ifstream in(path.c_str());
    if (!in.is_open())
        return -1;

    vector<string> vec;
    string line;
    while (getline(in, line)) {
        boost::tokenizer<boost::escaped_list_separator<char>> tok(line);
        vec.assign(tok.begin(), tok.end());
        if (consumer != nullptr) {
            consumer(vec);
        }
    }
    return 0;
}

[[maybe_unused]] int read_csv(const string &path, vector<vector<string>> *out) {
    return read_csv(path, [out](vector<string> vec) { out->emplace_back(vec); }); // NOLINT
}

[[maybe_unused]] int read_csv(const string &path,
                              unordered_map<string, vector<string>> *out) {
    return read_csv(path, [out](vector<string> vec) {
        out->insert({vec[0], vec});
    });
}

[[maybe_unused]] int read_csv(const string &path,
                              unordered_map<uint32_t, float> *out) {
    return read_csv(path, [out](vector<string> vec) {
        out->insert({boost::lexical_cast<uint32_t>(vec[0]),
                     boost::lexical_cast<float>(vec[1])});
    });
}
