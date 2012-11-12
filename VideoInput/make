#!/bin/bash
echo "VideoInput and its testers / Provides libVideoInput.a / Author : Ammar Qammaz"  

if [ -e libVideoInput.a ]
then
  rm libVideoInput.a
fi 


gcc -c PixelFormatConversions.c -o PixelFormatConversions.o 
gcc -c PixelFormats.c -o PixelFormats.o 
gcc -c PrintV4L2.c -o PrintV4L2.o
gcc -c V4L2_c.c -o V4L2_c.o
gcc -c image_storage.c -o image_storage.o
gcc -c image_storage_png.c -o image_storage_png.o
gcc -c image_storage_jpg.c -o image_storage_jpg.o 
gcc -c main.c -o main.o
gcc -c state.c -o state.o 
gcc -c tools.c -o tools.o 
 
PARTS_OF_LIB="main.o image_storage.o image_storage_png.o image_storage_jpg.o V4L2_c.o PrintV4L2.o PixelFormats.o tools.o state.o  PixelFormatConversions.o"

ar  rcs  libVideoInput.a $PARTS_OF_LIB

rm $PARTS_OF_LIB

if [ -e libVideoInput.a ]
then
  echo "Success.."

      if [ -d VideoInputTester ]
      then
       cd VideoInputTester 
       ./make
       cd ..
     fi

      if [ -d VideoInputGUITester ]
      then
       cd VideoInputGUITester 
       ./make
       cd ..
     fi

      if [ -d VideoSnapShot ]
      then
       cd VideoSnapShot 
       ./make
       cd ..
     fi 

else
  echo "Failure.."
fi

 
exit 0
