FROM alpine
MAINTAINER dterazhao (https://github.com/dtera)

ADD . /service/
WORKDIR /service

RUN rm -rf ot_client cmake-build-debug build .gitignore CMakeLists.txt main.cpp README.md tests.h \
    ot_server/cmake-build-debug ot_server/build ot/BaseOTReceiver.hpp ot/KOutOfNForOTReceiver.hpp

RUN apk update && apk upgrade
RUN apk add --no-cache git
RUN apk add --no-cache g++
RUN apk add --no-cache make
RUN apk add --no-cache cmake
RUN apk add --no-cache hiredis-dev
RUN apk add --no-cache openssl && apk add --no-cache openssl-dev && rm -rf /var/cache/apk/*

RUN sh -c common/shell/install-oatpp-modules.sh

WORKDIR /service/ot_server/build

RUN cmake ..
RUN make

EXPOSE 8000 8000

ENTRYPOINT ["./ot_server-exe"]
