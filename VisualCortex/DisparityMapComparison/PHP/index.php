<html>
<body>

<br><br><br><br>
<center>

<h2>Online Stereoscopic Disparity Depth Mapping</h2>

<img src="help.jpg" width = 600 ><br>

You can <a href="http://vision.middlebury.edu/stereo/data/">find some stereo tests to try here</a><br>
The images shown above are the following ( <a href="cones0.jpg">Left</a> ) ( <a href="cones1.jpg">Right</a> ) <br>
The underlying processor is <a href="https://github.com/AmmarkoV/RoboVision/tree/master/VisualCortex">GuarddoG's visual cortext</a>  <br><br><br>


<form action="process.php" method="post" enctype="multipart/form-data">
<label for="left">Left View: </label><input type="file" name="left" id="left"><br><br>
<label for="right">Right View: </label><input type="file" name="right" id="right"><br><br>

<label for="method">Disparity Method Usage : </label>
<select name="method">
<option value="guarddog">GuarddoG</option>
<option value="opencv">OpenCV</option>
<option value="libelas">libELAS</option> 
</select>

<br><br><br><br>

<input type="submit" name="submit" value="Submit">
</form>
</center>

</body>
</html> 
