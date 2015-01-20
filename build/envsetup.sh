#!/bin/sh
export WORK_PATH=$PWD
# gnueabihf toolchain
GCC_GNU_EABIHF_PATH=$WORK_PATH/build/arm-none-linux-gnueabi/bin
export GNUHF_LIBC_INC=$WORK_PATH/build/arm-none-linux-gnueabi/arm-none-linux-gnueabi/libc/usr/include
export GNUHF_LIBC_PATH=$WORK_PATH/build/arm-none-linux-gnueabi/arm-none-linux-gnueabi/libc/usr/lib
export WORK_OUTPUT_PATH=$WORK_PATH/out
export CROSS_COMPILE=arm-none-linux-gnueabi-
export PATH=$GCC_GNU_EABIHF_PATH:$PATH
export WORK_LIB_PATH=$WORK_PATH/out/lib