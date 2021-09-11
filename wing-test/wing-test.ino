#include "wifi.h"
#include "api_request.h"
#include "low_power.h"

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  measureSetUP();
  lowPowerSetUP();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  setupWifi();
  login();
}

void loop(){
  // lowpower 적용해야함
  sendData();
  delay(30000);
}
