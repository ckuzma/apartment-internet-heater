# apartment-internet-heater
An IoT project aimed at controlling an apartment's built-in heating / cooling unit.

# Basic instructions before loading onto Bluemix / devices:
	BeagleBone_Local_App/automatic_temp_controller.py:
	- `BLUEMIX_ADDRESS = 'http://<YOUR_APP>.mybluemix.net/heater-toggle'`
	- Change it to the address of your Bluemix app
	NodeMCU_Arduino_Program/nodemcu_web_servo.ino:
	-Change these lines as appropriate:
			`const char* ssid = "<YOUR_WIFI_SSID>";`
			`const char* password = "<YOUR_WIFI_PASSWORD>";`
			`const char* host = "<YOUR_APP>.mybluemix.net";`

# Basic deployment:
	Bluemix app
	- Deploy the whole `heater-servo` app to Bluemix
	NodeMCU Program
	- Load the `nodemcu_web_servo.ino` onto the NodeMCU
	- Wire up the servo to 5V, GND, and D1 on the NodeMCU (GPIO5 on the ESP8266)
	- Power up
	Python App
	- On a local device, do this to run:
	- `python automatic_temp_controller.py 70.0` -- Turns on heater whenever below 70.0F
	- Modify the script to run in cooling mode
