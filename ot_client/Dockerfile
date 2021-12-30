FROM alpine:3.13.7
MAINTAINER dterazhao (https://github.com/dtera)

ADD . /service/

RUN apk update && apk upgrade
RUN apk add --no-cache git
RUN apk add --no-cache g++
RUN apk add --no-cache make
RUN apk add --no-cache cmake
RUN apk add --no-cache openssl-dev && rm -rf /var/cache/apk/*

WORKDIR /service
RUN sh -c common/shell/install-oatpp-modules.sh

WORKDIR /service/ot_client/build
RUN cp -R ../conf ./
RUN cmake ..
RUN make

WORKDIR /service
RUN find * -maxdepth 0 | grep -v ot_client | xargs rm -rf

WORKDIR /service/ot_client
RUN find * -maxdepth 0 | grep -v build | xargs rm -rf

WORKDIR /service/ot_client/build
VOLUME /service/ot_client/build/conf

EXPOSE 8001 8001

ENTRYPOINT ["./ot_client-exe"]
