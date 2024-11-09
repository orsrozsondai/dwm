#!/usr/bin/env bash

case $1 in
	"up") 
		pactl set-sink-volume @DEFAULT_SINK@ +5%
		refresh_status.sh;;
	"down")
		pactl set-sink-volume @DEFAULT_SINK@ -5%
		refresh_status.sh;;
	"mute")
		pactl set-sink-mute @DEFAULT_SINK@ toggle
		refresh_status.sh;;
esac
