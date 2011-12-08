#!/bin/bash
# Thanks c00kiemon5ter :D
# This is meant to be run from RoboVisionX , RoboVisionCLI directory ( thats why it has a ../ )
../Scripts/lsusb.py -Ui | grep -A1 "$1" | sed -n '$ s:.* \(\w\+\):\1:p'
exit 0
