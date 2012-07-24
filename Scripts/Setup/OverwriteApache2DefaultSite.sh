#!/bin/bash

echo "This script will override default setting for apache2 , it is supposed to be run from RoboVision Root dir" 

echo
echo -n "                Do you want to proceed (Y/N)?"
read answer
if test "$answer" != "Y" -a "$answer" != "y";
then exit 0;
fi

sudo cp /etc/apache2/sites-availiable/default robot/permfs/ConfigurationFilesTemplate/old_default_site
sudo cp robot/permfs/ConfigurationFilesTemplate/new_default_site /etc/apache2/sites-availiable/default
sudo /etc/init.d/apache2 restart
exit 0
