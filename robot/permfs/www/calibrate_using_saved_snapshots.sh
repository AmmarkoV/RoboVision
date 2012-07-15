#!/bin/bash
echo "Calibrate using saved snapshots assumes there are files named"
echo "   snapshot_2011-06-25_12:24:08_0.ppm "
echo "   snapshot_2011-06-25_12:24:08_1.ppm , etc "
echo "on memfs folder that will be transported to memfs/calibration/cam1 and"
echo "memfs/calibration/cam0 to be processed by the second opencv script.. "
cd memfs
cp *0.ppm calibration/cam0
cp *1.ppm calibration/cam1
cd calibration 
./CalibrateBoth
cd ../..
echo "Done"

exit 0
