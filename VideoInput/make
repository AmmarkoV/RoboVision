#!/bin/bash
echo "Hand made make script for VideoInput - VideoInputTester .."
echo "Should provide libVideoInput.a and VideoInputTester/bin/Debug/VideoInputTester"
echo "Written by Ammar Qammaz a.k.a AmmarkoV :) "
echo "Compiling files.."
gcc -c main.cpp -o main.o
gcc -c image_storage.cpp -o image_storage.o
gcc -c V4L2.cpp -o V4L2.o
gcc -c PrintV4L2.cpp -o PrintV4L2.o
gcc -c PixelFormats.cpp -o PixelFormats.o 
gcc -c PixelFormatConversions.cpp -o PixelFormatConversions.o 

echo "Linking files.."
ar  rcs libVideoInput.a main.o image_storage.o V4L2.o PrintV4L2.o PixelFormats.o PixelFormatConversions.o 

echo "Cleaning compiled object files.."
rm main.o image_storage.o V4L2.o PrintV4L2.o PixelFormats.o PixelFormatConversions.o 

echo "Compiling Tester.."
cd VideoInputTester
./make
cd ..

echo "Done.."
exit 0
