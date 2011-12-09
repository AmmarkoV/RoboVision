#!/bin/bash
sudo mv /dev/ttyUSB0 /dev/ttyUSBX
sudo mv /dev/ttyUSB1 /dev/ttyUSB0
sudo mv /dev/ttyUSBX /dev/ttyUSB1
exit 0
