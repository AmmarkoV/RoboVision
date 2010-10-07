#!/bin/bash
echo "Compiling The whole RoboVision Project :D"

echo "Compiling VideoInput.."
cd VideoInput
./make
cd ..

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


echo "Done.."
exit 0
