#!/bin/sh
#enable the module
echo "Now Enable The LongSung Module"

#echo 13 > /sys/class/gpio/export

echo out > /sys/class/gpio/gpio45/direction
echo out > /sys/class/gpio/gpio26/direction
echo out > /sys/class/gpio/gpio23/direction
echo in > /sys/class/gpio/gpio22/direction
echo out > /sys/class/gpio/gpio13/direction

echo "Now Reset"
echo 0 > /sys/class/gpio/gpio45/value
echo 1 > /sys/class/gpio/gpio26/value
echo 1 > /sys/class/gpio/gpio23/value
echo 1 > /sys/class/gpio/gpio13/value
sleep 8
echo "Now RUN"
echo 1 > /sys/class/gpio/gpio45/value
echo 0 > /sys/class/gpio/gpio13/value
echo 0 > /sys/class/gpio/gpio26/value
echo 0 > /sys/class/gpio/gpio23/value
sleep 8

echo "Now Read Status"
cat /sys/class/gpio/gpio22/value

#echo 1 > /dev/Pin33-12-nRESET
#echo 1 > /dev/Pin22-nRESET
#echo 1 > /dev/Pin44-USIM_DET
#echo 1 > /dev/Pin20-W_DIS
