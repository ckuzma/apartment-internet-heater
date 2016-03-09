import Adafruit_BMP.BMP085 as BMP085

sensor = BMP085.BMP085()
temp = sensor.read_temperature()
temp = (temp * 9) / 5 + 32

print(temp)
