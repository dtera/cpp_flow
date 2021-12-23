FROM alpine
MAINTAINER dterazhao (https://github.com/dtera)

ADD . /service/
WORKDIR /service/ot_server

RUN rm -rf ../cmake-build-debug ../build ../CMakeLists.txt ../main.cpp ../tests.h ../README.md cmake-build-debug build

RUN apk update && apk upgrade
RUN apk add --no-cache git
RUN apk add --no-cache g++
RUN apk add --no-cache make
RUN apk add --no-cache cmake
RUN apk add --no-cache openssl && apk add --no-cache openssl-dev && rm -rf /var/cache/apk/*

RUN sh -c ./install-oatpp-modules.sh

WORKDIR /service/ot_server/build

RUN cmake ..
RUN make

EXPOSE 8000 8000

ENTRYPOINT ["./ot_server-exe"]