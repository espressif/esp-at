#!/bin/sh
export PKG_CONFIG_PATH=/usr/lib/pkgconfig

if [ -f ~/getAmbarellaSDK.py ];then

  echo "Already configured"

  else

  # Install Development Tools
  dpkg --add-architecture i386
  sudo apt-get update
  sudo apt-get install -y \
  git \
  wget \
  make \
  libncurses-dev \
  flex \
  bison \
  gperf \
  python \
  python-serial
#    autoconf              \
#    bc                    \
#    cmake                 \
#    coreutils             \
#    cramfsprogs           \
#    fakeroot              \
#    g++                   \
#    gawk                  \
#    genext2fs             \
#    git                   \
#    lib32bz2-1.0          \
#    lib32ncurses5         \
#    lib32z1               \
#    libcurl4-openssl-dev  \
#    libfakeroot           \
#    libgtk2.0-0           \
#    libSM6                \
#    libssl-dev            \
#    libtool               \
#    libxml2-dev           \
#    mtd-utils             \
#    p7zip-full            \
#    patch                 \
#    vim                   \
#    xutils-dev 



  
      sudo mkdir -p /opt/esp
      sudo chmod 777 /opt/esp
      cd /opt/esp
      wget https://artifactory.tools.simplisafe.com/artifactory/misc/toolchain/xtensa-esp32-elf-linux64-1.22.0-61-gab8375a-5.2.0.tar.gz -nv
      tar -xzf /opt/esp/xtensa-esp32-elf-linux64-1.22.0-61-gab8375a-5.2.0.tar.gz

      export PATH=$PATH:$/opt/esp/xtensa-esp32-elf/bin

      export GO_MAJOR="$GO_MAJOR"
      export GO_MINOR="$GO_MINOR"
      export GO_PATCH="$GO_PATCH"
      export GO_PIPELINE_COUNTER="$GO_PIPELINE_COUNTER"

      echo "Version = $GO_MAJOR.$GO_MINOR.$GO_PATCH.$GO_PIPELINE_COUNTER"
fi
