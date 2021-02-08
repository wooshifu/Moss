FROM ubuntu:20.04

COPY sources.list /etc/apt/sources.list
COPY sshd_config /etc/ssh/sshd_config

WORKDIR /root/Moss

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install --fix-missing -y build-essential binutils qemu-system-aarch64 g++-10-aarch64-linux-gnu \
       gdb-multiarch make cmake clang-format-11 clang-tidy-11 ssh rsync less vim iwyu \
    && update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-11 100 \
    && update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-11 100 \
    && apt-get clean \
    && apt-get autoremove \
    && rm -rf /var/cache/apt/archives \
    && mkdir /run/sshd \
    && yes password | passwd root \
    && echo 'password of root has been changed to "password"' \
    && echo 'set completion-ignore-case On' >> ~/.inputrc

ENV CC=/usr/bin/aarch64-linux-gnu-gcc-10
ENV CXX=/usr/bin/aarch64-linux-gnu-g++-10
ENV LD=/usr/bin/aarch64-linux-gnu-ld

CMD ["/usr/sbin/sshd", "-D"]
