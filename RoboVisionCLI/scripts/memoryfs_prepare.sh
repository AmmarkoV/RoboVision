#!/bin/bash
echo "Prepare Apache Log directory , because it has been mounted on memory and it does not exist!"
mkdir /var/log/apache2
touch /var/log/apache2/error.log
mkdir /home/guarddog/RoboVisionRuntime/memfs/public_html
echo "Copy Web interface to the new partition!"
cp /home/guarddog/RoboVisionRuntime/scripts/webinterface.php /home/guarddog/RoboVisionRuntime/memfs/public_html/index.php
cp /home/guarddog/RoboVisionRuntime/scripts/execute.php /home/guarddog/RoboVisionRuntime/memfs/public_html/execute.php
cp /home/guarddog/RoboVisionRuntime/scripts/video_feeds.html /home/guarddog/RoboVisionRuntime/memfs/public_html/video_feeds.html
cp /home/guarddog/RoboVisionRuntime/scripts/controls.html /home/guarddog/RoboVisionRuntime/memfs/public_html/controls.html
cp /home/guarddog/RoboVisionRuntime/scripts/console_output.php /home/guarddog/RoboVisionRuntime/memfs/public_html/console_output.php
cp /home/guarddog/RoboVisionRuntime/scripts/keepalive.php /home/guarddog/RoboVisionRuntime/memfs/public_html/keepalive.php
echo "Clear data files and set permissions !"
touch /home/guarddog/RoboVisionRuntime/memfs/public_html/viewers.dat
touch /home/guarddog/RoboVisionRuntime/memfs/public_html/consoleout.dat
chmod 777 /home/guarddog/RoboVisionRuntime/memfs/public_html
chmod 777 /home/guarddog/RoboVisionRuntime/memfs/public_html/viewers.dat
chmod 777 /home/guarddog/RoboVisionRuntime/memfs/public_html/consoleout.dat
echo "Clear images !"
cp /home/guarddog/RoboVisionRuntime/empty.jpeg /home/guarddog/RoboVisionRuntime/memfs/public_html/feed0.jpeg
cp /home/guarddog/RoboVisionRuntime/empty.jpeg /home/guarddog/RoboVisionRuntime/memfs/public_html/feed1.jpeg
cp /home/guarddog/RoboVisionRuntime/empty.jpeg /home/guarddog/RoboVisionRuntime/memfs/public_html/feed2.jpeg
cp /home/guarddog/RoboVisionRuntime/empty.jpeg /home/guarddog/RoboVisionRuntime/memfs/public_html/feed3.jpeg
cp /home/guarddog/RoboVisionRuntime/empty.ppm /home/guarddog/RoboVisionRuntime/memfs/public_html/feed0.ppm
cp /home/guarddog/RoboVisionRuntime/empty.ppm /home/guarddog/RoboVisionRuntime/memfs/public_html/feed1.ppm
cp /home/guarddog/RoboVisionRuntime/empty.ppm /home/guarddog/RoboVisionRuntime/memfs/public_html/feed2.ppm
cp /home/guarddog/RoboVisionRuntime/empty.ppm /home/guarddog/RoboVisionRuntime/memfs/public_html/feed3.ppm
/etc/init.d/apache2 start

echo "Done"
exit 0
