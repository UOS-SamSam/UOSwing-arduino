
// 적외선 센서 pin
const int ULTRA_TRIGGER_PIN = 13;
const int ULTRA_ECHO_PIN = 12;
const int ULTRA_TRIGGER_PIN2 = 10;
const int ULTRA_ECHO_PIN2 = 9;

const int PAD_WIDTH = 130; // 하나당 차이로 갱신
const int FULL_WIDTH = 1600; // 하나 채워졌을 때 잰 거리로 갱신

void measureSetUP(){
  pinMode(ULTRA_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRA_ECHO_PIN, INPUT);
  pinMode(ULTRA_TRIGGER_PIN2, OUTPUT);
  pinMode(ULTRA_ECHO_PIN2, INPUT);
}

long get_distance_mm(int TRIGGER_PIN, int ECHO_PIN) { // 생리대 개수 체크
  long duration = 0;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH); //측정 시작
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);  //측정 끝
  
  duration = pulseIn(ECHO_PIN, HIGH); // 측정된 거리 받아오기
  return ((340*duration)/1000) / 2;
}

int get_pad_number(int type){ // type이 0이면 위, 1이면 아래
  long distance_mm;
  if(type == 0){
    distance_mm = get_distance_mm(ULTRA_TRIGGER_PIN, ULTRA_ECHO_PIN)-50;
  }else{
    distance_mm = get_distance_mm(ULTRA_TRIGGER_PIN2, ULTRA_ECHO_PIN2);
  }
  int num;


  if(distance_mm > 170){
    num = 0;
  }else if(distance_mm > 150){
    num = 4;
  }else if(distance_mm > 130){
    num = 5;
  }else if(distance_mm > 110){
    num = 6;
  }else if(distance_mm > 80){
    num = 7;
  }else if(distance_mm > 60){
    num = 8;
  }else if(distance_mm > 40){
    num = 9;
  }else if(distance_mm > 25){
    num = 10;
  }else{
    num = 11;
  }
  
  Serial.println(num);
  if(num>11){
    num = 11;
  }
  return num;
}
