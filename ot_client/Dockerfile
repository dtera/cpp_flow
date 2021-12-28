FROM alpine:3.13.7
MAINTAINER dterazhao (https://github.com/dtera)

ADD . /service/
WORKDIR /service

RUN rm -rf ot_server cmake-build-debug build .gitignore CMakeLists.txt main.cpp README.md tests.h \
    ot_client/cmake-build-debug ot_client/build ot/BaseOTSender.hpp ot/KOutOfNForOTSender.hpp common/RedisCli.hpp

RUN apk update && apk upgrade
RUN apk add --no-cache git
RUN apk add --no-cache g++
RUN apk add --no-cache make
RUN apk add --no-cache cmake
RUN apk add --no-cache openssl-dev && rm -rf /var/cache/apk/*

RUN sh -c common/shell/install-oatpp-modules.sh

WORKDIR /service/ot_client/build

RUN cmake ..
RUN make

EXPOSE 8001 8001

ENTRYPOINT ["./ot_server-exe"]
