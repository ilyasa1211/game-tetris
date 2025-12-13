FROM ubuntu:24.04 AS builder

WORKDIR /app

# Install dependencies
RUN apt-get update && \
  DEBIAN_FRONTEND=noninteractive apt-get install -y \
  python3 cmake unzip ninja-build curl xz-utils

RUN curl -L -o emsdk.zip https://github.com/emscripten-core/emsdk/archive/refs/tags/4.0.21.zip && \
  unzip emsdk.zip && \
  cd emsdk-4.0.21 && \
  ./emsdk install latest && \
  ./emsdk activate latest

# Copy source code
COPY . .

RUN bash <<-EOF
  set -e
  . ./emsdk-4.0.21/emsdk_env.sh
  emcmake cmake -S . -B out/build/web -G Ninja -DPLATFORM=Web
  cmake --build out/build/web
  mkdir -p /app/dist
  mv out/build/web/Tetris.* /app/dist
EOF

FROM nginx:1-alpine AS runner

WORKDIR /usr/share/nginx/html

COPY --from=builder /app/dist .

EXPOSE 80

RUN cat <<EOF > /etc/nginx/conf.d/default.conf
server {
    listen       80;
    server_name  localhost;

    access_log  /var/log/nginx/host.access.log;

    location / {
        root   /usr/share/nginx/html;
        index  Tetris.html;
    }
}
EOF