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
    parser.add_argument('filename')
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
            body = json.load(body_file)
            print('Request body')
            print(json.dumps(body, indent=4, ensure_ascii=False))

    auth_token = get_token()
    response = get_caller(command_cfg['method'])(
        f'http://localhost:8080/{command_cfg["handler"]}',
        data=json.dumps(body),
        headers={'Token': auth_token},
    )
    print(f'Got status: {response.status_code}')

    body = response.json()
    if body:
        print(json.dumps(body, indent=4, ensure_ascii=False))


def main():
    config = load_config()
    args = parse_args(config)
    call(config, args)


if __name__ == '__main__':
    main()
