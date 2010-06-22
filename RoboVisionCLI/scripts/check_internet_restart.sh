#!/bin/sh

HOST=www.otenet.gr # < your favourite ISP here , host to spam ping
FAILLOG=fail_log.txt # file to log failures to
TMPFILE=/tmpfs/conn_monitor # temporary file
INTERVAL=300 # 5 lepta , how often to ping, in seconds
USERNAME=routerusername
PASSWORD=routerpassword
ROUTERIP=routerip # For example 192.168.1.1
ROUTERREBOOT=restart # άλλα  ADSL modems θέλουν system reboot , reboot , κτλ 

append_log()
{
OLDIFS=$IFS
IFS=$
for LINE in `cat -E $TMPFILE`; do
LINE=`echo $LINE | tr -d "\n"`
echo " " $LINE >> $FAILLOG
done
IFS=$OLDIFS
}

echo "Script Started , will telnet " $ROUTERIP " and ask for " $ROUTERREBOOT " when needed!"

while [ 1 ]; do
ping -c 4 $HOST &> $TMPFILE
if [ $? -eq 0 ]; then
echo "PING SUCCESS at "`date`"."
else
echo "PING FAILURE at "`date`"."
# we failed, so run a traceroute for diagnostic purposes
#traceroute $HOST 1>> $TMPFILE 2>&1
echo "PING FAILURE at "`date`"." >> $FAILLOG
(
sleep 5
echo "$USERNAME\r"
sleep 5
echo "$PASSWORD\r"
sleep 5
echo "$ROUTERREBOOT\r"
sleep 40
) | telnet $ROUTERIP 23
append_log
fi
sleep $INTERVAL
done
