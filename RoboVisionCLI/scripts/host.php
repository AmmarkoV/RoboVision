<?php sleep(2) ?>
<html>
 <head>
  <title>Host Stats</title>
 </head>
 <body>
  <?php  
   if (!file_exists ("sensor_data"))
   {
     // AUTO GENERATION OF FILE :)
     echo "Auto Generation of file to take care of permissions! :P ";
     $file=fopen("sensor_data","w+"); 
     fclose($file);   
     return 0;
   } 
  
    // echo exec("whoami");
    //ADD SENSOR DATA
    echo exec("echo \"SENSOR DATA :\n\" > sensor_data");
    echo exec("sensors >> sensor_data");
    echo exec("echo \"\n\n\" >> sensor_data");
      
    //ADD UPTIME DATA
    echo exec("echo \"UPTIME DATA :\n\" >> sensor_data");
    echo exec("uptime >> sensor_data"); 
    echo exec("echo \"\n\n\" >> sensor_data");

    //ADD SWAP DATA
    echo exec("echo \"SWAP DATA :\n\" >> sensor_data");
    echo exec("free >> sensor_data"); 
    echo exec("echo \"\n\n\" >> sensor_data");

    //ADD DISK USAGE DATA
    echo exec("echo \"DISK USAGE DATA :\n\" >> sensor_data");
    echo exec("df -hT >> sensor_data"); 
    echo exec("echo \"\n\n\" >> sensor_data");


    $file=fopen("sensor_data","r") or exit("Internal error!");
    if ($file) 
    { 
     $theData = fread($file, filesize("sensor_data")); 
     echo nl2br($theData);
     fclose($file);   
    } else
    { echo "Could not retrieve new filesize from cache :S<br>"; }
   
  ?>
 </body>
</html>
