FROM --platform=linux/amd64 userver_with_aws:latest

RUN pip install wheel

COPY user /home/user/
RUN useradd user
RUN chown -R user /home/user
RUN ln -s /lib/x86_64-linux-gnu/libbson-1.0.so /lib/x86_64-linux-gnu/libbson.so
RUN ln -s /lib/x86_64-linux-gnu/libmongoc-1.0.so /lib/x86_64-linux-gnu/libmongoc.so
