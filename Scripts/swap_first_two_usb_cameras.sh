#!/bin/bash
sudo mv /dev/video0 /dev/videoX
sudo mv /dev/video1 /dev/video0
sudo mv /dev/videoX /dev/video1
exit 0
