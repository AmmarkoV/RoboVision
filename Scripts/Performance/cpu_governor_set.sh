#!/bin/bash
#CPU Frequency governor script by HoKaze (version 0.3) modded for guarddog

#runs this script as root if not already
if [ ! $( id -u ) -eq 0 ]; then
	exec gksu "${0}" # call this script as root
	exit ${?}  # since we're 'execing' above, we wont reach this exit
               # unless something goes wrong.
fi
 
governor="performance"

for CPUFREQ in /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor
do
	[ -f $CPUFREQ ] || continue
	echo -n $governor > $CPUFREQ
done
