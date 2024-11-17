FROM --platform=linux/amd64 db_course_work

RUN apt-get update -y && apt-get install -y \
    tmux \
    unzip

RUN pip install pyright

WORKDIR /
RUN wget https://github.com/neovim/neovim/releases/latest/download/nvim-linux64.tar.gz
RUN tar -C /opt -xzf nvim-linux64.tar.gz

RUN git clone https://github.com/ybairamgalin/neovim.git /root/.config/nvim
ENV PATH="/opt/nvim-linux64/bin:$PATH"
RUN nvim --headless +Lazy! sync +qa
