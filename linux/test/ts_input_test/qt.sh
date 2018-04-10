#!/bin/sh
base1=qpe
pid=`/bin/pidof $base1`
if [ -n "$pid" ]; then
    killall $base1
fi

base2=qss
pid=`/bin/pidof $base2`
if [ -n "$pid" ]; then
    killall $base2
fi

base3=quicklauncher
pid=`/bin/pidof $base3`
if [ -n "$pid" ]; then
    killall  $base3
fi


base4=fluidlauncher
pid=`/bin/pidof $base4`
if [ -n "$pid" ]; then
    killall  $base4
fi

awk -F "laohua=" '{print $2}' /proc/cmdline > /etc/laohua
LAOHUA=`cat /etc/laohua`

if [ $LAOHUA = "Y" ]
then
/forlinx/bin/mplayer -fs -loop 0 /forlinx/video/xm.mp4
else
export KEYPAD_DEV=/dev/input/keypad

#tslib config
export TSLIB_TSDEVICE=/dev/input/touchscreen0
export TSLIB_CONFFILE=/etc/ts.conf
export TSLIB_PLUGINDIR=/usr/lib/ts
export POINTERCAL_FILE=/etc/pointercal
export TSLIB_CALIBFILE=/etc/pointercal
export TSLIB_CONSOLEDEVICE=none
export TSLIB_FBDEVICE=/dev/fb0

FB_SIZE=$(cat /sys/class/graphics/fb0/virtual_size)

# qteverywhere config
export QTDIR=/forlinx/qt
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
export QT_QWS_FONTDIR=$QTDIR/lib/fonts
#if [ $FB_SIZE = "800,1200"  ]
#then
#export QWS_SIZE=800x600
#else
#export QWS_SIZE=800x480
#fi
if [ $FB_SIZE = "800,1200" ]; then
export QWS_SIZE=800x600
elif [ $FB_SIZE = "1280,1440" ]; then
export QWS_SIZE=1280x720
elif [ $FB_SIZE = "1024,1536" ]; then
export QWS_SIZE=1024x768
else
export QWS_SIZE=800x480
fi
export QWS_DISPLAY=LinuxFb:/dev/fb0

TYPE=`cat /etc/t2m`

if [ $TYPE = "T" ]
then
export QWS_MOUSE_PROTO="Tslib:/dev/input/touchscreen0"
else
export QWS_MOUSE_PROTO="Intellimouse:/dev/input/mice"
fi

#export QWS_KEYBOARD="USB:/dev/input/usbkbd"

awk -F "calibrate=| laohua" '{print $2}' /proc/cmdline > /etc/cali

CALI=`cat /etc/cali`

if [ $CALI = "Y" ]
then
if [ ! -e /etc/pointercal ] ; then
        /usr/bin/ts_calibrate
        sync
fi
fi


export HOME=/forlinx/qt

cd /forlinx/web/lighttpd/sbin
./lighttpd -f ../config/lighttpd.conf

export QT_QWS_FONTDIR=/forlinx/qt/lib/fonts/ 

#udhcpc date

ifconfig eth0 178.100.254.249

cd /home/forlinx
./PadQt -qws 2>/dev/null &

#cd /forlinx/qt/bin
#./matrix_gui -qws 2>/dev/null &
fi