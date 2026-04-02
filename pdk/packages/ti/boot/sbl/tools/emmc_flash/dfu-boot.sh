#!/bin/bash

# No need to change anything below this line
UMS_part1=/dev/disk/by-id/usb-Linux_UMS_disk*part1
UMS_part2=/dev/disk/by-id/usb-Linux_UMS_disk*part2
SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname $SCRIPT`
user=`logname`

usage()
{
	echo
	echo "dfu-boot.sh => Utility script to mount eMMC to PC"
	echo "Usage:"
	echo "  sudo ./dfu-boot.sh --PLATFORM --mount DEV"
	echo "  PLATFORM: j721e-evm, j7200-evm"
	echo "  DEV: specify the device to mount => 0 for eMMC"
}

read_config() {
configfile=$HOME/ti/jacinto/workarea/pdk/packages/ti/boot/sbl/tools/emmc_flash/k3bootswitch.conf
section=$1
param=$2
	python3 -c "
import configparser;
import sys;
config = configparser.ConfigParser();
config.read('$configfile');
print (config['$section'].get('$param',''));
"
}

init() {
board=$1
	prebuilt=$SCRIPTPATH/bin/$board
	boot_select=$SCRIPTPATH/boot_select/$board
	uart_dev=`read_config $board uart_dev`
	switch=`read_config $board switch`

}

toggle_power()
{
	echo -n "Reboot manually and press enter.. "
	read DUMMY
}

# Bootloader takes time to initialize
# wait till PC detects a dfu device
wait_till_ready() {
msg=$1
	for i in `seq 30`; do
		dfu-util -l | grep "Found DFU" >/dev/null 2>&1
		if [ $? -eq "0" ]; then
			>&2 echo "    >>>> dfu ready $msg after $i tries"
			return
		fi
		sleep 0.2
	done
	>&2 echo "    >>>> ERROR: Timeout waiting for dfu"
	>&2 echo "    >>>>        Make sure to connect USB cable from EVM to host machine"
	>&2 echo "    >>>>        Refer to readme for correct switch settings for DFU bootmode:"
	exit 1
}

# Use dfu to send prebuilt binaries till you get to the
# Cortex-A u-boot prompt
boot_till_uboot() {
	wait_till_ready "for tiboot3.bin"
	2>&1 dfu-util -R -a bootloader -D $prebuilt/tiboot3.bin
	wait_till_ready "for sysfw.itb"
	2>&1 dfu-util -R -a sysfw.itb -D $prebuilt/sysfw.itb
	wait_till_ready "for tispl.bin"
	2>&1 dfu-util -R -a tispl.bin -D $prebuilt/tispl.bin
	wait_till_ready "for u-boot.img"
	2>&1 dfu-util -R -a u-boot.img -D $prebuilt/u-boot.img
}

# Detect and mount the partitions
try_mount() {
uart_dev=$1
mdev=$2
	for i in `seq 1 100`; do
		echo "ums 0 mmc $mdev" > $uart_dev
		sleep 0.1
		if [ -b $UMS_part1 ] && [ -b $UMS_part2 ]; then
			mkdir -p /media/$user/UMS-boot
			mkdir -p /media/$user/UMS-rootfs
			mount $UMS_part1 /media/$user/UMS-boot -o uid=`id | cut -d'(' -f1  | cut -d'=' -f2`,gid=`id | cut -d'(' -f2  | cut -d'=' -f2`
			mount $UMS_part2 /media/$user/UMS-rootfs
			echo "    >>>> Mounted partions at /media/$user/UMS-boot and /media/$user/UMS-rootfs"
			return
		fi
	done
	>&2 echo "    >>>> ERROR: Could not find partitions $UMS_part1"
	exit 1
}

unmount_all_partitions ()
{
  echo "unmounting boot partition.."
  umount $UMS_part1
  echo "unmounted boot partition"
  echo "unmounting rootfs partition.."
  umount $UMS_part2
  echo "unmounted rootfs partition"
}


# Main script starts from here
if [ `whoami` != "root" ]; then
	echo "This script should be called with sudo!!"
	usage
	exit 1
fi

while [[ $# -gt 0 ]]
do
case $1 in
	--j7|--j7es|--j721e|--j721e-evm)
		init "j721e-evm"
		shift
		;;
	--vcl|--j7vcl|--j7200|--j7200-evm)
		init "j7200-evm"
		shift
		;;
	-m|--mount)
		mdev=$2
		if [ $mdev != "0" ]; then 
			usage
			exit 1
		fi
		shift
		shift
		;;
	-h|--help)
		usage
		exit 0
		;;
	*)
		echo "Invalid argument $1"
		usage
		exit 1
		;;
esac
done

if [ ! -z $mdev ]; then
	# Reboot the board and mount the specified device
	toggle_power
	boot_till_uboot >/dev/null
	try_mount $uart_dev $mdev
	unmount_all_partitions
else
	echo "Invalid usage!!"
	usage
fi
