#include <ESP8266WiFi.h>
#include <Servo.h>

const char* ssid = "<YOUR_WIFI_SSID>";
const char* password = "<YOUR_WIFI_PASSWORD>";
const char* host = "<YOUR_APP>.mybluemix.net";

Servo servo;  // Instantiate a servo object

void setup() {
  Serial.begin(9600);
  servo.attach(5);

  //Serial.println("Turning heater OFF");
  servo.write(122); // Off

  delay(100);

  // We start by connecting to a WiFi network

  //Serial.println();
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }

  //Serial.println("");
  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(10000);
  ++value;

  //Serial.print("connecting to ");
  //Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    //Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/heater-toggle/arduino";
  //Serial.print("Requesting URL: ");
  //Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(500);

  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    char start = client.read();
    if (start == '{'){
      String line = client.readStringUntil('}');
      ////Serial.print(line);
      if (line == "off"){
        //Serial.println("Turning heater OFF");
        servo.write(122); // Off
      }
      if (line == "on"){
        //Serial.println("Turning heater ON");
        servo.write(153); // On
      }
    }
  }

  //Serial.println();
  //Serial.println("closing connection");
}
