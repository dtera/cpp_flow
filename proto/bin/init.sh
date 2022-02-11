#!/usr/bin/env bash

# shellcheck disable=SC2046
# shellcheck disable=SC2164
CD=$(cd $(dirname "$0"); pwd)
WD=$(cd $(dirname "$CD"); pwd)
echo "Current Directory: $CD"
echo "Work Directory: $WD"

protoc --cpp_out="$WD"/gen --proto_path "$WD" mmdcotservicehttpbroker.proto