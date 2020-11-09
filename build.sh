#!/bin/bash
LANG=C
export CROSS_COMPILE=/usr/bin/aarch64-linux-gnu-

#DEVICE=emmc/sdmmc
if [[ -e build.conf ]];then
	. build.conf
fi

case $1 in
	"upload")
		scp ./build/mt7622/release/{bl2.img,fip.bin} bpi-r64.gpt $uploaduser@$uploadserver:$uploaddir
	;;
	"rename")
		mv ./build/mt7622/release/bl2{,_$DEVICE}.img
		mv ./build/mt7622/release/fip{,_$DEVICE}.bin
	;;
	"")
		echo "device: $DEVICE"
		if [[ -e u-boot.bin ]];then
			set -x
			make distclean
			make PLAT=mt7622 BL33=u-boot.bin BOOT_DEVICE=$DEVICE all fip
			set +x
		else
			echo "u-boot.bin missing!"
		fi
	;;
esac
