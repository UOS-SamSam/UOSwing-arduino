#include <DHT.h>
#include <WiFiEsp.h>

const int DHT11_PIN = 2;

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

char ssid[] = "WIFI-SSID";  //SSID 입력
char pass[] = "WIFI-PW";    //PW 입력
int status = WL_IDLE_STATUS;

const char SERVER[] = "api.wing-test.kro.kr";
const int PORT = 8080;
const char SERVER_TEST[] = "arduino.cc";
const int PORT_TEST = 80;
WiFiEspClient client;


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
  distance_mm = ((duration / 2.9) / 2);
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
}

void loop() {
  //HTTP Request
  http_request(SERVER, PORT, "GET /api/v1/padbox");
  
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
  delay(10000);
}
