#!/bin/bash

svn checkout http://madwifi-project.org/svn/madwifi/trunk madwifi
cd madwifi
sudo make install 

sudo modprobe -r ath5k
sudo modprobe ath_pci
sudo echo "blacklist ath5k" > /etc/modprobe.d/blacklist-custom.conf
sudo echo "options ath_pci autocreate=ap" > /etc/modprobe.d/blacklist-custom.conf

if [ -e /etc/modprobe.d/blacklist-ath_pci.conf  ]
then
  echo "Warning there is a blacklist rule for the Madwifi driver .."  
  #sudo rm /etc/modprobe.d/blacklist-ath_pci.conf 
else 

fi

exit 0
