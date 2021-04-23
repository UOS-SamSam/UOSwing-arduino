#include <stdio.h>
#include <DHT.h>
#include <WiFiEsp.h>
#include <LowPower.h>

typedef struct {
  float hum;
  float temp;
} ht;

// 온습도 센서 pin
const int DHT11_PIN = A1;

// 적외선 센서 pin
const int ULTRA_TRIGGER_PIN = 13;
const int ULTRA_ECHO_PIN = 12;

// 버튼 pin
const int BUTTON_PIN = 2;

// wifi 모듈 pin
const int ESP_TX = 4;
const int ESP_RX = 3;

#ifndef HAVE_HWSERIAL 1
#endif
#include <SoftwareSerial.h>
SoftwareSerial wifiSerial(ESP_TX, ESP_RX);

char ssid[] = "SSID";    // ssid 입력
char pass[] = "PASSWORD"; // 비밀번호 입력
int status = WL_IDLE_STATUS;

const char SERVER[] = "api.wing-test.kro.kr"; // 서버 도메인
const int PORT = 8080; // 서버 접속 포트
WiFiEspClient client;

String X_AUTH_TOKEN = ""; // 생리대함 인증 토큰
int PADBOX_ID = 2; // 생리대함 id
int PAD_WIDTH = 50; // 생리대 너비

ht get_hum_and_temp() { // 온습도 체크
  DHT dht = DHT(DHT11_PIN, DHT11, 1);
  ht rtn;
  rtn.hum = dht.readHumidity(false);            //(force mode)
  rtn.temp = dht.readTemperature(false, false); //(섭씨, force mode)
  return rtn;
}

long get_distance_mm() { //거리 체크
  long duration = 0;
  digitalWrite(ULTRA_TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRA_TRIGGER_PIN, HIGH); //측정 시작
  delayMicroseconds(10);
  digitalWrite(ULTRA_TRIGGER_PIN, LOW);  //측정 끝

  duration = pulseIn(ULTRA_ECHO_PIN, HIGH); //측정된 거리 받아오기
  
  long distance_mm;
  distance_mm = ((duration / PAD_WIDTH) / 2);

  // 개수를 return 하도록 수정 (희은)
  int num = (20 - distance_mm);
  if(num < 0) num = 0;
  if(num > 20) num = 20;
  return num;
}

void tell_to_esp() {
  if(wifiSerial.available()) {
    Serial.write(wifiSerial.read());
  }
  if(Serial.available()) {
    wifiSerial.write(Serial.read());
  }
}

void printWifiStatus(){
  // 연결된 WiFi의 ssid 출력
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // ip 주소 출력
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // 신호 세기 출력
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void connect_wifi() { // WiFi network 연결
  while(status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    //WPA/WPA2 네트워크 연결
    status = WiFi.begin(ssid, pass);
  }
}

void login(const char* server, const char* port) { // login 후 토큰 저장
  Serial.println("Starting login...");
  String loginData="{\"email\":\"samsam-uos@gmail.com\",\"password\":\"33wing\"}";
 
  if(client.connect(server, port)){
    Serial.println("Connected to server");
    client.print("POST /api/v1/admin/login");
    client.println(" HTTP/1.1");

    client.print("Host: ");
    client.println(server);
    client.println("Accept: */*");
    client.println("Connection: close"); // 반드시 이곳에 위치
    client.println("Content-Type: application/json");

    client.print("Content-Length: ");
    client.println(loginData.length());
    client.println(); // 반드시 헤더와 바디 분리
    client.println(loginData);
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    client.flush();
    client.stop();
    return;
  }

  String token = "";
  while (client.available()) {
     token = client.readString();
  }
  X_AUTH_TOKEN = token;
}

void sendData(const char* server, const char* port) { // 온습도, 수량 데이터 서버로 전송
  // HTTP patch request
  Serial.println("Starting Sending data to server...");
  ht htData = get_hum_and_temp();
  long padData = get_distance_mm();
  
  String requestData = "{\"humidity\":"+String(htData.hum)+",\"temperature\":"+String(htData.temp)+",\"padAmount\":"+String(padData)+"}";
  Serial.println(requestData);
  
  if(client.connect(server, port)){
    Serial.println("Connected to server");
    client.print("PATCH /api/v1/padbox/updateState/"+String(PADBOX_ID));
    client.println(" HTTP/1.1");
    
    client.print("Host: ");
    client.println(server);
    client.println("Accept: */*");
    client.println("Connection: keep-alive"); // 반드시 keep-alive
    client.print("X-AUTH-TOKEN: ");
    client.println(X_AUTH_TOKEN);
    client.println("Content-Type: application/json");

    client.print("Content-Length: ");
    client.println(requestData.length());
    client.println(); // 반드시 헤더와 바디 분리
    client.println(requestData);
  }
  
}

void setup() {
  pinMode(ULTRA_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRA_ECHO_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);
  
  Serial.begin(9600);
  wifiSerial.begin(9600);
  WiFi.init(&wifiSerial);

  //WiFi shield 작동 확인
  if(WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while(true);  //all stop
  }

  connect_wifi();

  //연결 확인
  Serial.print("You're connected to the network");
  printWifiStatus();
  Serial.println();
  
  // login
  login(SERVER, PORT);
  Serial.println(X_AUTH_TOKEN); // 토큰 출력
}

void wakeUp(){ // 버튼 interupt 시 trigger되는 함수 (딱히 하는 기능 없음)
  Serial.println("Button interrupt");
  delay(1000);
  apiRequest();
}

void apiRequest(){ // api 호출 관련 함수
  // HTTP Request
  sendData(SERVER, PORT);
  
  // Response 출력
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  
  client.flush(); // flush 해주어야 소켓 리소스가 반환됨
  client.stop();
}

void loop() {
//  attachInterrupt(0, wakeUp, FALLING); // Falling일 때 wakeUp이 trigger 됨 => 버튼이 눌리면 타이머 초기화
  for (int i = 0; i < 1; i++) { // 1시간에 한 번 호출
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_ON); // 8초 * 450 = 3600초
    // ADC_OFF, BOD_ON이어야 API 호출 성공함
  }
  apiRequest();
//  detachInterrupt(0);
}
