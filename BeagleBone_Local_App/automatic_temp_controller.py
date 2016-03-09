from datetime import datetime
import requests
import sys
import time
import Adafruit_BMP.BMP085 as BMP085
sensor = BMP085.BMP085()

BLUEMIX_ADDRESS = 'http://<YOUR_APP>.mybluemix.net/heater-toggle'

if len(sys.argv) != 2:
    print('You did not specify a temperature.')
    sys.exit()

TEMPERATURE = float(sys.argv[1])
print('Temperature setting: ' + str(TEMPERATURE))

current_command = None
params = {'command': None}
x = 0
while x < 1:
    temp = sensor.read_temperature()
    temp = (temp * 9) / 5 + 32
    if temp < TEMPERATURE:
        params['command'] = 'on'
    else:
        params['command'] = 'off'
    if params['command'] != current_command:
        requests.post(BLUEMIX_ADDRESS, json=params)
        current_command = params['command']
    print(datetime.now().strftime('%Y-%m-%d %H:%M:%S') + '  |  ' + str(temp) + " F  |  " + current_command)
    time.sleep(60)
