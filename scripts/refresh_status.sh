#!/usr/bin/env bash

statuscmd () {

        # Media
	SONG=$(playerctl -i brave,plasma-browser-integration metadata -f '󰝚 {{artist}} - {{title}} ' 2>/dev/null)
	TITLE_LEN=$(echo "$SONG" | wc -c)
	if [ $TITLE_LEN -gt 1 ]; then
		if [ $TITLE_LEN -gt 51 ]; then
			printf "$SONG" | cut -c 1-50 | tr '\n' '.'
			echo ".."
		else
			printf "$SONG"
		fi
		echo '|'
	fi

        # Kernel
        echo ''
        uname -r
        echo '|'

		# IP address
		echo '󰖩'
		IP=$(ip a show dev wlo1 | awk '/inet / {print $2}' | sed 's/\/..//')
		if [ -z $IP ]; then
			echo disconnected
		else
			echo $IP
		fi

		echo '|'

	# Cpu temp
	echo ''
	cat /sys/class/thermal/thermal_zone7/temp | sed 's/\(.\)..$/°C/'
	echo '|'

        # Memory usage
        echo ''
        free -h | awk '/Mem/ {print $3}' | sed 's/i//'
        echo '|'

        # Volume
        VOL=$(pactl get-sink-volume @DEFAULT_SINK@ | awk '{print $5}')
        MUTE=$(pactl get-sink-mute @DEFAULT_SINK@ | awk '{print $2}')

        if [ $MUTE == "yes" ]
        then
        echo 󰖁 $VOL
        elif [ $VOL == "0%" ]
        then
        echo 󰕿 $VOL
        else
        echo 󰕾 $VOL
        fi
        echo '|'

        # Battery
        BAT=$(upower -i /org/freedesktop/UPower/devices/battery_BAT0 | grep percentage: | awk '{print $2}')
		BAT_VALUE=${BAT%?}
        if [ $(upower -i /org/freedesktop/UPower/devices/battery_BAT0 | awk '/state:/ {print $2}') == "discharging" ]
        then
			case $BAT_VALUE in
		        [89]* | 100) echo "  $BAT";;
		        [67]* ) echo "  $BAT";;
		        [45]* ) echo "  $BAT";;
		        [23]* ) echo "  $BAT";;
		        *) echo "  $BAT";;
   	 		esac
        else
            echo 󱐋 $BAT

        fi

    	echo '|'

        # Date and time
        date '+ %B %-d, %-H:%M'
}
STATUS=$(statuscmd | tr '\n' ' ')

xsetroot -name "$STATUS"
