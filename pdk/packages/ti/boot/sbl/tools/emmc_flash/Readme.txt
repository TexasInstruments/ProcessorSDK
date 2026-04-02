eMMC flash tool
================

This tool allows you to flash eMMC on Linux machine.
This is useful when you want to boot j721e using eMMC bootmode.


Hardware setup
---------------

* USB cable (c-type) should be connected from the board to the Linux PC
* UART cable should be connected from the main_uart to Linux PC


Switch settings for DFU boot mode
---------------------------------

* j721e-evm settings  => SW8 = 1000 0000      SW9 = 0010 0000      SW3 = 0101 00 1010


Usage
-----

* Install dfu-util package on the Linux PC with
    ``sudo apt-get install dfu-util``

* To mount the emmc from j721e-evm board to the Linux PC, run following
    ``sudo ./dfu-boot.sh --j721e-evm --mount 0``

* Above script uses four binary files i.e sysfw.itb, tiboot3.bin, tispl.bin, u-boot.img and k3bootswitch.conf
  to boot till u-boot from that you can access eMMC as a device attached to the PC that is some thing like 
  /dev/sda or /dev/sdb. 

* If your eMMC is already partitioned you dont need to partition again you can check whether you eMMC is partitioned 
  or not by using `lsblk` command

* If it is not partitioned you can partition by running the following command
    ``sudo ./partition.sh --device /dev/sd*``  (/dev/sd* can be /dev/sda or /dev/sdb ...)

* By above command you partition the UDA partition of eMMC into 2 partitions i,e if your device is /dev/sda then you  
  get /dev/sda1, /dev/sda2 as partitions. Among two one is of size 62M with FAT 32 file system and the other is of size
  14.8G with linux file system.

* Now mount the first partition to some directory
    ``sudo mount /dev/sda1 <directory_path>

* Copy the required images (tiboot3.bin, tifs.bin, app) to that directory. By doing this you actually flashing images to eMMC. 

* Now change the boot switches to eMMC boot mode and boot through the eMMC boot mode.
   eMMC bootmode settings ==> SW8 = 1000 0000      SW9 = 0000 0000

Limitations
------------

* If eMMC is corrupted or partition table in eMMC was deleted then you can see invalid partition in /dev/ttyUSB0 in Main UART. 
  In that case you need to boot through MMCSD boot mode and run the partition.sh script. Now you can be able to flash eMMC by
  the above procedure. 


