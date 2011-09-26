#!/bin/bash
echo "Prepare Apache Log directory , because it has been mounted on memory and it does not exist!"
mkdir /var/log/apache2
touch /var/log/apache2/error.log 
echo "Copy Web interface to the new partition!"
cp RoboVisionRuntime/scripts/webinterface.php RoboVisionRuntime/memfs/public_html/index.php
cp RoboVisionRuntime/scripts/host.php RoboVisionRuntime/memfs/public_html/host.php
cp RoboVisionRuntime/scripts/sensor_data RoboVisionRuntime/memfs/public_html/sensor_data

cp RoboVisionRuntime/scripts/webinterface.php RoboVisionRuntime/memfs/public_html/index.php
cp RoboVisionRuntime/scripts/execute.php RoboVisionRuntime/memfs/public_html/execute.php
cp RoboVisionRuntime/scripts/video_feeds.html RoboVisionRuntime/memfs/public_html/video_feeds.html
cp RoboVisionRuntime/scripts/controls.html RoboVisionRuntime/memfs/public_html/controls.html
cp RoboVisionRuntime/scripts/console_output.php RoboVisionRuntime/memfs/public_html/console_output.php
cp RoboVisionRuntime/scripts/keepalive.php RoboVisionRuntime/memfs/public_html/keepalive.php
echo "Clear data files and set permissions !"
touch RoboVisionRuntime/memfs/public_html/viewers.dat
touch RoboVisionRuntime/memfs/public_html/consoleout.dat
chmod 777 RoboVisionRuntime/memfs/public_html
chmod 777 RoboVisionRuntime/memfs/public_html/viewers.dat
chmod 777 RoboVisionRuntime/memfs/public_html/consoleout.dat
echo "Clear images !"
cp RoboVisionRuntime/empty.jpeg RoboVisionRuntime/memfs/public_html/feed0.jpeg
cp RoboVisionRuntime/empty.jpeg RoboVisionRuntime/memfs/public_html/feed1.jpeg
cp RoboVisionRuntime/empty.jpeg RoboVisionRuntime/memfs/public_html/feed2.jpeg
cp RoboVisionRuntime/empty.jpeg RoboVisionRuntime/memfs/public_html/feed3.jpeg
cp RoboVisionRuntime/empty.ppm RoboVisionRuntime/memfs/public_html/feed0.ppm
cp RoboVisionRuntime/empty.ppm RoboVisionRuntime/memfs/public_html/feed1.ppm
cp RoboVisionRuntime/empty.ppm RoboVisionRuntime/memfs/public_html/feed2.ppm
cp RoboVisionRuntime/empty.ppm RoboVisionRuntime/memfs/public_html/feed3.ppm
sudo /etc/init.d/apache2 start

sudo chown guarddog:guarddog RoboVisionRuntime/memfs/public_html/*
sudo chown guarddog:guarddog RoboVisionRuntime/memfs/*
sudo chown guarddog:guarddog RoboVisionRuntime/*
sudo chown guarddog:guarddog RoboVisionRuntime/scripts*
sudo chown guarddog:guarddog RoboVisionRuntime/Sounds*


echo "Done"
exit 0
