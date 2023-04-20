#include "RTClib.h"
#include <Wire.h>
#include "WebServer.h"
#include "wifi_credentials.h"
#include "Board.h"
#include <ESP8266WiFi.h>


static void web_server_cb_handler(bool pin_state)
{
  digitalWrite(VALVE_CTRL_PIN, pin_state);
}

void setup() 
{
  //Configure the IO
  pinMode(VALVE_CTRL_PIN, OUTPUT);
  digitalWrite(VALVE_CTRL_PIN, LOW);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, LOW);


  // put your setup code here, to run once:
  web_server_init(web_server_cb_handler);
}

void loop() 
{
  // put your main code here, to run repeatedly:

}
