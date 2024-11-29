import uuid

import requests
from tests.client import PostgresConnection


def execute_test():
    PostgresConnection.execute_no_return(
        """
        insert into service.users (yandex_id, token, login, name, phone) 
        values (1, 'token', 'user_login', 'user_name', 'user_phone')
        """
    )
    event_name = 'name'
    description = 'description'
    id = uuid.uuid4()
    PostgresConnection.execute_no_return(
        f"""
        insert into service.events (event_id, name, description)
        values ('{id}', '{event_name}', '{description}')
        """
    )

    response = requests.get(
        'http://localhost:8080/v3/events',
        headers={'X-Token': 'token'}
    )
    assert response.status_code == 200

    assert response.json() == {
        'events': [
            {
                'name': event_name,
                'description': description,
            }
        ]
    }


def test_requests_post_1():
    execute_test()


def test_requests_post_2():
    execute_test()


def test_requests_post_3():
    execute_test()


def test_requests_post_4():
    execute_test()


def test_requests_post_5():
    execute_test()
