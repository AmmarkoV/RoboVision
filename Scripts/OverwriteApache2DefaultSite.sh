#!/bin/bash
echo "Overwriting Default Apache Configuration! ( This is supposed to be run from RoboVision Root dir )"
sudo cp Scripts/new_default_site /etc/apache2/sites-availiable/default
sudo /etc/init.d/apache2 restart
exit 0
