# 버튼 모듈 연결

* OUT => 3 pin
* VCC => 5V
* GND => GND

```c
void setup() {
  Serial.begin(9600);   // 시리얼 통신을 시작, 통신속도는 (9600)
  pinMode(2, INPUT); // 디지털 3번 핀을 입력모드로 설정
}

void loop() {
  int a = digitalRead(2);  // 정수형 변수 a를 생성한 후 디지털 3번핀에 입력되는 신호를 입력
  if (a == HIGH) {     // a가 HIGH (1)이면 (버튼을 누르지 않으면)
    Serial.println("Do not press");  // 시리얼 모니터에 Do not press을 입력)
    delay(2000);   // 2초간 대기

  }
  else {    // 그게 아니라면 (a가 HIGH가 아니라면) (버튼을 누르면)
    Serial.println("Press down");                                      // 시리얼 모니터에 Press down을 입력
    delay(2000);  // 2초간 대기
  }
}
```

참고 : https://m.blog.naver.com/PostView.nhn?blogId=roboholic84&logNo=221014246406&proxyReferer=https:%2F%2Fwww.google.com%2F



## 버튼 이벤트가 한 번만 발생하게 하기

```c
attachInterrupt(0, wakeUp, ROW);
=> attachInterrupt(0, wakeUp, RISING);
```

attachInterrupt(인터럽트번호, 핸들러, 모드)

모드는 언제 트리거 되어야하는 지를 의미하며 다음 5가지 모드가 있다.

* LOW : pin이 low일 때
* CHANGE : pin이 바뀔 때
* RISING : pin이 low에서 high로 바뀔 때
* FALLING : pin이 high에서 low로 바뀔 때
* HIGH : pin이 high일 때 



# 슬립모드 깨우기

* LowPower.h를 추가한다.
  * https://github.com/rocketscream/Low-Power 에서 zip 파일로 다운받아 "스케치 => 라이브러리 포함하기 => .ZIP 라이브러리 추가"로 추가하기

* 아두이노는 **2, 3번 핀이 인터럽트 기능을 사용할 수 있으며, 각각 인터럽트 번호가 0과 1이라는 점**을 기억하자.
  * 따라서 버튼은 2번 또는 3번 핀을 사용해야한다. (코드는 2번 핀 사용)

```c
#include <LowPower.h>
 
const int wakeUpPin = 2;  //pin of interrupt 0
 
void setup() {
  Serial.begin(9600);   // 시리얼 통신을 시작, 통신속도는 (9600)
  pinMode(wakeUpPin, INPUT);
  Serial.println("Start....");
}

void loop() {
  attachInterrupt(0, wakeUp, LOW);  // LOW, HIGH, RISING, FALLING, CHANGE
  // Low일 때 wakeUp이 trigger됨
  
  Serial.println("Sleep Start");
  for (int i = 0; i < 450; i++) { // 1시간에 한 번 호출
      // i<1로 변경하여 8초에 한 번 호출하는 지 확인해볼 수 있음
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); // 8초 * 450 = 3600초
  }
  detachInterrupt(0);
  // 여기서 api 호출해야함
  Serial.println("api 호출");
  delay(2000);
}
 
void wakeUp()
{
  Serial.println("Sleep End");
  delay(2000);
}
```



참고 : https://www.circuitbasics.com/lowering-arduino-power-requirements-using-sleep-mode/

https://twinw.tistory.com/124



최종 코드는 wing-test.ino 파일에 반영되어있다.