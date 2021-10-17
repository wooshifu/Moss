FROM ubuntu:20.04

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

ARG LLVM_VERSION=-13
COPY sources.list /etc/apt/sources.list
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -  && \
    apt update && \
    apt install -y \
        clang${LLVM_VERSION} \
        llvm${LLVM_VERSION} \
        lld${LLVM_VERSION} \
        clang-tidy${LLVM_VERSION} \
        clang-format${LLVM_VERSION} && \
    apt clean && \
    apt autoremove && \
    rm -rf /var/cache/apt/archives

ENV PATH=/usr/lib/llvm-13/bin:$PATH

RUN mkdir /run/sshd
RUN yes password | passwd root
RUN echo 'password of root has been changed to "password"'
RUN echo 'set completion-ignore-case On' >> ~/.inputrc

ENV CC=clang
ENV CXX=clang++
ENV LD=lld

CMD ["/usr/sbin/sshd", "-D"]
