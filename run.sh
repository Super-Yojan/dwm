#!/bin/bash

Xephyr -br -ac -reset -screen 1920x1080 :1 &
sleep 1s 
export DISPLAY=:1 
exec pasystray &
exec nm-applet &
exec /usr/lib/kdeconnectd &
exec kdeconnect-indicator &
exec ~/Documents/dwm-bar/dwm_bar.sh &
./dwm &
~/.fehbg & 
