#!/usr/bin/env bash

case $1 in
	"play") playerctl -i brave,plasma-browser-integration play-pause;;
	"next") playerctl -i brave,plasma-browser-integration next
		sleep 0.1;;
	"prev") playerctl -i brave,plasma-browser-integration previous
		sleep 0.1;;
	
esac

refresh_status.sh
