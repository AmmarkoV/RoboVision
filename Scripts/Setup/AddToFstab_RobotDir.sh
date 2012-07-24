#!/bin/bash

if [ -d /robot ]; then
	echo "/robot/ directory already exists"
else 
	echo "Creating /robot/ directory"
        sudo mkdir /robot
fi 

if cat /etc/fstab | grep -q "/robot"
then
   echo "GuarddoG tmpfs seems to already exist in Fstab" 
  exit 0
else
   echo "GuarddoG tmpfs doesnt seems to exist in Fstab" 
   sudo sh -c 'echo "tmpfs /robot/memfs      tmpfs rw,nodev,nosuid,size=64M,umask=0000          0 0" >> /etc/fstab' 
fi
exit 0