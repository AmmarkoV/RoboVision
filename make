#!/bin/bash

clear
echo "Compiling The whole RoboVision Project :D"

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ] ; do SOURCE="$(readlink "$SOURCE")"; done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
cd $DIR


echo "Compiling 3d Party Libraries.."
notify-send "Compiling 3d Party Libraries.."

if [ -d "3dpartylibs" ]; then
cd 3dpartylibs
./make
cd ..
else
  echo "Error : Could not find 3d party libs directory" 
fi



  if [ -d AmmarServer ] 
   then
     echo "AmmarServer dir already exists.."
     else
     echo "Could not find AmmarServer , cloning a fresh copy from github!"
     git clone git://github.com/AmmarkoV/AmmarServer.git
   fi


  if [ -e AmmarServer/make ] 
   then
     echo "Make bash script for AmmarServer is OK !"
   
     cd AmmarServer
     ./make
     cd ..

   else
     echo "The script was unable to find the VideoInput library or to automatically download it "
     echo "Please try installing git ( sudo apt-get install git ) or unzipping manually a recent version of VideoInput"
     echo "https://github.com/AmmarkoV/VideoInput   or http://ammar.gr"
     exit 1  
fi


if [ -d "VideoInput" ]; then
echo "Compiling VideoInput.."
notify-send "Compiling VideoInput.."
cd VideoInput
./make
cd ..
else
  echo "Error : Could not find VideoInput lib directory" 
fi
 
echo "Compiling GuarddoG Main Libs.."
notify-send "Compiling GuarddoG Main Libs.."


if [ -d "VisualCortex" ]; then
echo "Compiling VisualCortex.."
cd VisualCortex
./make
cd ..
else
  echo "Error : Could not find VisualCortex libs directory" 
fi


if [ -d "InputParser" ]; then
echo "Compiling InputParser.."
cd InputParser
./make
cd ..
else
  echo "Error : Could not find InputParser lib directory" 
fi

if [ -d "MotorFoundation" ]; then
echo "Compiling MotorFoundation.."
cd MotorFoundation
./make
cd ..
else
  echo "Error : Could not find MotorFoundation lib directory" 
fi


if [ -d "WorldMapping" ]; then
echo "Compiling WorldMapping.."
cd WorldMapping
./make
cd ..
else
  echo "Error : Could not find WorldMapping lib directory" 
fi
 
if [ -d "RVKnowledgeBase" ]; then
echo "Compiling RVKnowledgeBase.."
cd RVKnowledgeBase
./make
cd ..
else
  echo "Error : Could not find RVKnowledgeBase lib directory" 
fi

if [ -d "RoboKernel" ]; then
echo "Compiling RoboKernel.."
cd RoboKernel
./make
cd ..
else
  echo "Error : Could not find RoboKernel lib directory" 
fi

#COMPILE INTERFACES

echo "Compiling GuarddoG Interfaces.."
notify-send "Compiling GuarddoG Interfaces.."


if [ -d "ThreeDimensionMaps" ]; then
echo "Compiling ThreeDimensionMaps.."
cd ThreeDimensionMaps
./make
cd ..
cp ThreeDimensionMaps/bin/Release/ThreeDimensional RoboVisionX/ThreeDimensional
else
  echo "Error : Could not find ThreeDimensionMaps lib directory" 
fi

if [ -d "RoboVisionCLI" ]; then
echo "Compiling RoboVision Console Front-end.."
cd RoboVisionCLI
./make
cd ..
else
  echo "Error : Could not find RoboVisionCLI executable directory" 
fi

if [ -d "RoboVisionX" ]; then
echo "Compiling RoboVision WxWidgets Front-end.."
cd RoboVisionX
./make
cd ..
else
  echo "Error : Could not find RoboVisionX executable directory" 
fi


if [ -d "RoboVisionStarter" ]; then
echo "Compiling RoboVision Starter App.."
cd RoboVisionStarter
./make
cd ..
else
  echo "Error : Could not find RoboVisionX executable directory" 
fi


#
#         ------------------------------------------------------------------------------------------------------------------------------------
#           A FINAL CHECK ON THE BINARIES TO SEE THAT COMPILATION WAS A SUCCESS
#         ------------------------------------------------------------------------------------------------------------------------------------
#

Scripts/Setup/update_install.sh

notify-send "GuarddoG compilation script has now finished.."
echo "Done.."
exit 0
