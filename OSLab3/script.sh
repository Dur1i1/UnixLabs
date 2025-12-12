#!/bin/bash
make -s
sudo insmod myModule.ko
sleep 1
echo "----"
cat /proc/SomeTest
sudo rmmod myModule
#sudo dmesg | tail -10
make clean -s
