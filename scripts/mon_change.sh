#!/usr/bin/env bash

intern=eDP-1
extern=HDMI-1

if xrandr | grep "$extern disconnected"; then
	notify-send "HDMI disconnected"
	exit 0
fi

choice=$(printf "Expand\nOnly External\nOnly Internal\nDuplicate" | dmenu )

if [[ -z $choice ]]; then
	exit 0
fi

case $choice in
	"Expand") xrandr --output "$intern" --primary --auto --output "$extern" --right-of "$intern" --auto;;
	"Only Internal") xrandr --output "$extern" --off --output "$intern" --auto;;
	"Only External") xrandr --output "$intern" --off --output "$extern" --auto;;
	"Duplicate") xrandr --output "$intern" --auto --output "$extern" --same-as "$intern" --auto;;
esac
