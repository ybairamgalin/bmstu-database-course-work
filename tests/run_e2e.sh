#!/bin/bash

set -e

/usr/bin/service/pg_service_template \
  --config /etc/configs/static_config.yaml \
  --config_vars /etc/configs/config_vars.docker.yaml &

sleep 20

pytest -vv --junit-xml=/report/report_e2e.xml --junit-prefix=e2e /usr/bin/service/tests
