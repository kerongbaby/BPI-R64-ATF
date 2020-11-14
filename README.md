# ATF for Bananapi-R64

currently only emmc is supported!

sdmmc version can be built,but does not boot (only bootrom output) and BRLYT-header in bl2.img conflicts with gpt (0x200)

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

## GPT-Layout in bpi-r64.gpt

the first 1024 (0x400) blocks are reserved for GPT

```
Disk /dev/mmcblk0: 7.3 GiB, 7818182656 bytes, 15269888 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: gpt
Disk identifier: 2BD17853-102B-4500-AA1A-8A21D4D7984D

Device         Start   End Sectors   Size Type
/dev/mmcblk0p1  2048  6143    4096     2M Linux filesystem #fip 0x800 (len 0x1000)
/dev/mmcblk0p2  6144  7167    1024   512K Linux filesystem #config (uboot-env) 0x1800 (len 0x400), unused
/dev/mmcblk0p3  7168  8191    1024   512K Linux filesystem #rf (calibration data), unused
/dev/mmcblk0p4  8192 49151   40960    20M Linux filesystem #kernel = BPI-BOOT (currently not defined as FAT)

/dev/mmcblk0: PTUUID="2bd17853-102b-4500-aa1a-8a21d4d7984d" PTTYPE="gpt"
/dev/mmcblk0p1: PARTLABEL="fip" PARTUUID="18de6587-4f17-4e08-a6c9-d9d3d424f4c5"
/dev/mmcblk0p2: PARTLABEL="config" PARTUUID="19a4763a-6b19-4a4b-a0c4-8cc34f4c2ab9"
/dev/mmcblk0p3: PARTLABEL="rf" PARTUUID="8142c1b2-1697-41d9-b1bf-a88d76c7213f"
/dev/mmcblk0p4: PARTLABEL="kernel" PARTUUID="971f7556-ef1a-44cd-8b28-0cf8100b9c7e"
```

## Install with upstream uboot

needs option CONFIG_SUPPORT_EMMC_BOOT=y in installed uboot (sdcard) for "mmc partconf"!

### eMMC

```sh
#flash bl2
setenv ipaddr 192.168.0.19
setenv serverip 192.168.0.11
setenv loadaddr 0x44000000
mmc partconf 0 1 1 1
if tftp ${loadaddr} ${serverip}:bl2.img;then mmc erase 0x0 0x400;mmc write ${loadaddr} 0x0 0x400;fi
mmc partconf 0 1 1 0
```
```sh
#flash gpt+fip
#mmc partconf 0 1 1 0
setenv loadaddr 0x44000000
if tftp ${loadaddr} ${serverip}:bpi-r64.gpt;then mmc erase 0x0 0x400;mmc write ${loadaddr} 0x0 0x400;fi 
#write fip.bin to first user-partition @ 2048 (0x800), count 4096 (0x1000) sectors
if tftp ${loadaddr} ${serverip}:fip.bin;then mmc erase 0x800 0x1000;mmc write ${loadaddr} 0x800 0x1000;fi 
```

## install from system

(not yet available)
