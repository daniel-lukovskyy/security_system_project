This is a small prototipe of security system based on ESP32 microcontroller, IR, PIR, brightness sensors and display.
The main purpose of the system is to track unpleasant movements during the night or sleeping hours.
You can adjust working hours by yourself. Meanwhile it works from 22:00 till 07:00.
During working hours you'll see on display ammount of interruptions.
There 2 kinds of interruptions: light interruption detects how many times the light was turned on.
Regular interruption shows the only movement. There are different combinations of data that allows to detect the movement. Brightness sensor works in team with other sensors (not only counts the lights that were turned on)
During not working hours the display shows that this is not working hours and shows the current time (setted on GMT +3), you can adjust it.
In order to use the code you have to chenge the wifi data to you own (SSID, PASSWORD).
Time management based on NTP.

NOTICE ! : There is a function "center_text" , she works with every display, so you can be sure that there will not be troubles with displays.

NOTICE ! : You will have to manually adjust the potentiometers on the sensors to get the correct scanning distance.

NOTICE ! : You can also unmute get_sensprs_data in order to show sensors data on display.
