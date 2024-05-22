drop schema if exists service cascade;

create schema service;

create table service.users(
    yandex_id bigint primary key,
    token text not null unique,
    login text unique not null,
    name text not null,
    phone text unique not null,
    created_at timestamptz not null default now()
);

create table service.events(
    event_id bigserial not null primary key,
    name text not null,
    start_date date not null,
    duration_days int not null check (duration_days >= 0)
);

create table service.requests(
    request_id bigserial not null primary key,
    event_id bigint not null references service.events(event_id),
    user_id bigint not null references service.users(yandex_id),
    description text not null,
    status text not null default 'new',
    created_at timestamptz not null default now(),
    updated_at timestamptz not null default now()
);

create table service.permissions(
    slug text not null,
    user_id bigint not null references service.users(yandex_id),
    created_at timestamptz not null default now(),

    primary key (user_id, slug)
);

create table service.file_meta(
    uuid uuid not null primary key,
    source_name text not null,
    hash text not null unique,
    created_at timestamptz not null default now()
);
