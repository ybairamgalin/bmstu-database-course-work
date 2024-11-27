import requests
from tests.client import PostgresConnection


def test_requests_post():
    PostgresConnection.execute_no_return(
        """
        insert into service.users (yandex_id, token, login, name, phone) 
        values (1, 'token', 'user_login', 'user_name', 'user_phone')
        """
    )
    response = requests.get(
        'http://localhost:8080/v3/events',
        headers={'X-Token': 'token'}
    )
    assert response.status_code == 200
