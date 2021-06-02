// 적외선 센서 pin
const int ULTRA_TRIGGER_PIN = 13;
const int ULTRA_ECHO_PIN = 12;
const int ULTRA_TRIGGER_PIN2 = 10;
const int ULTRA_ECHO_PIN2 = 9;

/* 개수 체크 */
long get_distance_mm(int TRIGGER_PIN, int ECHO_PIN) { // 생리대 개수 체크
  long duration = 0;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH); //측정 시작
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);  //측정 끝
  
  duration = pulseIn(ECHO_PIN, HIGH); // 측정된 거리 받아오기
  Serial.println("Duration");
  Serial.println(duration);
  return duration / 2;
  // 최대 duration이 13000 나옴
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ULTRA_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRA_ECHO_PIN, INPUT);
  pinMode(ULTRA_TRIGGER_PIN2, OUTPUT);
  pinMode(ULTRA_ECHO_PIN2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  long m = get_distance_mm(ULTRA_TRIGGER_PIN, ULTRA_ECHO_PIN);
  Serial.print("1층 mm : ");
  Serial.println(m); // mm
  m = get_distance_mm(ULTRA_TRIGGER_PIN2, ULTRA_ECHO_PIN2);
  Serial.print("2층 mm : ");
  Serial.println(m);
  delay(2000);
}
