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

create table service.permissions(
    slug text not null,
    user_id bigint not null references service.users(yandex_id),
    created_at timestamptz not null default now(),

    primary key (user_id, slug)
);

create table service.events(
    event_id bigserial not null primary key,
    name text not null
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

create table service.file_meta(
    uuid uuid not null primary key,
    source_name text not null,
    hash text not null unique,
    created_at timestamptz not null default now()
);

create table service.request_file(
    request_id bigint not null references service.requests(request_id),
    file_uuid uuid not null references service.file_meta(uuid),

    primary key (request_id, file_uuid)
);

create table service.comments(
    comment_id bigserial not null primary key,
    request_id bigint not null references service.requests(request_id),
    author_id bigint not null references service.users(yandex_id),
    content text not null,
    created_at timestamptz not null default now()
);

create or replace function service.update_updated_at()
    returns trigger as $$
begin
    new.updated_at = now();
    return new;
end;
$$ language plpgsql;

drop trigger if exists update_updated_at on service.requests;
create trigger update_updated_at
before update on service.requests
for each row
execute function service.update_updated_at();

create table service.articles(
    article_id bigserial not null primary key,
    description text,
    author_id bigint not null references service.users(yandex_id),
    event_id bigint not null references service.events(event_id),
    created_at timestamptz not null default now()
);

create table service.article_file(
    article_id bigint not null references service.articles(article_id),
    file_uuid uuid not null references service.file_meta(uuid),

    primary key (article_id, file_uuid)
)
