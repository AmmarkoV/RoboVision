#!/bin/bash

BINARY="robovision"

  if [ -e /usr/bin/$BINARY ]
  then
    sudo rm  /usr/bin/$BINARY 
  else  
    echo "No $BINARY detected "
  fi

BINARY="robovisionx"

  if [ -e /usr/bin/$BINARY ]
  then
    sudo rm  /usr/bin/$BINARY
  else 
   echo "No $BINARY Binary detected"
  fi
exit 0


#make /robot/ etc..
#sudo chmod 744 /usr/share/flashyslideshows/app_clipart/*  
#sudo chmod 744 /usr/share/flashyslideshows/app_clipart/*  

exit 0