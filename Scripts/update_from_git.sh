#!/bin/bash

#Reminder , to get a single file out of the repo use  "git checkout -- path/to/file.c"
  if [ -d AmmarServer ] 
   then
     echo "AmmarServer dir exists so lets update it first.." 
     cd AmmarServer
     ./update_from_git.sh
     cd ..
   fi


git reset --hard HEAD
git pull
./make
exit 0
