#!/bin/bash

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
exit 0


#make /robot/ etc..
#sudo chmod 744 /usr/share/flashyslideshows/app_clipart/*  
#sudo chmod 744 /usr/share/flashyslideshows/app_clipart/*  

exit 0