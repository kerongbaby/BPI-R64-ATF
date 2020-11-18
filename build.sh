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
		set -x
		mv ./build/mt7622/release/bl2.img ./bl2_$DEVICE.img
		mv ./build/mt7622/release/fip.bin ./fip_$DEVICE.bin
		set +x
	;;
	"clean")
		make distclean
	;;
	"")
		echo "device: $DEVICE"
		if [[ -e u-boot.bin ]];then
			make distclean
			set -x
			make PLAT=mt7622 BL33=u-boot.bin BOOT_DEVICE=$DEVICE all fip
			set +x
		else
			echo "u-boot.bin missing!"
		fi
	;;
esac
