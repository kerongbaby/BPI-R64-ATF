#!/bin/bash
set -e
./build.sh clean
./build.sh
./build.sh rename
./build.sh img
cp bpi-r64_sdmmc.img /mnt/d
