<html>
<head><meta http-equiv="refresh" content="1; url=controls.html"></head>
<body>

<?php

$the_file_path="commands.dat";
if (!file_exists ($the_file_path))
{
 $file=fopen($the_file_path,"w+");
 fclose($file);   
} 

if ( filesize($the_file_path)>200 )   
{  
   echo "The guarddog command line is beeing flooded , please wait ..!\n";
}
 else

{ //FILESIZE NOT FLOODED GUARD :)

if (isset($_REQUEST['direction'])) 
  {
    $size = strlen($_REQUEST['direction']);
    $user_ip = $_SERVER['REMOTE_ADDR'];
    if ( $size>50 ) 
     {
      echo $user_ip." : Overflow direction command filtered "; 
     } 
    else
    {
     echo $user_ip." : Direction Request : Go ".$_REQUEST['direction'];
     $file=fopen($the_file_path,"a");
     fwrite($file,$_REQUEST['direction']."(".$user_ip.")\n"); 
     fclose($file);
    }  
  }
   else
if (isset($_REQUEST['commandline'])) 
  {
   $size = strlen($_REQUEST['commandline']);
   $user_ip = $_SERVER['REMOTE_ADDR'];
   if ( $size>50 ) 
     {
      echo $user_ip." : Overflow command filtered "; 
     } 
    else
    {
      echo "Command line Request : ".$_REQUEST['commandline'];
      $file=fopen($the_file_path,"a");
      fwrite($file,$_REQUEST['commandline'].",(".$user_ip.")\n"); 
      fclose($file);
    }  
  }
 
} //FILESIZE NOT FLOODED GUARD :)

?>

</body>
</html>
