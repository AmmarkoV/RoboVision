#!/bin/bash
echo "This script needs your password to copy guarddog startup script to the system ..!"
echo "you can also use sysv-rc-conf to see if the guarddog script is called on the runlevel of your choice !"
echo "This is supposed to be run from the root directory of the RoboVision Project"

echo
echo -n "                Do you want to proceed (Y/N)?"
read answer
if test "$answer" != "Y" -a "$answer" != "y";
then exit 0;
fi

sudo usermod -a -G audio guarddog 
sudo cp Scripts/guarddog_linux_startup_script.sh /etc/init.d/guarddog_linux_startup_script.sh
sudo chmod +X /etc/init.d/guarddog_linux_startup_script.sh
sudo update-rc.d guarddog_linux_startup_script.sh defaults 
exit 0
