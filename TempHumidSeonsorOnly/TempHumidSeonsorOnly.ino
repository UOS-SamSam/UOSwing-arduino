#include <dht.h> // 온습도 라이브러리

// 온습도 센서 pin
const int DHT11_PIN = A1;

// 온습도
dht DHT;

typedef struct {
  float hum;
  float temp;
} ht;

/* 온습도 체크 */
ht get_hum_and_temp() { // 온습도 체크
  int chk = DHT.read11(DHT11_PIN);
  ht rtn; 
  switch (chk)
  {
    case DHTLIB_OK:  
    Serial.print("OK,\t"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.print("Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.print("Time out error,\t"); 
    break;
    case DHTLIB_ERROR_CONNECT:
        Serial.print("Connect error,\t");
        break;
    case DHTLIB_ERROR_ACK_L:
        Serial.print("Ack Low error,\t");
        break;
    case DHTLIB_ERROR_ACK_H:
        Serial.print("Ack High error,\t");
        break;
    default: 
    Serial.print("Unknown error,\t"); 
    break;
  }
  // DISPLAY DATA
  
  rtn.hum = DHT.humidity;
  rtn.temp = DHT.temperature;
  Serial.println(DHT.humidity, 1);
  Serial.println(DHT.temperature, 1);
  return rtn;
}
/* 온습도 체크 */

void setup() {
  // put your setup code here, to run once:
  pinMode(DHT11_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  ht htData = get_hum_and_temp();
  delay(2000);
}
