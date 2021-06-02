// 버튼 pin
const int BUTTON_PIN = 3;

void wakeUp(){
  Serial.println("wake up");
  // wifi 연결 후 api 보내기
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), wakeUp, LOW); // 버튼 인터럽트 설정
  
}
