#!/bin/bash
git clone git://github.com/AmmarkoV/RoboVision.git
./apt-get-guarddog-dependencies
./RoboVisionX/scripts/include_guarddog_to_system_startup.sh
./make
echo "Done"
exit 0
