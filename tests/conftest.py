import pytest
from tests.client import PostgresConnection


def pytest_configure(config):
    print('Pytest configure')
    p = {
        "dbname": "pg_service_template_db_1",
        "user": "user",
        "password": "password",
        "host": "service-postgres",
        "port": "5432",
    }
    PostgresConnection.open_connection(p)


@pytest.fixture(autouse=True)
def clean_schema():
    result = PostgresConnection.execute(
        """
        select table_name
        from information_schema.tables
        where table_schema = 'service'
        """,
    )

    for row in result:
        PostgresConnection.execute_no_return(
            f"""
            delete from service.{row[0]}
            """
        )
    yield

    # Optionally, you could reinitialize the schema or perform other cleanup actions here
