#!/bin/bash
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

notify-send "GuarddoG compilation is done.."
echo "Done.."
exit 0
