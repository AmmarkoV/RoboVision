#!/bin/bash
echo "Make Devices Readable"
sudo chown guarddog:guarddog /dev/video0 
sudo chown guarddog:guarddog /dev/video1 
sudo chown guarddog:guarddog /dev/dsp
sudo chown guarddog:guarddog /dev/ttyUSB0
sudo chown guarddog:guarddog /dev/ttyUSB1
echo "Device Prepare is Done.."
exit 0
