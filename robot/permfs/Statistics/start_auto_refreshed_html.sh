#!/bin/bash
firefox auto_refreshed.html&
echo "Press [CTRL+C] to stop Auto Refresh.."
while true
do
	sleep 1
        ./produce_gnuplots.sh
done
exit 0
