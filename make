#!/bin/bash

clear

echo "Compiling The whole RoboVision Project :D"


echo "Compiling 3d Party Libraries.."
notify-send "Compiling 3d Party Libraries.."
cd 3dpartylibs
./make
cd ..


echo "Compiling VideoInput.."
cd VideoInput
./make
cd ..

echo "Compiling KinectInput.."
cd KinectInput
./make
cd ..


echo "Compiling GuarddoG Main Libs.."
notify-send "Compiling GuarddoG Main Libs.."


echo "Compiling VisualCortex.."
cd VisualCortex
./make
cd ..

echo "Compiling InputParser.."
cd InputParser
./make
cd ..

echo "Compiling MotorFoundation.."
cd MotorFoundation
./make
cd ..

echo "Compiling WorldMapping.."
cd WorldMapping
./make
cd ..

echo "Compiling World3D.."
cd World3D
./make
cd ..

echo "Compiling RVKnowledgeBase.."
cd RVKnowledgeBase
./make
cd ..

echo "Compiling RoboKernel.."
cd RoboKernel
./make
cd ..

#COMPILE INTERFACES

echo "Compiling GuarddoG Interfaces.."
notify-send "Compiling GuarddoG Interfaces.."


echo "Compiling ThreeDimensionMaps.."
cd ThreeDimensionMaps
./make
cd ..
cp ThreeDimensionMaps/bin/Release/ThreeDimensional RoboVisionX/ThreeDimensional


echo "Compiling RoboVision Console Front-end.."
cd RoboVisionCLI
./make
cd ..

echo "Compiling RoboVision WxWidgets Front-end.."
cd RoboVisionX
./make
cd ..

if [ -e RoboVisionX/bin/Release/RoboVisionX ]
then
  echo "RoboVisionX Executable is OK .."
  notify-send "RoboVisionX Executable is OK .."
else
  echo "RoboVisionX Executable FAILED !!!!!!!!!!"
  notify-send "RoboVisionX Executable FAILED !!!!!!!!!!"
fi

if [ -e RoboVisionCLI/bin/Release/RoboVisionCLI ]
then
  echo "RoboVisionCLI Executable is OK .."
  notify-send "RoboVisionCLI Executable is OK .."
else
  echo "RoboVisionCLI Executable FAILED !!!!!!!!!!"
  notify-send "RoboVisionCLI Executable FAILED !!!!!!!!!!"
fi


notify-send "GuarddoG compilation script has now finished.."
echo "Done.."
exit 0
