//
// Created by zhaohuiqiang on 2021/12/17.
//

#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

#include <vector>
#include <string>
#include <algorithm>

using namespace std;


inline char c_and(const char &a, const char &b) {
    return a & b;
}

inline char c_or(const char &a, const char &b) {
    return a | b;
}

inline char c_xor(const char &a, const char &b) {
    return a ^ b;
}

inline char c_and_not(const char &a, const char &b) {
    return a & (~b);
}

inline char c_or_not(const char &a, const char &b) {
    return a | (~b);
}

inline char c_xor_not(const char &a, const char &b) {
    return a ^ (~b);
}

inline const char &char_self(const char &c) {
    return c;
}

inline char char_255(const char &c) {
    return (char) 255;
}

inline char char_not(const char &c) {
    return ~c;
}

template<typename BIT_BIN_F, typename BIT_UNARY_F1, typename BIT_UNARY_F2>
void str_bit_bin_op(const string &a, const string &b, string &res, BIT_BIN_F &bit_bin_f,
                    const bool &a_tail_reserve = true, const bool &b_tail_reserve = true,
                    const BIT_UNARY_F1 &a_tail_f = &char_self, const BIT_UNARY_F2 &b_tail_f = &char_self) {
    int m = max(a.size(), b.size());
    int n = min(a.size(), b.size());
    string t;
    for (int i = 0; i < n; i++) {
        char c = bit_bin_f(a[i], b[i]);
        if (c == (char) 0) {
            t += c;
        } else {
            res += t + c;
            t = "";
        }
    }
    if (m != n) {
        res += t;
    }

    if (m == a.size()) {
        if (a_tail_reserve) {
            for (int i = n; i < m; i++) {
                res += a_tail_f(a[i]);
            }
        }
    } else {
        if (b_tail_reserve) {
            for (int i = n; i < m; i++) {
                res += b_tail_f(b[i]);
            }
        }
    }

}

template<typename BIT_BIN_F>
void str_bit_bin_op(const string &a, const string &b, string &res, BIT_BIN_F &bit_bin_f,
                    const bool &a_tail_reserve = true, const bool &b_tail_reserve = true) {
    str_bit_bin_op(a, b, res, bit_bin_f, a_tail_reserve, b_tail_reserve, char_self, char_self);
}

string str_not(const string &a) {
    string res;
    string t;
    for (char i: a) {
        char c = ~i;
        if (c == (char) 0) {
            t += c;
        } else {
            res += t + c;
            t = "";
        }
    }
    return res;
}

string str_and(const string &a, const string &b) {
    string res;
    str_bit_bin_op(a, b, res, c_and, false, false);
    return res;
}

string str_or(const string &a, const string &b) {
    string res;
    str_bit_bin_op(a, b, res, c_or);
    return res;
}

string str_xor(const string &a, const string &b) {
    string res;
    str_bit_bin_op(a, b, res, c_xor);
    return res;
}

string str_and_not(const string &a, const string &b) {
    string res;
    str_bit_bin_op(a, b, res, c_and_not, true, false);
    return res;
}

string str_or_not(const string &a, const string &b) {
    string res;
    str_bit_bin_op(a, b, res, c_or_not, true, true, char_255, char_self);
    return res;
}

string str_xor_not(const string &a, const string &b) {
    string res;
    str_bit_bin_op(a, b, res, c_xor_not, true, true, char_not, char_self);
    return res;
}

string str_co_and(const vector<string> &vs, const vector<int> *cs = nullptr, const bool &is_not = false) {
    int j = 0, len = vs.size();
    if (cs != nullptr) {
        j = (*cs)[0];
        len = (*cs).size();
    }
    string res = is_not ? str_not(vs[j]) : vs[j];
    if (is_not) {
        if (cs == nullptr) {
            for (int i = 1; i < len; i++) {
                res = str_and_not(res, vs[i]);
            }
        } else {
            for (int i = 1; i < len; i++) {
                res = str_and_not(res, vs[(*cs)[i]]);
            }
        }
    } else {
        if (cs == nullptr) {
            for (int i = 1; i < len; i++) {
                res = str_and(res, vs[i]);
            }
        } else {
            for (int i = 1; i < len; i++) {
                res = str_and(res, vs[(*cs)[i]]);
            }
        }
    }
    return res;
}

string str_co_or(const vector<string> &vs, const vector<int> *cs = nullptr, const bool &is_not = false) {
    int j = 0, len = vs.size();
    if (cs != nullptr) {
        j = (*cs)[0];
        len = (*cs).size();
    }
    string res = is_not ? str_not(vs[j]) : vs[j];
    if (is_not) {
        if (cs == nullptr) {
            for (int i = 1; i < len; i++) {
                res = str_or_not(res, vs[i]);
            }
        } else {
            for (int i = 1; i < len; i++) {
                res = str_or_not(res, vs[(*cs)[i]]);
            }
        }
    } else {
        if (cs == nullptr) {
            for (int i = 1; i < len; i++) {
                res = str_or(res, vs[i]);
            }
        } else {
            for (int i = 1; i < len; i++) {
                res = str_or(res, vs[(*cs)[i]]);
            }
        }
    }
    return res;
}

string str_co_xor(const vector<string> &vs, const vector<int> *cs = nullptr, const bool &is_not = false) {
    int j = 0, len = vs.size();
    if (cs != nullptr) {
        j = (*cs)[0];
        len = (*cs).size();
    }
    string res = is_not ? str_not(vs[j]) : vs[j];
    if (is_not) {
        if (cs == nullptr) {
            for (int i = 1; i < len; i++) {
                res = str_xor_not(res, vs[i]);
            }
        } else {
            for (int i = 1; i < len; i++) {
                res = str_xor_not(res, vs[(*cs)[i]]);
            }
        }
    } else {
        if (cs == nullptr) {
            for (int i = 1; i < len; i++) {
                res = str_xor(res, vs[i]);
            }
        } else {
            for (int i = 1; i < len; i++) {
                res = str_xor(res, vs[(*cs)[i]]);
            }
        }
    }
    return res;
}
