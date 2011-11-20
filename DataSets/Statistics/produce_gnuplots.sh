#!/bin/bash

gnuplot -e 'set terminal png; set output "hypervisor_performance_delay.png"; set xlabel "RoboVision Uptime in ms"; set ylabel "Average Proccesing time in microseconds"; plot "hypervisor_performance.dat" using 1:2 with lines  title "GuarddoG Performance Graph"'

gnuplot -e 'set terminal png; set output "hypervisor_performance_fps.png"; set xlabel "RoboVision Uptime in ms"; set ylabel "Average Frame Rate in fps"; plot "hypervisor_performance.dat" using 1:3 with lines  title "GuarddoG Performance Graph"'



exit 0
