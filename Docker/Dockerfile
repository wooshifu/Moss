FROM ubuntu:21.04

COPY sources.list /etc/apt/sources.list
COPY sshd_config /etc/ssh/sshd_config

WORKDIR /root/Moss

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install --fix-missing -y build-essential
RUN apt-get install --fix-missing -y binutils
RUN apt-get install --fix-missing -y qemu-system-aarch64
RUN apt-get install --fix-missing -y gcc-aarch64-linux-gnu
RUN apt-get install --fix-missing -y g++-aarch64-linux-gnu
RUN apt-get install --fix-missing -y gdb-multiarch
RUN apt-get install --fix-missing -y make
RUN apt-get install --fix-missing -y cmake
RUN apt-get install --fix-missing -y clang
RUN apt-get install --fix-missing -y llvm
RUN apt-get install --fix-missing -y lld
RUN apt-get install --fix-missing -y clang-format
RUN apt-get install --fix-missing -y clang-tidy
#RUN apt-get install --fix-missing -y openssh-server
#RUN apt-get install --fix-missing -y ssh
#RUN apt-get install --fix-missing -y rsync
RUN apt-get install --fix-missing -y less
RUN apt-get install --fix-missing -y vim
RUN apt-get install --fix-missing -y iwyu
RUN apt-get clean
RUN apt-get autoremove
RUN rm -rf /var/cache/apt/archives
RUN mkdir /run/sshd
RUN yes password | passwd root
RUN echo 'password of root has been changed to "password"'
RUN echo 'set completion-ignore-case On' >> ~/.inputrc

ENV CC=/usr/bin/aarch64-linux-gnu-gcc
ENV CXX=/usr/bin/aarch64-linux-gnu-g++
ENV LD=/usr/bin/aarch64-linux-gnu-ld

#CMD ["/usr/sbin/sshd", "-D"]
