#!/bin/bash

echo "Installing GuarddoG into system"
sudo echo " "

BINPATH="RoboVisionStarter"
BINARY="robovisionstarter"
  if [ -e $BINPATH/$BINARY ]
  then
    echo "$BINARY App is OK :) , including it to system binaries .."
    sudo cp $BINPATH/$BINARY /usr/bin/$BINARY 
  else
    echo "$BINARY App could not be installed , you probably got a library missing"
    exit 1
  fi

BINPATH="RoboVisionCLI"
BINARY="robovision"
  if [ -e $BINPATH/$BINARY ]
  then
    echo "$BINARY App is OK :) , including it to system binaries .."
    sudo cp $BINPATH/$BINARY /usr/bin/$BINARY 
  else
    echo "$BINARY App could not be installed , you probably got a library missing"
    exit 1
  fi

BINPATH="RoboVisionX"
BINARY="robovisionx"
  if [ -e $BINPATH/$BINARY ]
  then
    echo "$BINARY App is OK :) , including it to system binaries .."
    sudo cp $BINPATH/$BINARY /usr/bin/$BINARY 
  else
    echo "$BINARY App could not be installed , you probably got a library missing"
    exit 1
  fi


BINPATH="Scripts/Setup"
BINARY="guarddog"
  if [ -e $BINPATH/$BINARY ]
  then
    echo "$BINARY App is OK :) , including it to system binaries .."
    sudo cp $BINPATH/$BINARY /usr/bin/$BINARY 
  else
    echo "$BINARY App could not be installed , you probably got a library missing"
    exit 1
  fi


echo Please add robovisionstarter to /etc/rc.conf DAEMONS 

echo "Would you like to create a tmpfs and move files in  /robot/ ( and /robot/memfs directory ) ?!"
echo
echo -n "                Do you want to proceed (Y/N)?"
read answer
if test "$answer" != "Y" -a "$answer" != "y";
then exit 0;
fi

mkdir robot/memfs
sudo mkdir /robot/
sudo cp -rf robot/* /robot/
sudo Scripts/Setup/AddToFstab_RobotDir.sh 
 

#sudo chmod 744  /robot/*   

exit 0