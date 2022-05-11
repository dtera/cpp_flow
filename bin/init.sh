#!/bin/sh

cd ..
mkdir build
# shellcheck disable=SC2164
cd build
cmake -DOPENSSL_ROOT_DIR=/opt/homebrew/opt/openssl -DOPENSSL_LIBRARIES=/opt/homebrew/opt/openssl/lib ..