#!/usr/bin/env bash

# shellcheck disable=SC2046
# shellcheck disable=SC2164
CD=$(cd $(dirname "$0"); pwd)
WD=$(cd $(dirname "$CD"); pwd)
echo "Current Directory: $CD"
echo "Work Directory: $WD"

protoc --cpp_out="$WD"/gen --proto_path "$WD" mmdcotservicehttpbroker.proto
#cd "$WD"/gen/
#rm -f mmdcotservicehttpbroker_pb*
#mv mmdcotservicehttpbroker.pb.cc mmdcotservicehttpbroker_pb.cc
#mv mmdcotservicehttpbroker.pb.h mmdcotservicehttpbroker_pb.h
##brew install gnu-sed
#gsed -i 's/mmdcotservicehttpbroker\.pb\.h/mmdcotservicehttpbroker_pb\.h/g' mmdcotservicehttpbroker_pb.cc
#git add mmdcotservicehttpbroker_pb.cc mmdcotservicehttpbroker_pb.h