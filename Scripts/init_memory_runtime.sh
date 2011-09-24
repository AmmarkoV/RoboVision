#!/bin/bash

sudo mount -t ramfs -o size=100m ramfs RoboVisionRuntime/

echo "Preparing Memory Runtime Space for program!"
echo "Changing owner of Runtime to guarddog :) !"
sudo chown guarddog:guarddog RoboVisionRuntime


echo "Create Directory Structure!"
mkdir RoboVisionRuntime/bin
mkdir RoboVisionRuntime/bin/Release
mkdir RoboVisionRuntime/scripts
mkdir RoboVisionRuntime/Sounds
mkdir RoboVisionRuntime/Stored
mkdir RoboVisionRuntime/memfs
mkdir RoboVisionRuntime/memfs/public_html


echo "Copy Files to run!"

cp RoboVisionCLI/haarcascade_frontalface_alt.xml RoboVisionRuntime/haarcascade_frontalface_alt.xml
cp RoboVisionCLI/empty.jpeg RoboVisionRuntime/empty.jpeg
cp RoboVisionCLI/empty.ppm RoboVisionRuntime/empty.ppm
cp RoboVisionCLI/guard.ini RoboVisionRuntime/guard.ini
cp RoboVisionCLI/RoboVisionCLICompat RoboVisionRuntime/RoboVisionCLICompat
cp RoboVisionCLI/bin/Release/RoboVisionCLI RoboVisionRuntime/bin/Release/RoboVisionCLI 
cp RoboVisionCLI/scripts/* RoboVisionRuntime/scripts/
cp RoboVisionCLI/Sounds/* RoboVisionRuntime/Sounds/

echo "everything done!"

echo "Done"
exit 0
