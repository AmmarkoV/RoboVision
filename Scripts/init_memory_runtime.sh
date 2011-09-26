#!/bin/bash

sudo mount -t tmpfs -o size=100m tmpfs RoboVisionRuntime/

echo "Preparing Memory Runtime Space for program!"
echo "Changing owner of Runtime to guarddog :) !"
sudo chown guarddog:guarddog RoboVisionRuntime


echo "Create Directory Structure!"
mkdir RoboVisionRuntime/bin
sudo chown guarddog:guarddog RoboVisionRuntime/bin
mkdir RoboVisionRuntime/bin/Release
sudo chown guarddog:guarddog RoboVisionRuntime/bin/Release
mkdir RoboVisionRuntime/scripts
sudo chown guarddog:guarddog RoboVisionRuntime/scripts
mkdir RoboVisionRuntime/Sounds
sudo chown guarddog:guarddog RoboVisionRuntime/Sounds
mkdir RoboVisionRuntime/Stored
sudo chown guarddog:guarddog RoboVisionRuntime/Stored
mkdir RoboVisionRuntime/memfs
sudo chown guarddog:guarddog RoboVisionRuntime/memfs
mkdir RoboVisionRuntime/memfs/public_html
sudo chown guarddog:guarddog RoboVisionRuntime/memfs/public_html


echo "Copy Files to run!"

cp RoboVisionCLI/haarcascade_frontalface_alt.xml RoboVisionRuntime/haarcascade_frontalface_alt.xml
cp RoboVisionCLI/empty.jpeg RoboVisionRuntime/empty.jpeg
cp RoboVisionCLI/empty.ppm RoboVisionRuntime/empty.ppm
cp RoboVisionCLI/guard.ini RoboVisionRuntime/guard.ini

cp RoboVisionCLI/RoboVisionCLICompat RoboVisionRuntime/RoboVisionCLICompat
chmod +x RoboVisionRuntime/RoboVisionCLICompat

cp RoboVisionCLI/bin/Release/RoboVisionCLI RoboVisionRuntime/bin/Release/RoboVisionCLI
chmod +x RoboVisionRuntime/bin/Release/RoboVisionCLI

cp RoboVisionCLI/scripts/* RoboVisionRuntime/scripts/
chmod +x RoboVisionRuntime/scripts/*

cp DataSets/Sounds/* RoboVisionRuntime/Sounds/


echo "everything done!"

echo "Done"
exit 0
