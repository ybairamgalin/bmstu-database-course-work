import warnings

warnings.simplefilter("ignore")

import argparse
import json

import requests


def load_config():
    with open('cli/settings.json') as config_file:
        return json.load(config_file)


def parse_args(config: dict):
    parser = argparse.ArgumentParser(
        prog='Conference cli',
    )
    parser.add_argument(
        'command',
        choices=config.keys(),
    )
    parser.add_argument('-f', '--filename')
    parser.add_argument('--id')

    return parser.parse_args()


def get_caller(method: str):
    if method == 'POST':
        return requests.post
    elif method == 'GET':
        return requests.get
    elif method == 'PUT':
        return requests.put
    raise ValueError(f'Unknown method: {method}')


def get_token():
    with open('cli/token') as token_file:
        return token_file.readline().strip()


def call(config, args):
    command_cfg = config[args.command]

    body = None
    if command_cfg['body_required']:
        with open(args.filename, 'r') as body_file:
            body = ''.join(body_file.readlines())
            print('Request body')
            print(body)

    query_params = None
    if 'id_param_name' in command_cfg:
        query_params = {
            command_cfg['id_param_name']: args.id
        }

    auth_token = get_token()
    response = get_caller(command_cfg['method'])(
        f'http://localhost:8080/{command_cfg["handler"]}',
        params=query_params,
        data=body,
        headers={'Token': auth_token},
    )
    print(f'Got status: {response.status_code}')

    if (
        'omit_response_body' in command_cfg
        and command_cfg['omit_response_body'] is True
    ):
        return

    body = response.json()
    if body:
        print(json.dumps(body, indent=4, ensure_ascii=False))


def main():
    config = load_config()
    args = parse_args(config)
    call(config, args)


if __name__ == '__main__':
    main()
