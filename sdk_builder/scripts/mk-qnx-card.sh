#! /bin/sh
# Script to create SD card

execute ()
{
    $* >/dev/null
    if [ $? -ne 0 ]; then
        echo
        echo "ERROR: executing $*"
        echo
        exit 1
    fi
}


usage ()
{
  echo "
Usage: `basename $1` <options>

Mandatory options:
  --device              SD block device node (e.g /dev/sdd)
"
  exit 1
}

check_if_main_drive ()
{
  mount | grep " on / type " > /dev/null
  if [ "$?" != "0" ]
  then
    echo "-- WARNING: not able to determine current filesystem device"
  else
    main_dev=`mount | grep " on / type " | awk '{print $1}'`
    echo "-- Main device is: $main_dev"
    echo $main_dev | grep "$device" > /dev/null
    [ "$?" = "0" ] && echo "++ ERROR: $device seems to be current main drive ++" && exit 1
  fi

}

check_if_big_size ()
{
  partname=`basename $device`
  size=`cat /proc/partitions | grep $partname | head -1 | awk '{print $3}'`
  if [ $size -gt 17000000 ]; then
    cat << EOM

************************* WARNING ***********************************
*                                                                   *
*      Selected Device is greater then 16GB                         *
*      Continuing past this point will erase data from device       *
*      Double check that this is the correct SD Card                *
*                                                                   *
*********************************************************************
EOM

    ENTERCORRECTLY=0
    while [ $ENTERCORRECTLY -ne 1 ]
    do
      read -p 'Would you like to continue [y/n] : ' SIZECHECK
      echo ""
      echo " "
      ENTERCORRECTLY=1
      case $SIZECHECK in
        "y")  ;;
        "n")  exit;;
        *)  echo "Please enter y or n";ENTERCORRECTLY=0;;
      esac
      echo ""
    done
  fi
}

unmount_all_partitions ()
{
  for i in `ls -1 $device*`; do
    echo "unmounting device '$i'"
    umount $i 2>/dev/null
  done
  mount | grep $device
}

# Check if the script was started as root or with sudo
user=`id -u`
[ "$user" != "0" ] && echo "++ Must be root/sudo ++" && exit

# Process command line...
while [ $# -gt 0 ]; do
  case $1 in
    --device) shift; device=$1; shift; ;;
    *) copy="$copy $1"; shift; ;;
  esac
done

test -z $device && usage $0


if [ ! -b $device ]; then
   echo "ERROR: $device is not a block device file"
   exit 1;
fi

check_if_main_drive

check_if_big_size

echo "************************************************************"
echo "*         THIS WILL DELETE ALL THE DATA ON $device         *"
echo "*                                                          *"
echo "*         WARNING! Make sure your computer does not go     *"
echo "*                  in to idle mode while this script is    *"
echo "*                  running. The script will complete,      *"
echo "*                  but your SD card may be corrupted.      *"
echo "*                                                          *"
echo "*         Press <ENTER> to confirm....                     *"
echo "************************************************************"
read junk

udevadm control -s
unmount_all_partitions

execute "dd if=/dev/zero of=$device bs=1024 count=1024"

sync

cat << END | fdisk $device
n
p
1

+128M
n
p
2

+1024M
n
p
3


t
1
c
t
3
c
a
1
w
END

unmount_all_partitions
sleep 3

# handle various device names.
PARTITION1=${device}1
if [ ! -b ${PARTITION1} ]; then
        PARTITION1=${device}p1
fi

PARTITION2=${device}2
if [ ! -b ${PARTITION2} ]; then
        PARTITION2=${device}p2
fi

PARTITION3=${device}3
if [ ! -b ${PARTITION3} ]; then
        PARTITION3=${device}p3
fi

# make partitions.
echo "Formatting ${device} ..."
if [ -b ${PARTITION1} ]; then
  mkfs.vfat -F 32 -n "boot" ${PARTITION1}
else
  echo "Cant find boot partition in /dev"
fi

if [ -b ${PARITION2} ]; then
  mkfs.ext4 -L "rootfs" ${PARTITION2}
  
else
  echo "Cant find qnxfs partition in /dev"
fi

if [ -b ${PARITION3} ]; then
  mkfs.vfat -F 32 -n "qnxfs" ${PARTITION3}
else
  echo "Cant find rootfs partition in /dev"
fi

echo "Partitioning and formatting completed!"
mount | grep $device

udevadm control -S
echo "completed!"
