#!/usr/bin/env bash

CHOICE=$(printf "Shutdown\nReboot\nLock\nLog out" | dmenu )

if [[ -z $CHOICE ]]; then
	exit 0
fi

case $CHOICE in
	"Shutdown") shutdown now;;
	"Reboot") reboot;;
	"Lock") betterlockscreen -l blur;;
	"Log out") pkill --signal SIGKILL dwm;;
esac
