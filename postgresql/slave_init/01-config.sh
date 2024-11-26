#!/bin/bash
set -e

until pg_basebackup -h master -D "$PGDATA" -U postgres -vP -W --wal-method=stream; do
  echo "Waiting for master to connect..."
  sleep 1
done

echo "standby_mode = 'on'" >> "$PGDATA/recovery.conf"
echo "primary_conninfo = 'host=service-postgres port=5432 user=user password=password'" >> "$PGDATA/recovery.conf"
