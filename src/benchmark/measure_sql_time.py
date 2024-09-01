import psycopg2
from psycopg2 import sql, extras
import time
import statistics
import random
import string
from datetime import datetime

db_params = {
    'dbname': 'pg_service_template_db_1',
    'user': 'user',
    'password': 'password',
    'host': 'localhost',
    'port': '5432'
}


def generate_random_data():
    yandex_id = random.randint(1, 1_000_000_000)
    token = ''.join(random.choices(string.ascii_letters + string.digits, k=32))
    login = ''.join(random.choices(string.ascii_lowercase, k=10))
    name = ''.join(random.choices(string.ascii_letters, k=10))
    phone = ''.join(random.choices(string.digits, k=12))
    role = 'user'
    created_at = datetime.now()
    return (yandex_id, token, login, name, phone, role, created_at)


def insert_random_records_batch(cursor, n):
    records = [generate_random_data() for _ in range(n)]
    insert_query = """
        INSERT INTO service.users (yandex_id, token, login, name, phone, role, created_at)
        VALUES %s
        ON CONFLICT (yandex_id) DO NOTHING;
    """
    extras.execute_values(cursor, insert_query, records)
    logins = [record[2] for record in records]
    return logins


def measure_query_time(query, n, iterations=100):
    times = []

    try:
        conn = psycopg2.connect(**db_params)
        cursor = conn.cursor()

        cursor.execute("DELETE FROM service.users;")
        conn.commit()

        logins = insert_random_records_batch(cursor, n)
        conn.commit()

        for _ in range(iterations):
            rand_login = logins[random.randint(0, len(logins) - 1)]

            start_time = time.time()
            cursor.execute(query, (rand_login,))
            conn.commit()
            end_time = time.time()

            execution_time = end_time - start_time
            times.append(execution_time)

        cursor.close()
        conn.close()

        med_time = statistics.mean(times)
        return med_time

    except Exception as e:
        print(f"Произошла ошибка: {e}")
        return None

query = """
    select yandex_id, token, login, name, phone, role 
    from service.users 
    where login = %s
"""

times = []
# Замер времени выполнения запроса для 100, 200, ..., 1000 записей
for N in range(0, 100000, 5000):
    median_time = measure_query_time(query, N)
    if median_time is not None:
        times.append(median_time)
        print(f"Медианное время выполнения запроса при {N} записях: {median_time:.6f} секунд")

print(times)
