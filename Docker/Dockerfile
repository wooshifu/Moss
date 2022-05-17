FROM ubuntu:22.04

# uncomment following line to speedup docker image build
COPY sshd_config /etc/ssh/sshd_config

WORKDIR /root/Moss

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update && \
    apt install -y \
        gnupg \
        wget \
        build-essential \
        binutils \
        qemu-system-aarch64 \
        clang \
        clang-tidy \
        lld \
        llvm \
        gcc-aarch64-linux-gnu \
        g++-aarch64-linux-gnu \
        gdb-multiarch \
        file \
        ninja-build \
        make \
        cmake \
        openssh-server \
        ssh \
        rsync \
        less \
        vim \
        iwyu && \
    apt clean && \
    apt autoremove && \
    rm -rf /var/cache/apt/archives

RUN mkdir /run/sshd && \
    yes password | passwd root && \
    echo 'password of root has been changed to "password"' && \
    echo 'set completion-ignore-case On' >> ~/.inputrc

CMD ["/usr/sbin/sshd", "-D"]
