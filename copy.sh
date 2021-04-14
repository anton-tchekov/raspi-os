#!/bin/sh
sudo mount /dev/mmcblk0p1 /mnt/usb
sudo cp -rf bin/kernel7.img /mnt/usb/kernel7.img
sudo umount /mnt/usb
