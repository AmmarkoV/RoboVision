<html><head>
  <meta http-equiv="content-type" content="text/html; charset=ISO-8859-1">
  <title>GuardDog <?php echo date('l jS \of F Y h:i:s A'); ?></title> 
 <style>
    #center-content
    {
       width: 690px ;
       margin-left: auto ;
       margin-right: auto ; 
    }
  </style>
 </head>
 <body>
   <br>
  
  <div id="center-content">
  <?php
    $username = "guarddog";
    $password = "password";
    if ($_POST['txtusername']!= $username || $_POST['txtpassword']!= $password) { 
   ?>
     <h2>GuardDog HTTP interface</h2>
     This interface is password protected , please supply your credentials!<br><br>  
     <form name="form" method="post" action="<?php echo $_SERVER['php_SELF']; ?>">
      Username: <input type="text" name="txtusername" /> 
      Password: <input type="password" name="txtpassword" /><br>
      <input type="submit" value="Submit" />
     </form>
<?php
}
else {
?>
<!-- Start of protected site -->
 

  <fieldset>
   <legend>GuardDog Http Interface</legend>
   

  <table>
   <tbody>
   <tr>
    <td>
     <iframe name="videofeeds" frameborder=0 src="video_feeds.html" width="690" height="350"> </iframe> 
    </td>

   </tr>

   <tr>
    <td> 
      <!-- CONTROLS START HERE -->
       <iframe scrolling="no" name="guicontrols" frameborder=0 src="controls.html" width="690" height="260"> </iframe> 
      <!-- CONTROLS STOP HERE -->        
  
    </td>
   </tr>
  </tbody>
 </table>
 </fieldset>
 <iframe scrolling="no" name="keepalive" frameborder=0 src="keepalive.php" width="1" height="1"> </iframe> 
<!-- End of protected site -->
 <?php
 }
 ?> 

 </div>
 </body></html>

