FROM cr.yandex/crp37i9qfn16d69aaulp/conference-app:latest

COPY ./build_debug/pg_service_template /usr/bin/service

RUN mkdir -p /etc/configs
COPY configs /etc/configs
