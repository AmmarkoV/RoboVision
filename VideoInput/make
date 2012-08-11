#!/bin/bash
echo "VideoInput and its testers / Provides libVideoInput.a / Author : Ammar Qammaz"  

if [ -e libVideoInput.a ]
then
  rm libVideoInput.a
fi 


gcc -c main.cpp -o main.o
gcc -c image_storage.cpp -o image_storage.o
gcc -c image_storage_png.cpp -o image_storage_png.o
gcc -c image_storage_jpg.cpp -o image_storage_jpg.o
gcc -c V4L2.cpp -o V4L2.o
gcc -c PrintV4L2.cpp -o PrintV4L2.o
gcc -c PixelFormats.cpp -o PixelFormats.o 
gcc -c PixelFormatConversions.cpp -o PixelFormatConversions.o 

ar  rcs libVideoInput.a main.o image_storage.o image_storage_png.o image_storage_jpg.o V4L2.o PrintV4L2.o PixelFormats.o PixelFormatConversions.o 

rm main.o image_storage.o image_storage_png.o V4L2.o PrintV4L2.o PixelFormats.o PixelFormatConversions.o 
 

if [ -e libVideoInput.a ]
then
  echo "Success.."
else
  echo "Failure.."
fi

 
exit 0
