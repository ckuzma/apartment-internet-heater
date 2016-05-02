/**
* Takes in a temperature reading and then sends
* it off to IBM Internet of Things Foundation.
*
* Connect VCC of the BMP180 sensor to 3.3V
* Connect GND of the BMP180 sensor to GND
* Connect SCL to D6 (NodeMCU) / GPIO 12 (ESP8266)
* Connect SDA to D7 (NodeMCU) / GPIO 13 (ESP8266
*
* Based loosely on an example program by Ant Elder,
* whose original program carried the Apache License v2
**/

#include <Adafruit_BMP085.h> // This is the modified version!
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <Wire.h>

/* Define the onboard LED in case we want to flash it */
//#define LED D0

/* Values will want (temp till we can push from IoTF) */
boolean HEATER_ON = true;
float DESIRED_TEMP = 21.65;
int ON_POSITION = 153;
int OFF_POSITION = 122;
int SERVO_PIN = 5; // D1 (NodeMCU) / GPIO 5 (ESP8266)

/* ----- FILL IN CUSTOM VALUES BELOW ----- */
const char* ssid = "<WIFI SSID>";
const char* password = "<WIFI PASSWORD>";
#define ORG "<IBM IoTF Organization>"
#define DEVICE_TYPE "NodeMCU" // Can change to whatever
#define DEVICE_ID "<UNIQUE DEVICE ID>"
#define TOKEN "<IBM IoTF Device Token>"
/* ----- FILL IN CUSTOM VALUES ABOVE ----- */

/* Function prototypes: */
boolean toggleHeater(float, float, boolean);
void turnHeaterOn();
void turnHeaterOff();

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClient wifiClient;
PubSubClient client(server, 1883, NULL, wifiClient);

Adafruit_BMP085 bmp; // Get the BMP180 up and going
Servo myservo; // Get the servo up and going

void setup() {
  delay(500);
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
  Serial.println();
  Serial.print("Connecting to ");
  // Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected, IP address: ");
  // Serial.println(WiFi.localIP());

  /* Set the onboard LED as an output device */
  // pinMode(LED, OUTPUT);
  // digitalWrite(LED, HIGH); // LED high/low is inverted. (This turns it off.)

  /* Start up the servo */
  myservo.attach(SERVO_PIN);
}

/* Initialize the temperature value here */
float cTemp = 0.0;
boolean heaterOn = false;

void loop() {

  /* Connect to WiFi and print out the status */
  if (!!!client.connected()) {
    Serial.print("Reconnecting client to ");
    Serial.println(server);
    while (!!!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    Serial.println();
  }

  /* Get the temperature */
  cTemp = bmp.readTemperature();
  heaterOn = toggleHeater(DESIRED_TEMP, cTemp, HEATER_ON);

  /* Assemble the data payload */
  String payload = "{\"d\":{\"cTemp\":";
  payload += cTemp;
  payload += ",\"active\":";
  payload += heaterOn;
  payload += "}}";

  /* Print out the assembled payload */
  // Serial.print("Sending payload: ");
  // Serial.println(payload);

  /* Send the payload. Uncomment prints for debugging. */
  if (client.publish(topic, (char*) payload.c_str())) {
    // Serial.println("Publish ok");
  } else {
    // Serial.println("Publish failed");
  }

  /* Delay before repeating the process */
  delay(60000); // 1000 == one second
}

boolean toggleHeater(float tempDesired, float tempActual, boolean heat) {
  if (heat == true) { // If we are heating the room...
    if (tempActual < tempDesired) {
      // Turn on the heater
      turnHeaterOn();
      return true;
    }
    else {
      turnHeaterOff();
      return false;
    }
  }
  else { // If we are cooling the room...
    if (tempActual > tempDesired) {
      // Turn on the A/C
      turnHeaterOn();
      return true;
    }
    else {
      turnHeaterOff();
      return false;
    }
  }
}

void turnHeaterOn() {
  myservo.write(ON_POSITION);
}

void turnHeaterOff() {
  myservo.write(OFF_POSITION);
}
