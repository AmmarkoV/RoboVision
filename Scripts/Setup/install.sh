#!/bin/bash

BINARY="robovision"
  if [ -e $BINARY ]
  then
    echo "$BINARY App is OK :) , including it to system binaries .."
    sudo cp $BINARY /usr/bin/$BINARY 
  else
    echo "$BINARY App could not be installed , you probably got a library missing"
    exit 1
  fi

BINARY="robovisionx"
  if [ -e $BINARY ]
  then
    echo "$BINARY App is OK :) , including it to system binaries .."
    sudo cp $BINARY /usr/bin/$BINARY 
  else
    echo "$BINARY App could not be installed , you probably got a library missing"
    exit 1
  fi
exit 0


#make /robot/ etc..
#sudo chmod 744 /usr/share/flashyslideshows/app_clipart/*  
#sudo chmod 744 /usr/share/flashyslideshows/app_clipart/*  

exit 0