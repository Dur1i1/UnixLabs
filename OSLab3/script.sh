#!/bin/bash
make
sudo insmod myModule.ko
sleep 1
sudo rmmod myModule
sudo dmesg | tail -5
make clean
