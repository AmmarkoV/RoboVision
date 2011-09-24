#!/bin/bash
echo "Make Devices Readable"
sudo chown guarddog:guarddog /dev/video0 /dev/video1 
sudo chown guarddog:guarddog /dev/dsp
echo "Done"
exit 0
