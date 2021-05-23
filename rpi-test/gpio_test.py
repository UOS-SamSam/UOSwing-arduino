import RPi.GPIO as gpio
import time

class Sensor:
    def __init__(self):
        gpio.setmode(gpio.BCM)
        self.trig = 24
        self.echo = 23
        print("start ultrasonic sensor")
        gpio.setup(self.trig, gpio.OUT)
        gpio.setup(self.echo, gpio.IN)

    def get_dist(self):
        try:
            while True:
                gpio.output(self.trig, False)
                time.sleep(0.5)
                gpio.output(self.trig, True)
                time.sleep(0.00001)
                gpio.output(self.trig, False)
                while gpio.input(self.echo) == 0:
                    pulse_start = time.time()
                while gpio.input(self.echo) == 1:
                    pulse_end = time.time()
                pulse_duration = pulse_end - pulse_start
                distance = pulse_duration * 17000 #time * speed / 2
                distance = round(distance, 2) #Cut in second decimal place
                return distance
        except:
            gpio.cleanup()
            return -1

if __name__ == '__main__':
    sensor = Sensor()
    print("dist:", sensor.get_dist(), "cm")