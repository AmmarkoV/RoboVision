#!/bin/bash

BINARY="robovisionstarter"

  if [ -e /usr/bin/$BINARY ]
  then
    sudo rm  /usr/bin/$BINARY 
  else  
    echo "No $BINARY detected "
  fi

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