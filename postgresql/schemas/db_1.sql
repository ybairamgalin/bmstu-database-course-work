drop schema if exists service cascade;

create schema service;

create table service.users
(
    yandex_id  bigint primary key,
    token      text        not null unique,
    login      text        not null unique,
    name       text        not null,
    phone      text unique not null,
    role       text        not null default 'user',
    created_at timestamptz not null default now()
);

create table service.events
(
    event_id    uuid        not null primary key,
    name        text        not null,
    description text        not null default '',
    created_at  timestamptz not null default now()
);

create table service.requests
(
    request_id  uuid        not null primary key,
    event_id    uuid        not null references service.events (event_id),
    user_id     bigint      not null references service.users (yandex_id),
    description text        not null,
    status      text        not null default 'new',
    created_at  timestamptz not null default now(),
    updated_at  timestamptz not null default now()
);

create table service.file_meta
(
    uuid       uuid        not null primary key,
    hash       text        not null unique,
    created_at timestamptz not null default now()
);

create table service.request_file
(
    request_id  uuid        not null references service.requests (request_id),
    source_name text        not null,
    file_uuid   uuid        not null references service.file_meta (uuid),
    created_at  timestamptz not null default now(),

    primary key (request_id, file_uuid)
);


create table service.comments
(
    comment_id bigserial   not null primary key,
    request_id uuid        not null references service.requests (request_id),
    author_id  bigint      not null references service.users (yandex_id),
    content    text        not null,
    created_at timestamptz not null default now()
);

create
or replace function service.update_updated_at()
    returns trigger as $$
begin
    new.updated_at
= now();
return new;
end;
$$
language plpgsql;

drop trigger if exists update_updated_at on service.requests;
create trigger update_updated_at
    before update
    on service.requests
    for each row
    execute function service.update_updated_at();

create table service.articles
(
    article_id uuid        not null primary key,
    title      text,
    content    text,
    author_id  bigint      not null references service.users (yandex_id),
    event_id   uuid        not null references service.events (event_id),
    created_at timestamptz not null default now()
);

create type service.attachment_v1 as (
    id uuid,
    filename text
    );
