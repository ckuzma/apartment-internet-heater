-- Initialize BMP180
OSS = 1 -- oversampling setting (0-3)
SDA_PIN = 4 -- sda pin, GPIO2
SCL_PIN = 3 -- scl pin, GPIO0
bmp180 = require("bmp180")
bmp180.init(SDA_PIN, SCL_PIN)

-- Initialize Servo
pwm.setup(2, 50, 512)
pwm.start(2)

x = 0
while x < 15 do -- Change this later

  -- Read the temperature from the sensor, convert to Fahrenheit
  bmp180.read(OSS)
  t = bmp180.getTemperature()
  t = (9 * t / 50 + 32)
  print(t) --Useful for debugging

  -- Check if temperature is within acceptable bounds
  if t < 70 then
    pwm.setduty(2, 110) -- Turn on if colder than temp
  else
    pwm.setduty(2, 93) -- Tun off if hotter than temp
  end

  -- Delay a bit
  tmr.delay(60000000)
  
end

-- release module
bmp180 = nil
package.loaded["bmp180"]=nil
