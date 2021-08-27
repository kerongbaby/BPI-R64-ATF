#!/bin/bash
LANG=C
export CROSS_COMPILE=/usr/bin/aarch64-linux-gnu-

#DEVICE=emmc/sdmmc
if [[ -e build.conf ]];then
	. build.conf
fi

case $1 in
	"upload")
		scp {bl2_*.img,fip_*.bin} bpi-r64*.gpt header_*.bin $uploaduser@$uploadserver:$uploaddir
	;;
	"rename")
		set -e
		mv ./build/mt7622/release/bl2.img ./bl2_$DEVICE.img
		mv ./build/mt7622/release/fip.bin ./fip_$DEVICE.bin
		set +x
	;;
	"img")
		target=bpi-r64_$DEVICE.img
		if [[ -e bl2_${DEVICE}.img ]] && [[ -e fip_${DEVICE}.bin ]];then
		echo "creating $target"
		dd of=$target if=/dev/zero bs=512 count=6144
		echo "write header (first block)"
		dd of=$target if=header_${DEVICE}.bin
		echo "write GPT"
		dd of=$target if=bpi-r64_headless.gpt bs=512 seek=1
		if [[ $DEVICE == "sdmmc" ]];then
			echo "write BL2"
			dd of=$target if=bl2_${DEVICE}.img bs=512 seek=1024
		fi
		echo "write FIP (BL31+uboot)"
		dd of=$target if=fip_${DEVICE}.bin bs=512 seek=2048
		else
			echo "error: bl2_${DEVICE}.img or fip_${DEVICE}.bin missing,"
			echo "please use './build.sh rename' after build to create these files"
		fi
	;;
	"clean")
		make distclean
	;;
	"fip")
		echo "FIP create for device: $DEVICE"
		if [[ -e $BL33_BIN ]];then
			set -e
			make PLAT=mt7622 BL33=$BL33_BIN BOOT_DEVICE=$DEVICE fip
			set +x
		else
			echo $BL33_BIN " u-boot.bin missing!"
		fi
	;;
	"")
		echo "device: $DEVICE"
		make distclean
		set -e
		make PLAT=mt7622 BOOT_DEVICE=$DEVICE all
		set +x
	;;
esac
