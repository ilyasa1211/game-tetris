FROM ubuntu:24.04 AS base

FROM base

ARG USERNAME=dev
ARG USER_UID=1001
ARG USER_GID=1001

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
  sudo curl lsb-release software-properties-common gnupg cmake git \
  # Raylib requirements
  libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev \
  libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev

# Install latest clang (clang 20)
RUN curl -L -o - https://apt.llvm.org/llvm.sh | bash

# Add User
RUN groupadd --gid ${USER_GID} ${USERNAME} && \
  useradd --gid ${USER_GID} --uid ${USER_UID} --shell /bin/bash --create-home ${USERNAME} && \
  echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME && \
  chmod 0440 /etc/sudoers.d/$USERNAME

USER ${USERNAME}
