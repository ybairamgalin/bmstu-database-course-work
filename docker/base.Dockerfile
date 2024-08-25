# В этом докерфайле установим нужные бибилотеки для работы приложения

FROM --platform=linux/amd64 ghcr.io/userver-framework/ubuntu-22.04-userver-pg:latest

RUN apt-get update -y && apt-get install -y \
    libcurl4-openssl-dev  \
    libssl-dev uuid-dev  \
    zlib1g-dev  \
    libpulse-dev

WORKDIR /extra
RUN git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp
WORKDIR /extra/build
RUN cmake ../aws-sdk-cpp  \
    -DCMAKE_BUILD_TYPE=Debug  \
    -DCMAKE_PREFIX_PATH=/usr/  \
    -DCMAKE_INSTALL_PREFIX=/usr/  \
    -DBUILD_ONLY="s3"
RUN make -j 12
RUN make install
