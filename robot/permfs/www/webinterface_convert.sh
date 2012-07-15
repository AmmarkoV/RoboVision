#!/bin/bash
#echo "Convert PPM to JPEG" 
convert memfs/public_html/feed0.ppm memfs/public_html/feed0.jpeg
convert memfs/public_html/feed1.ppm memfs/public_html/feed1.jpeg
convert memfs/public_html/feed2.ppm memfs/public_html/feed2.jpeg
convert memfs/public_html/feed3.ppm memfs/public_html/feed3.jpeg
#echo "Done"
#killall convert
exit 0
