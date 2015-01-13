#!/bin/sh
export KERNELDIR=`greadlink -f .`
export PARENT_DIR=`greadlink -f ..`
export USE_SEC_FIPS_MODE=true
export CROSS_COMPILE=/usr/tools/arm-linux-eabi/bin/arm-none-linux-gnueabi-
export PATH=/usr/local/bin:$PATH
export ROOTDIR=`greadlink -f ../../system/`

if [ "${1}" != "" ];then
  export KERNELDIR=`greadlink -f ${1}`
fi

if [ ! -f $KERNELDIR/.config ];
then
  make -j4 uav_defconfig
fi

. $KERNELDIR/.config

export ARCH=arm

cd $KERNELDIR/
make HOSTCFLAGS="-I /usr/include" -j4 || exit 1

rm -rf $ROOTDIR/lib/modules/*
mkdir -p $ROOTDIR/lib/modules/

gfind -name '*.ko' -exec gcp -av {} $ROOTDIR/lib/modules/ \;

${CROSS_COMPILE}strip --strip-unneeded $ROOTDIR/lib/modules/*
