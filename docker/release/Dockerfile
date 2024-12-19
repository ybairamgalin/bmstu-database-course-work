FROM cr.yandex/crp37i9qfn16d69aaulp/conference-app:latest

RUN mkdir -p /usr/bin/service
WORKDIR /usr/bin/service

COPY ./build_debug/pg_service_template /usr/bin/service/
COPY ./build_debug/pg_service_template_unittest /usr/bin/service/
COPY ./build_debug/pg_service_template_inttest /usr/bin/service/
COPY ./tests /usr/bin/service/tests

RUN mkdir -p /etc/configs
COPY configs /etc/configs
