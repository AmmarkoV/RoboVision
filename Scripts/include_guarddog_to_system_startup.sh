#!/bin/bash
echo "Please enter password to copy guarddog startup script to the system ..!"
echo "This is supposed to be run from the root directory of the RoboVision Project"
sudo cp Scripts/guarddog_linux_startup_script.sh /etc/init.d/guarddog_linux_startup_script.sh
sudo chmod +X /etc/init.d/guarddog_linux_startup_script.sh
sudo update-rc.d guarddog_linux_startup_script.sh defaults 
exit 0
