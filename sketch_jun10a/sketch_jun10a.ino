#include <LWiFi.h>
#include <WiFiClient.h>
#include "MCS.h"


#include <Ultrasonic.h>
#define TRIGGER_PIN  12
#define ECHO_PIN     13
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
// Assign AP ssid / password here
#define _SSID "AA202_2G"
#define _KEY  "iteaa202"

// Assign device id / key of your test device
MCSDevice mcs("DypR0ItS", "er0MUjDl8Jbqw3gY");

// Assign channel id
// The test device should have 2 channel
// the first channel should be "Controller" - "On/Off"
// the secord channel should be "Display" - "On/Off"

MCSDisplayFloat  ulcm("ulcm");
MCSDisplayFloat  ulin("ulin");


void setup() {
  // setup Serial output at 9600
  Serial.begin(9600);


  // setup LED/Button pin


  // setup Wifi connection
  while (WL_CONNECTED != WiFi.status())
  {
    Serial.print("WiFi.begin(");
    Serial.print(_SSID);
    Serial.print(",");
    Serial.print(_KEY);
    Serial.println(")...");
    WiFi.begin(_SSID, _KEY);
  }
  Serial.println("WiFi connected !!");

  // setup MCS connection
 
  mcs.addChannel(ulcm);
  mcs.addChannel(ulin);

  while (!mcs.connected())
  {
    Serial.println("MCS.connect()...");
    mcs.connect();
  }
  Serial.println("MCS connected !!");

  // read LED value from MCS server

}

void loop() {
  // call process() to allow background processing, add timeout to avoid high cpu usage
  Serial.print("process(");
  Serial.print(millis());
  Serial.println(")");
  mcs.process(100);

  // updated flag will be cleared in process(), user must check it after process() call.
 
   float cmMsec, inMsec;
  long microsec = ultrasonic.timing();

  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
  Serial.print("MS: ");
  Serial.print(microsec);
  Serial.print(", CM: ");
  Serial.print(cmMsec);
  Serial.print(", IN: ");
  Serial.println(inMsec);
  delay(1000);

 if (!ulcm.set(cmMsec))
    {
      Serial.print("Failed to update humidity");
      Serial.println(ulcm.value());
    }
 if (!ulin.set(inMsec))
    {
      Serial.print("Failed to update temperature");
      Serial.println(ulin.value());
    }
 delay(500);
  // check if need to re-connect
  while (!mcs.connected())
  {
    Serial.println("re-connect to MCS...");
    mcs.connect();
    if (mcs.connected())
      Serial.println("MCS connected !!");
  }
}
