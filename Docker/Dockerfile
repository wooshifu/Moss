FROM ubuntu:20.04

COPY sources.list /etc/apt/sources.list
COPY sshd_config /etc/ssh/sshd_config

WORKDIR /root/Moss

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install --fix-missing -y build-essential binutils qemu-system-aarch64 \
       gdb-multiarch make cmake clang-11 llvm-11 lld-11 clang-format-11 ssh rsync less vim \
    && update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-11 100 \
    && update-alternatives --install /usr/bin/clang clang /usr/bin/clang-11 100 \
    && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-11 100 \
    && update-alternatives --install /usr/bin/lld lld /usr/bin/lld-11 100 \
    && update-alternatives --install /usr/bin/llvm-objcopy llvm-objcopy-11 /usr/bin/llvm-objcopy-11 100 \
    && update-alternatives --install /usr/bin/llvm-objdump llvm-objdump-11 /usr/bin/llvm-objdump-11 100 \
    && update-alternatives --install /usr/bin/llvm-readelf llvm-readelf-11 /usr/bin/llvm-readelf-11 100 \
    && update-alternatives --install /usr/bin/llvm-nm llvm-nm-11 /usr/bin/llvm-nm-11 100 \
    && update-alternatives --install /usr/bin/llvm-strings llvm-strings-11 /usr/bin/llvm-strings-11 100 \
    && update-alternatives --install /usr/bin/llvm-strip llvm-strip-11 /usr/bin/llvm-strip-11 100 \
    && update-alternatives --install /usr/bin/llvm-size llvm-size-11 /usr/bin/llvm-size-11 100 \
    && update-alternatives --install /usr/bin/llvm-ranlib llvm-ranlib-11 /usr/bin/llvm-ranlib-11 100 \
    && update-alternatives --install /usr/bin/llvm-ar llvm-ar-11 /usr/bin/llvm-ar-11 100 \
    && update-alternatives --install /usr/bin/llvm-as llvm-as-11 /usr/bin/llvm-as-11 100 \
    && update-alternatives --install /usr/bin/llvm-cat llvm-cat-11 /usr/bin/llvm-cat-11 100 \
    && update-alternatives --install /usr/bin/llvm-cxxfilt llvm-cxxfilt-11 /usr/bin/llvm-cxxfilt-11 100 \
    && update-alternatives --install /usr/bin/llvm-link llvm-link-11 /usr/bin/llvm-link-11 100 \
    && apt-get clean \
    && apt-get autoremove \
    && rm -rf /var/cache/apt/archives \
    && mkdir /run/sshd \
    && yes password | passwd root \
    && echo 'password of root has been changed to "password"' \
    && echo 'set completion-ignore-case On' >> ~/.inputrc

ENV CC=/usr/bin/clang
ENV CXX=/usr/bin/clang++
ENV LD=/usr/bin/lld

CMD ["/usr/sbin/sshd", "-D"]
