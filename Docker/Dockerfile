FROM ubuntu:21.04

# uncomment following line to speedup docker image build
#COPY sources.list /etc/apt/sources.list
COPY sshd_config /etc/ssh/sshd_config

WORKDIR /root/Moss

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install --fix-missing -y build-essential \
    binutils \
    qemu-system-aarch64 \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    gdb-multiarch \
    file \
    ninja-build \
    make \
    cmake \
    clang \
    llvm \
    lld \
    clang-format \
    clang-tidy \
    openssh-server \
    ssh \
    rsync \
    less \
    vim \
    iwyu && apt-get clean && apt-get autoremove && rm -rf /var/cache/apt/archives

RUN mkdir /run/sshd
RUN yes password | passwd root
RUN echo 'password of root has been changed to "password"'
RUN echo 'set completion-ignore-case On' >> ~/.inputrc

ENV CC=clang
ENV CXX=clang++
ENV LD=lld

CMD ["/usr/sbin/sshd", "-D"]
