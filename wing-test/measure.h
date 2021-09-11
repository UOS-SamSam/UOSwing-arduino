// 적외선 센서 pin
void measureSetUP(){
//  pinMode(ULTRA_TRIGGER_PIN, OUTPUT);
//  pinMode(ULTRA_ECHO_PIN, INPUT);
//  pinMode(ULTRA_TRIGGER_PIN2, OUTPUT);
//  pinMode(ULTRA_ECHO_PIN2, INPUT);
}

long get_distance_mm(int TRIGGER_PIN, int ECHO_PIN) { // 생리대 개수 체크
//  long duration = 0;
//  digitalWrite(TRIGGER_PIN, LOW);
//  delayMicroseconds(2);
//  digitalWrite(TRIGGER_PIN, HIGH); //측정 시작
//  delayMicroseconds(10);
//  digitalWrite(TRIGGER_PIN, LOW);  //측정 끝
//  
//  duration = pulseIn(ECHO_PIN, HIGH); // 측정된 거리 받아오기
//  return duration / 2;
  return 1;
}

int get_pad_number(int type){ // type이 0이면 위, 1이면 아래
//
//  long distance_mm;
//  if(type == 0){
//    distance_mm = get_distance_mm(ULTRA_TRIGGER_PIN, ULTRA_ECHO_PIN);
//  }else{
//    distance_mm = get_distance_mm(ULTRA_TRIGGER_PIN2, ULTRA_ECHO_PIN2);
//  }
//  int num;
//  
//  if(distance_mm > FULL_WIDTH) num = 0;
//  else num = (FULL_WIDTH - distance_mm) / PAD_WIDTH;
//
//  if(num>11){
//    num = 11;
//  }
//  return num;
return 1;
}
