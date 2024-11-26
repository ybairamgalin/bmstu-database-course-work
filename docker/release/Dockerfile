FROM cr.yandex/crp37i9qfn16d69aaulp/conference-app:latest

COPY ./build/pg_service_template /usr/bin/service
COPY ./build/pg_service_template_unittest /usr/bin/service
COPY ./build/pg_service_template_inttest /usr/bin/service

RUN mkdir -p /etc/configs
COPY configs /etc/configs
