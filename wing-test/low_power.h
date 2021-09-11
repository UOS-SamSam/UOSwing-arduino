#include <RocketScream_LowPowerAVRZero.h> // AVR0용 저전력 라이브러리
#include <RocketScream_RTCAVRZero.h> // AVR0용 타이머 라이브러리

// 저전력용
const uint8_t unusedPins[] = {0, 1, 2, 4, 5, 6, 7, 8, 11}; // 저전력을 위해 안쓰는 핀 설정
volatile bool awake = false; // 타이머 플래그
int alarmSec = 5; // 타이머 시간

// 버튼 pin
const int BUTTON_PIN = 3;

void wakeUp(){
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
  /* Time in seconds, true: repeat, false: once */
  RTCAVRZero.enableAlarm(alarmSec, true);
  RTCAVRZero.attachInterrupt(wakeUp);
}
