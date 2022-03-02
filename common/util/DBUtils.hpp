//
// Created by zhaohuiqiang on 2021/12/24.
//
#pragma once

#include "common/cli/KVCli.h"
#include "common/cli/FKVCli.hpp"
#include "common/cli/RedisCli.hpp"
#include "common/ParamsTool.hpp"

template<typename M>
KVCli<M> *getKVCli(const string &kvType) {
    if (kvType == "redis") {
        return new RedisCli<M>(ParamsTool::get("redisHost", "localhost"),
                               ParamsTool::getInt("redisPort", 6379),
                               ParamsTool::get("redisPassword", ""));
    } else if (kvType == "fkv") {
        return nullptr;
    }
    return nullptr;
}