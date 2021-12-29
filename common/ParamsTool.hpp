//
// Created by zhaohuiqiang on 2021/12/28.
//
#pragma once
#pragma ide diagnostic ignored "UnusedParameter"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <map>
#include <string>

using namespace std;

class ParamsTool {
private:
    static map<string, string> data;
public:
    static string get(const string &key, const string &defaultValue = "") {
        try {
            return data.at(key);
        } catch (exception &e) {
            return defaultValue;
        }
    }

    static bool getBool(const string &key, const bool &defaultValue = false) {
        try {
            auto v = data.at(key);
            if (boost::iequals(v, "true")) {
                return true;
            }
            if (boost::iequals(v, "false")) {
                return false;
            }
            return boost::lexical_cast<bool>(v);
        } catch (exception &e) {
            return defaultValue;
        }
    }

    static short getShort(const string &key, const short &defaultValue = 0) {
        try {
            return boost::lexical_cast<short>(data.at(key));
        } catch (exception &e) {
            return defaultValue;
        }
    }

    static int getInt(const string &key, const int &defaultValue = 0) {
        try {
            return boost::lexical_cast<int>(data.at(key));
        } catch (exception &e) {
            return defaultValue;
        }
    }

    static long getLong(const string &key, const long &defaultValue = 0) {
        try {
            return boost::lexical_cast<long>(data.at(key));
        } catch (exception &e) {
            return defaultValue;
        }
    }

    static float getFloat(const string &key, const float &defaultValue = 0) {
        try {
            return boost::lexical_cast<float>(data.at(key));
        } catch (exception &e) {
            return defaultValue;
        }
    }

    static double getDouble(const string &key, const double &defaultValue = 0) {
        try {
            return boost::lexical_cast<double>(data.at(key));
        } catch (exception &e) {
            return defaultValue;
        }
    }

    static void fromArgs(int argc, const char *argv[]) {
        if (argc > 0) {
            if (boost::contains(argv[0], "=")) {
                fromArgsWithSep(argc, argv, "=");
            } else if (boost::contains(argv[0], ":")) {
                fromArgsWithSep(argc, argv, ":");
            } else {
                fromArgsWithPrefix(argc, argv);
            }
        }
    }

    static void fromArgsWithPrefix(int argc, const char *const *argv) {
        int i = 0;
        string key = argv[i];
        while (i < argc) {
            if (boost::starts_with(argv[i], "--")) {
                key = key.substr(2, key.length() - 2);
            } else if (boost::starts_with(argv[i], "-")) {
                key = key.substr(1, key.length() - 1);
            } else {
                throw runtime_error("Error parsing args on " + key + ". Please prefix keys with -- or -.");
            }
            i++;
            if (i >= argc || boost::starts_with(argv[i], "--") || boost::starts_with(argv[i], "-")) {
                data.insert({key, ""});
            } else {
                data.insert({key, argv[i]});
                i++;
            }
        }
    }

    static void fromArgsWithSep(int argc, const char *argv[], const string &sep) {
        for (int i = 0; i < argc; ++i) {
            auto arg = argv[i];
            vector<string> kv;
            boost::split(kv, arg, boost::is_any_of(sep));
            boost::trim(kv[0]);
            boost::trim(kv[1]);
            if (boost::starts_with(kv[0], "--")) {
                kv[0] = kv[0].substr(2, kv[0].length() - 2);
            }
            data.insert({kv[0], kv[1]});
        }
    }

};
