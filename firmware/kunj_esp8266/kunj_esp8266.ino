#include "RTClib.h"
#include <Wire.h>
#include "WebServer.h"
#include "wifi_credentials.h"
#include "Board.h"
#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <TimeAlarms.h>

#define WATERING_DURATION_S 30

RTC_DS3231 rtc;
static const char *ssid = APSSID;
static const char *password = APPSK;

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
  digitalWrite(LED_BUILTIN, LOW);

  // Serial port for debugging purposes
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  digitalWrite(LED_BUILTIN, HIGH);


  // Connect to Wi-Fi
  int wifi_conn_timeout = 180;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    if(--wifi_conn_timeout == 0) break;
  }
  if(wifi_conn_timeout == 0) {
    Serial.println("Connecting to WiFi failed!!!");
  } else {
    // Print ESP Local IP Address
    Serial.println(WiFi.localIP());
    digitalWrite(LED_BUILTIN, LOW);
    // put your setup code here, to run once:
    web_server_init(&web_server_cb_handler);
  }

  DateTime now = rtc.now();
  setTime(now.unixtime());

  Alarm.alarmRepeat(0,0,0, sync_with_rtc_time);
  // Alarm.alarmRepeat(7,0,0, start_drip_irrigation);    //Morning irrigation
  Alarm.alarmRepeat(17,0,0, start_drip_irrigation);   //Evening irrigation
}

void loop() 
{
  // put your main code here, to run repeatedly:
  // digitalClockDisplay();
  Alarm.delay(0); // wait one second between clock display
}

void sync_with_rtc_time(void)
{
  DateTime now = rtc.now();
  setTime(now.unixtime());
}

void start_drip_irrigation(void)
{
  digitalWrite(VALVE_CTRL_PIN, HIGH);
  Serial.println("Starting drip irrigation");
  Alarm.timerOnce(WATERING_DURATION_S, stop_drip_irrigation);  
}

void stop_drip_irrigation(void)
{
  digitalWrite(VALVE_CTRL_PIN, LOW);
  Serial.println("Stopped drip irrigation");
}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}

void printDigits(int digits)
{
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}