# LowPower 라이브러리 분석

## 아두이노 정보 (우리꺼)

Arduino Uno (R3)

* Atmega328 기반



## 사용 라이브러리

* sleep.h
* wdt.h
* power.h
* interrupt.h
* LowPower.h



## LowPower.h

### LowPowerClass 정의

> 마이크로컨트롤러에 따라 class의 정의가 달라짐

```c
// 주어진 period가 SLEEP_FOREVER가 아니면 wdt_enable(peroid)
if (period != SLEEP_FOREVER)
{
    wdt_enable(period);
    WDTCSR |= (1 << WDIE);
}
```

<wdt_enable 함수>

wdt_enable(timeout)은 wdt.h에서 제공하는 함수로 timeout 이후에 파기된다. (슬립모드에서도 실행 가능)



```c
lowPowerBodOn(SLEEP_MODE_IDLE);
```

<lowPowerBodOn 함수>

lowPowerBodOn(SLEEP_MODE_IDLE)

```c
#define	lowPowerBodOn(mode)	\
do { 						\
      set_sleep_mode(mode); \
      cli();				\
      sleep_enable();		\
      sei();				\
      sleep_cpu();			\
      sleep_disable();		\
      sei();				\
} while (0);
```

set_sleep_mode(mode)는 sleep.h에서 제공하는 함수로 mode는 다음과 같다.

* SLEEP_MODE_IDLE : CPU 클럭만 멈추고 다른 페리들은 살아있어서 외부 페리의 이벤트에 의해 깨어날 수 있다.
* SLEEP_MODE_ADC : IDLE 보다는 조금 더 페리를 정지 시키고 ADC 노이즈를 줄이는데 효과가 있다.
* SLEEP_MODE_PWR_SAVE : 슬립모드의 타이머를 이용하여 일정시간 이후 슬립모드에서 깨어날 수 있도록 한다.
* SLEEP_MODE_PWR_DOWN : 모든 페리를 끄고 외부 인터럽트만 동작하는 모드 (가장 전류 소모가 적음) 깨어나는 방법은 인터럽트 발생밖에 없음

>  해당 라이브러리에서는 SLEEP_MODE_IDLE과 SLEEP_MODE_ADC만 사용한다.

cli()는 interrupt를 세팅할 때 특정한 인터럽트가 간섭하지 않도록 global interrupt를 거부하도록 한다.

Global 인터럽터를 Enable/Disable 시키는 SREG flag를 세팅하는 것이다.

sleep_enable()은 디바이스를 sleep mode로 만든다. SE bit를 세팅한다.

sei()는 global interrupt를 허용하도록 한다.

sleep_cpu()는 SE bit 세팅 이후 동작하며 디바이스를 sleep 모드로 만든다.ㄴse

sleep_disable()은 SE bit를 초기화한다.

sei()는 global interrupt를 허용하도록 한다.



```c
if (adc == ADC_OFF)
{
    power_adc_enable();
    ADCSRA |= (1 << ADEN);
}
```

adc를 매개변수에 따라 세팅하거나 하지 않는다.

> adc는 analog to digital converter의 줄임말로 아날로그 신호를 디지털 값으로 변환해주는 기능이다.
>
> analogRead()라는 함수로 해당 기능을 제공한다.



### powerDown 함수

매개변수

* peroid
* adc
* bod : brown-out detection으로 (RESET 회로의 기능 + 순간적인 전원전압 하강에도 MCU를 리셋하고 다시 기동 시켜줄 수 있도록 하는 기능)



```C
if (period != SLEEP_FOREVER)
{
    wdt_enable(period);
    WDTCSR |= (1 << WDIE);
}
```

위에서의 설명과 동일함





## 분석

결국 라이브러리 상에서 peroid가 SLEEP_FOREVER가 아니면 wdt_enable 함수가 적용되지만, SLEEP_FOREVER는 PIN이 LOW일 때 깨어난다.

wdt_enable함수에서도 sei()로 global interrupt를 허용해두었기 때문에 pin이 low가 되었을 때 깨어날 수 있을 것이다.



<SLEEP_FOREVER 예제>

```c
// Allow wake up pin to trigger interrupt on low.
attachInterrupt(0, wakeUp, LOW);

// Enter power down state with ADC and BOD module disabled.
// Wake up when wake up pin is low.
LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 

// Disable external pin interrupt on wake up pin.
detachInterrupt(0); 

// Do something here
// Example: Read sensor, data logging, data transmission.
```

attachInterrupt(interrupt핀, 인터럽트 시 부를 ISR, mode)

> ISR에는 매개변수도 반환도 없음
>
> mode는 LOW, CHANGE, RISING, FALLING, HIGH가 있음
>
> > 위에 정의해 둠





## 참고

https://github.com/rocketscream/Low-Power/blob/master/LowPower.cpp : 라이브러리

https://playground.arduino.cc/Learning/ArduinoSleepCode/ : set_sleep_mode

https://www.nongnu.org/avr-libc/user-manual/group__avr__sleep.html : set_sleep_mode

https://nexp.tistory.com/1432 : set_sleep_mode 모드들

https://forum.arduino.cc/t/cli-sei-when-to-use-them/544065 : cli, sei

https://article2.tistory.com/1035 : cli, sei

https://opentutorials.org/module/2106/12249 : adc

https://www.arduino.cc/reference/ko/language/functions/external-interrupts/attachinterrupt/ : attachinterrupt

