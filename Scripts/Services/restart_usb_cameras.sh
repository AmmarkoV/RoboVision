#!/bin/bash
#sudo rmmod videodev
sudo rmmod uvcvideo
#sudo rmmod gspca_main

#sudo modprobe gspca_main
sudo modprobe uvcvideo
#sudo modprobe videodev
exit 0
