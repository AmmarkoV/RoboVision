#!/bin/bash


BINPATH="RoboVisionX"
BINARY="robovisionx"
if [ -e $BINPATH/$BINARY ]
then
  echo "$BINARY Executable is OK .."
  notify-send "$BINARY Executable is OK .."  
  ls -l $BINPATH | grep $BINARY 
   if [ -e /usr/bin/$BINARY ]
          then
            echo "Updating installed binary..!"
            sudo cp $BINPATH/$BINARY /usr/bin/$BINARY 
   fi 
else
  echo "$BINARY Executable FAILED !!!!!!!!!!"
  notify-send "$BINARY Executable FAILED !!!!!!!!!!"
fi

BINPATH="RoboVisionCLI"
BINARY="robovision"
if [ -e $BINPATH/$BINARY ]
then
  echo "$BINARY Executable is OK .."
  notify-send "$BINARY Executable is OK .."  
  ls -l $BINPATH | grep $BINARY 
   if [ -e /usr/bin/$BINARY ]
          then
            echo "Updating installed binary..!"
            sudo cp $BINPATH/$BINARY /usr/bin/$BINARY 
   fi 
else
  echo "$BINARY Executable FAILED !!!!!!!!!!"
  notify-send "$BINARY Executable FAILED !!!!!!!!!!"
fi

BINPATH="RoboVisionStarter"
BINARY="robovisionstarter"
if [ -e $BINPATH/$BINARY ]
then
  echo "$BINARY Executable is OK .."
  notify-send "$BINARY Executable is OK .."  
  ls -l $BINPATH | grep $BINARY 
   if [ -e /usr/bin/$BINARY ]
          then
            echo "Updating installed binary..!"
            sudo cp $BINPATH/$BINARY /usr/bin/$BINARY 
   fi 
else
  echo "$BINARY Executable FAILED !!!!!!!!!!"
  notify-send "$BINARY Executable FAILED !!!!!!!!!!"
fi



exit 0