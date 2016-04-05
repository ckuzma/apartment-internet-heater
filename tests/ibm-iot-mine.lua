----------------------------------------

orgID = "quickstart" -- IoT Foundation organization ID <==== "quickstart" for demo
broker = orgID..".messaging.internetofthings.ibmcloud.com" -- IP or hostname of IoTF service
mqttPort = 1883 -- MQTT port (default 1883: non-secure)
userID = "" -- blank for quickstart
userPWD = "" -- blank for quickstart
macID = "blah" -- unique Device ID or Ethernet Mac Address <==== Modify this!
deviceType = "esp8266" -- "esp8266" for quickstart
clientID = "d:"..orgID..":"..deviceType..":"..macID -- Client ID
count = 0 -- Test number of mqtt_do cycles
mqttState = 0 -- State control

topic = "iot-2/evt/status/fmt/json"

t0 = tmr.time()

-- Wifi credentials
SSID = "Festnetz-2.4" -- <========== Modify this!
wifiPWD = "reutlingen" -- <========== Modify this!


function wifi_connect()
 wifi.setmode(wifi.STATION)
 wifi.sta.config(SSID,wifiPWD)
 wifi.sta.connect()
end


function mqtt_do()
 count = count + 1 -- tmr.alarm counter

 if mqttState < 5 then
 mqttState = wifi.sta.status() --State: Waiting for wifi
 wifi_connect()

 elseif mqttState == 5 then
 print("Starting to connect...")
 m = mqtt.Client(clientID, 120, userID, userPWD)

 m:on("offline", function(conn)
 print ("Checking IoTF server...")
 mqttState = 0 -- Starting all over again
 end)

 m:connect(broker , mqttPort, 0,
 function(conn)
 print("Connected to " .. broker .. ":" .. mqttPort)
 mqttState = 20 -- Go to publish state
 end)

 elseif mqttState == 20 then
 mqttState = 25 -- Publishing...
 t1 = tmr.time() - t0
 if t1 > 100 then
 t1 = 0
 t0 = tmr.time()
 end
 -- t1 is used as emulated sensor data, real application will use GPIOx to sense external sensor

 m:publish(topic ,'{"d": {"data":'..t1..'}}', 0, 0,

 function(conn)
 -- Print confirmation of data published
 print("Sent message #"..count.." data:"..t1)
 mqttState = 20 -- Finished publishing - go back to publish state.
 end)

 else print("Waiting..."..mqttState)
 mqttState = mqttState - 1 -- takes us gradually back to publish state to retry
 end

end

tmr.alarm(2, 10000, 1, function() mqtt_do() end) -- send data every 10s
