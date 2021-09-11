#include <SPI.h>
#include <WiFiNINA.h>

#include "measure.h"

char server[] = "api.uoswing.kro.kr";    // name address for Google (using DNS)
const int port = 8080; // 서버 접속 포트
String X_AUTH_TOKEN = ""; // 생리대함 인증 토큰
int PADBOX_ID = 2; // 생리대함 id

WiFiClient client;

void printResponse() {
  // Response 출력
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  
  client.flush(); // flush 해주어야 소켓 리소스가 반환됨
  client.stop();
}

void getToken(){
  delay(4000); // 응답이 올 때까지 기다리기
  
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    client.flush();
    client.stop();
    return;
  }
  
  String response = client.readString();
  int index = response.indexOf("HTTP/1.1");
  
  X_AUTH_TOKEN = response.substring(0, index);
}

void login(){ // 로그인
  String loginData="{\"email\":\"admin@gmail.com\",\"password\":\"33wing\"}";
  
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("POST /api/v1/admin/login HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(loginData.length());
    client.println(); // 반드시 헤더와 바디 분리
    
    client.println(loginData);
    client.println("Connection: close");
    client.println();
  }else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
  
  getToken();
  Serial.println(X_AUTH_TOKEN);
}

void sendData() { // 온습도, 수량 데이터 서버로 전송
  // HTTP patch request
  Serial.println("Starting Sending data to server...");
  long padData = get_pad_number(0) + get_pad_number(1);
  
  String requestData = "{\"humidity\":"+String(0)+",\"temperature\":"+String(0)+",\"padAmount\":"+String(padData)+"}";
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
  printResponse();
}
