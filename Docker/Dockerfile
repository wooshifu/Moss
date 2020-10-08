FROM ubuntu:20.04

COPY sources.list /etc/apt/sources.list
COPY sshd_config /etc/ssh/sshd_config

WORKDIR /root/Moss

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install --fix-missing -y build-essential binutils qemu-system-arm qemu-system-aarch64\
       gdb-multiarch gcc-arm-none-eabi gcc-aarch64-linux-gnu g++-aarch64-linux-gnu make cmake gcc \
       clang-format-10 ssh rsync cmake-curses-gui less vim \
    && update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-10 100 \
    && apt-get clean \
    && apt-get autoremove \
    && rm -rf /var/cache/apt/archives \
    && mkdir /run/sshd \
    && yes password | passwd root \
    && echo 'password of root has been changed to "password"' \
    && echo 'set completion-ignore-case On' >> ~/.inputrc

ENV CC=/usr/bin/aarch64-linux-gnu-gcc
ENV CXX=/usr/bin/aarch64-linux-gnu-g++
ENV LD=/usr/bin/aarch64-linux-gnu-ld
ENV AS=/usr/bin/aarch64-linux-gnu-as
ENV AR=/usr/bin/aarch64-linux-gnu-ar

CMD ["/usr/sbin/sshd", "-D"]
