<html>
  <head>
    <title>Disparity Mapping Processor</title>
  </head>
 <body>
<?php

//THIS FILE with a tmp subfolder ( chmodded to 777 ) and an installation of guarddog given in the dispdir will work as an online disparity processor  :)
$dispdir = "/home/ammar/RoboVision/VisualCortex/DisparityMapComparison";
$origdir = getcwd();
$outdir = $origdir."/tmp/";
$left = "left";
$right = "right";
$leftfile = "0";
$rightfile = "1";

if ( 
       ( !isset( $_FILES[$left]["name"]) ) ||
       ( !isset( $_FILES[$right]["name"]) )
    )
 { 
     echo "<br><br><h1>Bad Request <a href=\"index.php\">Go Back</a></h1>";
     return(0) ;
 }

echo "<br><center><h1>Processing Stereo Disparity</h1><br><a href=\"javascript:history.go(-1)\">Go back</a><br><br><br>";

echo "<table><tr><td>";
if ($_FILES[$left]["error"] > 0) { echo "Error: " . $_FILES[$left]["error"] . "<br>"; return 1; }
else
  {
    echo "Left Image uploaded<br>";
    echo "Upload: " . $_FILES[$left]["name"] . "<br>";
    echo "Type: " . $_FILES[$left]["type"] . "<br>";
    echo "Size: " . ($_FILES[$left]["size"] / 1024) . " kB<br>";
    //echo "Stored in: " . $_FILES[$left]["tmp_name"];
    echo "<br><br>";
  }

echo "</td><td>&nbsp;</td><td>";

if ($_FILES[$right]["error"] > 0) { echo "Error: " . $_FILES[$right]["error"] . "<br>"; return 1; }
else
  {
    echo "Right Image uploaded<br>";
    echo "Upload: " . $_FILES[$right]["name"] . "<br>";
    echo "Type: " . $_FILES[$right]["type"] . "<br>";
    echo "Size: " . ($_FILES[$right]["size"] / 1024) . " kB<br>";
    //echo "Stored in: " . $_FILES[$right]["tmp_name"];
    echo "<br><br>";
  }

echo "</td></tr></table>";


$leftfile = hash('ripemd160', $_FILES[$left]["name"])."0";
$rightfile = hash('ripemd160', $_FILES[$right]["name"])."1";
$outfile = $outdir.$leftfile.$rightfile;
$outfile_init = $outfile;
$outurl = "tmp/".$leftfile.$rightfile; 

#echo "Left File = ".$leftfile;
#echo "Right File = ".$rightfile;
#echo "Out File = ".$outfile;

$ext = pathinfo($_FILES[$left]['name'], PATHINFO_EXTENSION);
if ($ext!="ppm") { exec("convert ".$_FILES[$left]["tmp_name"]." ".$outdir.$leftfile.".ppm" ); } else
                  { exec("cp ".$_FILES[$left]["tmp_name"]." ".$outdir.$leftfile.".ppm" ); }

$ext = pathinfo($_FILES[$right]['name'], PATHINFO_EXTENSION);
if ($ext!="ppm") { exec("convert ".$_FILES[$right]["tmp_name"]." ".$outdir.$rightfile.".ppm" ); } else
                  { exec("cp ".$_FILES[$right]["tmp_name"]." ".$outdir.$rightfile.".ppm" ); }

exec("convert ".$_FILES[$left]["tmp_name"]." ".$outdir.$leftfile.".jpg" ); 
exec("convert ".$_FILES[$right]["tmp_name"]." ".$outdir.$rightfile.".jpg" ); 

 

if ( isset($_REQUEST["method"]) )
  { 
      if ($_REQUEST["method"]=="guarddog")
          { 
             chdir ($origdir); 
             echo "<h3>Using GuarddoG depth mapping</h3>";
             $command = $dispdir."/stereo_disparity_my_tester ".
                        $outdir.$leftfile.".ppm ".
                        $outdir.$rightfile.".ppm ".       
                        $outfile;
             
             #echo "<br>Executing ( ".$command." )";
             $output = "";
             $retval = 0;
           
             $time_start = microtime(true); 
             $output = exec($command); 
             $time_end = microtime(true); 
             echo "<br>Processing took ".(($time_end-$time_start)*1000)." ms <br>";            

          } else
      if ($_REQUEST["method"]=="opencv")
          { 
             chdir ($origdir); 
             echo "<h3>Using OpenCV depth mapping</h3>";
             $command = $dispdir."/stereo_disparity_opencv_tester_viscrtx ".
                        $outdir.$leftfile.".ppm ".
                        $outdir.$rightfile.".ppm ".       
                        $outfile;
             
             #echo "<br>Executing ( ".$command." )<br>";
             $output = "";
             $retval = 0;
             
             $time_start = microtime(true); 
             $output = exec($command); 
             $time_end = microtime(true); 
             echo "<br>Processing took ".(($time_end-$time_start)*1000)." ms <br>";      
          }  else
      if ($_REQUEST["method"]=="libelas")
          { 
             echo "<h3>Using LibELAS depth mapping</h3>";
             chdir ($outdir);

             $command = $dispdir."/elas ".
                        $outdir.$leftfile.".ppm ".
                        $outdir.$rightfile.".ppm ";

             #echo "<br>Executing ( ".$command." )<br>";
             $time_start = microtime(true); 
             $output = exec($command); 
             $time_end = microtime(true); 
             echo "<br>Processing took ".(($time_end-$time_start)*1000)." ms <br>";      
             
             $outfile_init= $outdir.$leftfile."_disp.ppm"; 
             //libelas produces two views , we through one out..
             exec("rm ".$outdir.$rightfile."_disp.ppm"); 

             chdir ($origdir);
          } 
 
     exec("convert ".$outfile_init." ".$outfile.".jpg" ); 
     #clear in between ppms..!
     exec("rm ".$outdir.$leftfile.".ppm ".$outdir.$rightfile.".ppm ".$outfile_init ); 
       
  } 

      echo "Done!<br><br><br>"; 
      echo "<img id=\"left\" src=\"tmp/".$leftfile.".jpg\"> "; 
      echo "<img id=\"right\" src=\"tmp/".$rightfile.".jpg\"><br><br>"; 
      echo "<img id=\"out\" src=\"".$outurl.".jpg\" >";
?>
<br><br><br><br><br><br>
</body>
</html>
