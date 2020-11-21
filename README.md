# ATF for Bananapi-R64

<img src="bpi-r64-storage.svg">

## requirements

On a x86/x64-host you need cross compile tools for the arm64 architecture

(package list maybe incomplete)

```sh
sudo apt install gcc-aarch64-linux-gnu make gcc
```

## build

- change build.conf to match desired device (emmc/sd) and upload-settings
- copy 64bit uboot.bin to this source directory
- run ./build.sh

built binaries:

```
./build/mt7622/release/bl31.bin
./build/mt7622/release/bl2.bin
./build/mt7622/release/bl2.img
./build/mt7622/release/fip.bin
```
- run ./build.sh upload

## GPT-Layout

new GPT with BPI-BOOT/BPI-ROOT (headless)
```
Disk /dev/mmcblk0: 7.3 GiB, 7818182656 bytes, 15269888 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: gpt
Disk identifier: 2BD17853-102B-4500-AA1A-8A21D4D7984D

Device         Start   End Sectors   Size Type
/dev/sdb1    2048     6143     4096    2M Linux filesystem #fip 0x800 (len 0x1000)
/dev/sdb2    6144     7167     1024  512K Linux filesystem #config (uboot-env) 0x1800 (len 0x400), unused
/dev/sdb3    7168     8191     1024  512K Linux filesystem #rf (calibration data), unused
/dev/sdb4    8192   212991   204800  100M Microsoft basic data #BPI_BOOT for kernel/dtb
/dev/sdb5  212992 15947775 15734784  7,5G Linux filesystem #BPI-ROOT for rootfs

/dev/sdb: PTUUID="2bd17853-102b-4500-aa1a-8a21d4d7984d" PTTYPE="gpt"
/dev/sdb1: PARTLABEL="fip" PARTUUID="18de6587-4f17-4e08-a6c9-d9d3d424f4c5"
/dev/sdb2: PARTLABEL="config" PARTUUID="19a4763a-6b19-4a4b-a0c4-8cc34f4c2ab9"
/dev/sdb3: PARTLABEL="rf" PARTUUID="8142c1b2-1697-41d9-b1bf-a88d76c7213f"
/dev/sdb4: LABEL_FATBOOT="BPI_BOOT" LABEL="BPI_BOOT" UUID="D834-B456" TYPE="vfat" PARTLABEL="kernel" PARTUUID="94db3945-4694-4c68-8304-c94dd92099f8"
/dev/sdb5: LABEL="BPI-ROOT" UUID="f579f4e1-2593-4e72-887c-e75059f0b7f2" TYPE="ext4" PARTLABEL="root" PARTUUID="c9223fca-b686-4ec9-8bdc-6bdb85f0d7ae"

```
## Install

### EMMC with upstream uboot

needs option CONFIG_SUPPORT_EMMC_BOOT=y in installed uboot (sdcard) for "mmc partconf"!

```sh
#flash bl2
setenv ipaddr 192.168.0.19
setenv serverip 192.168.0.11
setenv loadaddr 0x44000000
mmc partconf 0 1 1 1
if tftp ${loadaddr} ${serverip}:bl2_emmc.img;then mmc erase 0x0 0x400;mmc write ${loadaddr} 0x0 0x400;fi
mmc partconf 0 1 1 0
```
```sh
#mmc partconf 0 1 1 0
setenv loadaddr 0x44000000
setenv serverip 192.168.0.10
#write emmc_header
if tftp ${loadaddr} ${serverip}:emmc_header.bin ;then mmc erase 0x0 0x400;mmc write ${loadaddr} 0x0 0x1;fi
#write gpt
if tftp ${loadaddr} ${serverip}:bpi-r64_headless.gpt;then mmc write ${loadaddr} 0x1 0x3FF; fi
#write fip
if tftp ${loadaddr} ${serverip}:fip_emmc.bin;then mmc erase 0x800 0x1000;mmc write ${loadaddr} 0x800 0x1000;fi
```

### SD via dd
```sh
target=/dev/sdb
sudo dd if=sd_header.bin of=$target bs=512 seek=0
sudo dd if=bpi-r64_headless.gpt of=$target bs=512 seek=1
sudo dd if=./bl2_sdmmc.img of=$target bs=512 seek=1024
sudo dd if=./fip_sdmmc.bin of=$target bs=512 seek=2048
```

## strip own gpt

without first 512 bytes (gpt only, like bpi-r64_headless.bin):

```sh
npart=4;sudo dd if=/dev/sdb of=bpi-r64_headless.gpt bs=1 skip=512 count=$(( $npart*128 +1024 ))
```
change npart to number of partitions in your GPT

## booting linux kernel

### booting fit-image

my kernel-repo creates for r64 (arch=arm64) also an FIT-Image (*.itb) which needs to be uploaded to tftp

```sh
setenv serverip 192.168.0.10
setenv loadaddr 0x44000000
tftp ${loadaddr} bpi-r64-5.4.77-main.itb
bootm ${loadaddr}
```

as there is no rootfs yet you need a initramfs for repair the GPT (fdisk reports "warning: GPT array CRC is invalid" i guess because of backup GPT missing in last block of emmc)
tried to fix with fdisk,gdisk and parted, no luck...only SIGSEV. seems like gpt contains wrong disk-size and backup-header is missing
