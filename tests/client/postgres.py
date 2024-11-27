import logging
import os

import psycopg2


class PostgresConnection:
    __connection = None
    __credentials = None

    def __init__(self):
        raise RuntimeError('Constructor should not be called')

    @staticmethod
    def open_connection(credentials_dict):
        PostgresConnection.__init_connection(credentials_dict)

    @staticmethod
    def execute_no_return(query, args=()):
        try:
            cursor = PostgresConnection.__connection.cursor()
            cursor.execute(query, args)
            PostgresConnection.__connection.commit()
        except Exception as error:
            logging.error('database exception: %s', error)
            PostgresConnection.__connection.rollback()
            raise error

    @staticmethod
    def execute(query, args=()):
        try:
            cursor = PostgresConnection.__connection.cursor()
            cursor.execute(query, args)
            PostgresConnection.__connection.commit()
            return cursor.fetchall()
        except Exception as error:
            logging.error('database exception: %s', error)
            PostgresConnection.__connection.rollback()
            raise error

    @staticmethod
    def __init_connection(credentials_dict):
        PostgresConnection.__connection = psycopg2.connect(
            **credentials_dict
        )
