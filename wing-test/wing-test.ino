#include <stdio.h>
#include <DHT.h>
#include <WiFiEsp.h>

const int DHT11_PIN = 2; // 온습도계 꽂은 pin으로 바꿀 것

typedef struct {
  float hum;
  float temp;
} ht;

const int ULTRA_TRIGGER_PIN = 8;
const int ULTRA_ECHO_PIN = 9;

const int ESP_TX = 4;
const int ESP_RX = 3;
#ifndef HAVE_HWSERIAL 1
#endif
#include <SoftwareSerial.h>
SoftwareSerial wifiSerial(ESP_TX, ESP_RX);

char ssid[] = "SSID";    //ssid 입력
char pass[] = "PASSWORD"; //비밀번호 입력
int status = WL_IDLE_STATUS;

const char SERVER[] = "api.wing-test.kro.kr";
const int PORT = 8080;
const char SERVER_TEST[] = "arduino.cc";
const int PORT_TEST = 80;
WiFiEspClient client;

String X_AUTH_TOKEN = "";
int PADBOX_ID = 2; // 생리대함 id
int PAD_WIDTH = 2.9; // 생리대 너비

//온습도 체크
ht get_hum_and_temp() {
  DHT dht = DHT(DHT11_PIN, DHT11, 1);
  ht rtn;
  rtn.hum = dht.readHumidity(false);            //(force mode)
  rtn.temp = dht.readTemperature(false, false); //(섭씨, force mode)
  return rtn;
}

//거리 체크
long get_distance_mm() {
  long duration = 0;
  digitalWrite(ULTRA_TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRA_TRIGGER_PIN, HIGH); //측정 시작
  delayMicroseconds(10);
  digitalWrite(ULTRA_TRIGGER_PIN, LOW);  //측정 끝

  duration = pulseIn(ULTRA_ECHO_PIN, HIGH); //측정된 거리 받아오기
  
  long distance_mm;
  distance_mm = ((duration / PAD_WIDTH) / 2);
  return distance_mm;
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

void connect_wifi() {
    //WiFi network 연결
  while(status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    //WPA/WPA2 네트워크 연결
    status = WiFi.begin(ssid, pass);
  }
}

void login(const char* server, const char* port) {
  Serial.println("Starting login...");
  String loginData="{\"email\":\"samsam-uos@gmail.com\",\"password\":\"33wing\"}";
 
  if(client.connect(server, port)){
    Serial.println("Connected to server");
    client.print("POST /api/v1/admin/login");
    client.println(" HTTP/1.1");

    client.print("Host: ");
    client.println(server);
    client.println("Accept: */*");
    client.println("Connection: close");
    client.println("Content-Type: application/json");

    client.print("Content-Length: ");
    client.println(loginData.length());
    client.println();
    client.println(loginData);
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    client.stop();
    return;
  }

  String token = "";
  while (client.available()) {
     token = client.readString();
  }
  X_AUTH_TOKEN = token;
}

void sendData(const char* server, const char* port) {
  // HTTP patch request
  Serial.println("Starting Sending data to server...");
  ht htData = get_hum_and_temp();
  long padData = get_distance_mm();
  
  String requestData = "{\"humidity\":"+String(htData.hum)+",\"padAmount\":"+String(htData.temp)+",\"temperature\":"+String(padData)+"}";
  Serial.println(requestData);
  
  if(client.connect(server, port)){
    Serial.println("Connected to server");
    client.print("PATCH /api/v1/padbox/updateState/"+String(PADBOX_ID));
    client.println(" HTTP/1.1");
    
    client.print("Host: ");
    client.println(server);
    client.println("Accept: */*");
    client.println("Connection: keep-alive");
    client.print("X-AUTH-TOKEN: ");
    client.println(X_AUTH_TOKEN);
    client.println("Content-Type: application/json");

    client.print("Content-Length: ");
    client.println(requestData.length());
    client.println(); // 반드시 헤더와 바디 분리
    client.println(requestData);
  }
  
}

void http_request(const char* server, const char* port, const char* method) {
  // HTTP request
  Serial.println("Starting connection to server...");
  if (client.connect(server, port)) {
    Serial.println("Connected to server");
    // HTTP request
    client.print(method);
    client.println(" HTTP/1.1");  // method 서술
    
    client.print("Host: ");
    client.println(server);       // http request 요구사항 (없으면 Bad Request)
    client.println("Connection: close");
    client.println();             // request 마무리
  }
}

void setup() {
  pinMode(ULTRA_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRA_ECHO_PIN, INPUT);
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
  Serial.println(X_AUTH_TOKEN);
}

void loop() {
  //HTTP Request
  sendData(SERVER, PORT);
  
  //Response 출력
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  
  // 서버와 연결이 끊긴 경우
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting from server...");
    
    client.stop();
  }

  Serial.println("end of loop");
  delay(10000);
}
