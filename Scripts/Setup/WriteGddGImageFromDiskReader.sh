#!/bin/bash
echo "This command will try to copy to device /dev/sdk , make sure this is the device you want , or change the script..!"
echo
echo -n "                Do you want to proceed dd @ /dev/sdk (Y/N)?"
read answer
if test "$answer" != "Y" -a "$answer" != "y";
then exit 0;
fi
 
echo "Copying .."
sudo dd if=robot/permfs/DiskImages.img of=/dev/sdk
exit 0 
