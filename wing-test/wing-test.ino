#include <RocketScream_LowPowerAVRZero.h> // AVR0용 저전력 라이브러리
#include <RocketScream_RTCAVRZero.h> // AVR0용 타이머 라이브러리

#include "wifi.h"
#include "api_request.h"

const uint8_t unusedPins[] = {0, 1, 2, 4, 5, 6, 7, 8, 11}; // 저전력을 위해 안쓰는 핀 설정
volatile bool awake = false; // 타이머 플래그
int alarmSec = 3600; // 타이머 시간
// 버튼 pin
const int BUTTON_PIN = 3;

void wakeUp(void)
{
  Serial.println("wake up");
  awake = true;
  RTCAVRZero.disableAlarm();
  RTCAVRZero.detachInterrupt();
}

void lowPowerSetUP() {

  uint8_t index;
  /* Ensure unused pins are not floating */
  for (index = 0; index < sizeof(unusedPins); index++)
  {
    pinMode(unusedPins[index], INPUT_PULLUP);
    LowPower.disablePinISC(unusedPins[index]);
  }
  
  pinMode(BUTTON_PIN, INPUT_PULLUP); // button

  attachInterrupt(BUTTON_PIN, wakeUp, LOW);
  RTCAVRZero.begin(false);
  // Time in seconds, true: repeat, false: once
  RTCAVRZero.enableAlarm(alarmSec, true);
  RTCAVRZero.attachInterrupt(wakeUp);
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  measureSetUP();
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  setupWifi();
  lowPowerSetUP();
  
}

void loop(){
  Serial.println("sleep");
  LowPower.standby();
  if(awake){
    login();
    sendData();
    awake = false;
    Serial.println("reset Alarm");
    delay(200);
    RTCAVRZero.enableAlarm(alarmSec, true);
    RTCAVRZero.attachInterrupt(wakeUp);
  }

}
