FROM --platform=linux/amd64 userver_with_aws:latest

COPY user /home/user/
RUN useradd user
RUN chown -R user /home/user
