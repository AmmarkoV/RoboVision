#!/bin/bash

gnuplot>  plot  "hypervisor_performance.dat" using 1:2 title 'Column', \ "hypervisor_performance.dat" using 1:3 title 'Beam' \ set terminal 'jpeg' \ set output 'myplot.jpg'


exit 0
