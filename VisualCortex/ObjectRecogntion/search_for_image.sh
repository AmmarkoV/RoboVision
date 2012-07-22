#!/bin/bash

for f in *.jpg; 
do 

find_image $1 $f
OUT=$?
if [ $OUT -eq 0 ];
then
         echo "File $1 found in file $f .."; 
#else
        #echo "User account does not exists in /etc/passwd file!"
fi

done

exit 0