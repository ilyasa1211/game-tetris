FROM ubuntu:24.04 AS base

FROM base

ARG USERNAME=dev
ARG USER_UID=1000
ARG USER_GID=1000
ARG OLD_USERNAME=ubuntu

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
  sudo curl lsb-release software-properties-common gnupg cmake git \
  # Raylib requirements
  libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev \
  libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev

# Install latest clang (clang 20 at the time of writing)
RUN curl -LO https://apt.llvm.org/llvm.sh && \
  chmod +x llvm.sh && \
  ./llvm.sh all && \
  rm -f llvm.sh

# Add User
RUN groupmod --gid ${USER_GID} --new-name ${USERNAME} ${OLD_USERNAME} && \
  usermod --gid ${USER_GID} --uid ${USER_UID} --shell /bin/bash --login ${USERNAME} ${OLD_USERNAME} && \
  echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME && \
  chmod 0440 /etc/sudoers.d/$USERNAME

USER ${USERNAME}
