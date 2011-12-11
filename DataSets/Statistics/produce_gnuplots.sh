#!/bin/bash

gnuplot -e 'set terminal png; set output "hypervisor_performance_delay.png"; set xlabel "RoboVision Uptime in seconds"; set ylabel "Average Frame Proccesing time in microseconds"; plot "hypervisor_performance.dat" using 1:2 with lines  title "GuarddoG Performance Graph"'

gnuplot -e 'set terminal png; set output "hypervisor_performance_fps.png"; set xlabel "RoboVision Uptime in seconds"; set ylabel "Average Frame Rate in fps"; plot "hypervisor_performance.dat" using 1:3 with lines  title "GuarddoG Performance Graph"'


gnuplot -e 'set terminal png; set output "disparity_map_performance_delay.png"; set xlabel "RoboVision Uptime in seconds"; set ylabel "Average Disparity Mapping Proccesing time in microseconds"; plot "hypervisor_disparity_mapping_performance.dat" using 1:2 with lines  title "Disparity Mapping Performance Graph"'


gnuplot -e 'set terminal png; set output "disparity_map_times.png"; set xlabel "Percentage of frame covered with depth data"; set ylabel "Average Disparity Mapping Proccesing time in microseconds"; plot "hypervisor_disparity_mapping_performance.dat" using 3:2  title "Disparity Mapping Performance Graph"'

gnuplot -e 'set terminal png; set output "disparity_scale.png"; set xlabel "Depth Units"; set ylabel "Centimeters"; plot "disparity_scale.dat" using 1:2  title "Disparity Scale"'



exit 0
