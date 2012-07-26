#!/bin/bash

BINPATH="RoboVisionCLI"
BINARY="robovision"
  if [ -e $BINPATH/$BINARY ]
  then
    echo "$BINARY App is OK :) , including it to system binaries .."
    sudo cp $BINPATH/$BINARY /usr/bin/$BINARY 
  else
    echo "$BINARY App could not be installed , you probably got a library missing"
    exit 1
  fi

BINPATH="RoboVisionX"
BINARY="robovisionx"
  if [ -e $BINPATH/$BINARY ]
  then
    echo "$BINARY App is OK :) , including it to system binaries .."
    sudo cp $BINPATH/$BINARY /usr/bin/$BINARY 
  else
    echo "$BINARY App could not be installed , you probably got a library missing"
    exit 1
  fi


BINPATH="Scripts/Setup"
BINARY="guarddog"
  if [ -e $BINPATH/$BINARY ]
  then
    echo "$BINARY App is OK :) , including it to system binaries .."
    sudo cp $BINPATH/$BINARY /usr/bin/$BINARY 
  else
    echo "$BINARY App could not be installed , you probably got a library missing"
    exit 1
  fi


echo "Would you like to create a tmpfs and move files in  /robot/ ( and /robot/memfs directory ) ?!"
echo
echo -n "                Do you want to proceed (Y/N)?"
read answer
if test "$answer" != "Y" -a "$answer" != "y";
then exit 0;
fi

mkdir robot/memfs
sudo mkdir /robot/
sudo cp -rf robot/* /robot/
sudo Scripts/Setup/AddToFstab_RobotDir.sh 

  if [ -d /etc/httpd/conf ]
  then
    echo "Found an Apache installation , adding GuarddoG webinterface"
   sudo mkdir /etc/httpd/conf/vhosts
   sudo ln -s /robot/permfs/ConfigurationFilesTemplate/guarddog_webinterface_apache.conf /etc/httpd/conf/vhosts/guarddog.conf
   #add new vhost to /etc/httpd/conf/httpd.conf
   if cat /etc/httpd/conf/httpd.conf | grep -q "Include conf/vhosts/guarddog.conf"
     then
       echo "GuarddoG seems to have been added to apache!"  
    else
      echo "GuarddoG doenst seem to exist in apache configuration!" 
      sudo sh -c 'echo "#GuarddoG Vhost:" >>/etc/httpd/conf/httpd.conf' 
      sudo sh -c 'echo "Include conf/vhosts/guarddog.conf" >>/etc/httpd/conf/httpd.conf'  
   fi
   
  if [ -e /srv/http/index.html ]
  then
    echo "There seems to already be a static page on /srv/http/index.html , skipping copy"
  else
    echo "Copying Static web page..!"
    sudo cp /robot/permfs/www/guarddog.jpg  /srv/http/guarddog.jpg
    sudo cp /robot/permfs/www/StaticPlaceholderPage.html  /srv/http/index.html
   fi
  
   sudo rc.d restart httpd
  else
    echo "Could not find an apache installation" 
  fi




#sudo chmod 744  /robot/*   

exit 0