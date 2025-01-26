#!/bin/bash

# Set colours for messaging
MSG_INFO='\033[0;44m'
MSG_ERROR='\033[0;41m'
MSG_SUCCESS='\033[0;42m'
RESULT=$?

# Build introduction blurb
echo -e "${MSG_INFO}Z-SIX Toolchain Build Script"
echo -e "\033[0m"
echo "This script will install the following utilities:"
echo $'build-essential, bison, flex, gmp, mpc, mpfr, texinfo, and isl \n'
echo "The following utilities will be downloaded, built, and installed to ${HOME}/opt/cross:"
echo "GCC  14.2.0"
echo $'Binutils 2.43 \n'
echo "Please make sure you are running on an Debian based system and have allotted"
echo $'at least 2 cores and 4096MB of ram to your Virtual Machine. \n'
echo "Cross compiling a toolchain may be time consuming, do not panic if your system appears"
echo $'to be stuck. Go brew a cup of coffee or tea while you wait. \n'
echo "Building the toolchain will start in 15 seconds. If you've already built it, please"
echo "run export-env.sh instead to prepare your shell session to build."

sleep 15

# Start host package installation
echo -e "${MSG_INFO}Starting to install packages"
echo -e "\033[0m"
sudo apt install --yes build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev
if [ $RESULT == 0 ]; then
  echo -e "${MSG_SUCCESS}Installed Host Packages!"
else
  echo -e "${MSG_ERROR}Command failed, please check output."
  exit 1
fi

# Make directories
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
mkdir $HOME/opt/cross
mkdir $HOME/src

# Download sources
echo -e "${MSG_INFO}Starting to download sources"
echo -e "\033[0m"
cd $HOME/src
wget "https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.xz"
if [ $RESULT == 0 ]; then
  echo -e "${MSG_SUCCESS}Downloaded Target-GCC!"
else
  echo -e "${MSG_ERROR}Command failed, please check output."
  exit 1
fi
wget "https://ftp.gnu.org/gnu/binutils/binutils-2.43.tar.xz"
if [ $RESULT == 0 ]; then
  echo -e "${MSG_SUCCESS}Downloaded Target-Binutils!"
else
  echo -e "${MSG_ERROR}Command failed, please check output."
  exit 1
fi
mkdir build-binutils
mkdir build-gcc
tar xvf binutils-2.43.tar.xz
tar xvf gcc-14.2.0.tar.xz

# Build Binutils
cd build-binutils
../binutils-2.43/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
if [ $RESULT == 0 ]; then
  echo -e "${MSG_SUCCESS}Built Binutils!"
else
  echo -e "${MSG_ERROR}Command failed, please check output."
  exit 1
fi
make install

# Build GCC
cd $HOME/src
which -- $TARGET-as || echo "${MSG_ERROR}}$TARGET-as is not in the PATH." && exit 1
cd build-gcc
../gcc-14.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx
make -j 2 all-gcc
if [ $RESULT == 0 ]; then
  echo -e "${MSG_SUCCESS}Built GCC!"
else
  echo -e "${MSG_ERROR}Command failed, please check output."
  exit 1
fi
make -j 2 all-target-libgcc
if [ $RESULT == 0 ]; then
  echo -e "${MSG_SUCCESS}Built libGCC!"
else
  echo -e "${MSG_ERROR}Command failed, please check output."
  exit 1
fi
make -j 2 all-target-libstdc++-v3
if [ $RESULT == 0 ]; then
  echo -e "${MSG_SUCCESS}Built libstdc++!"
else
  echo -e "${MSG_ERROR}Command failed, please check output."
  exit 1
fi
make install-gcc
make install-target-libgcc
make install-target-libstdc++-v3

# Test GCC
$HOME/opt/cross/bin/$TARGET-gcc --version
if [ $RESULT == 0 ]; then
  echo -e "${MSG_SUCCESS}Toolchain Installed Successfully!"
else
  echo -e "${MSG_ERROR}Command failed, please check output."
  exit 1
fi
export PATH="$HOME/opt/cross/bin:$PATH"