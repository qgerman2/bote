#include <Servo.h>
#include <Wire.h>

Servo servo1;
Servo servo2;

void setup() {
  pinMode(13, OUTPUT);
  servo1.attach(9);
  servo2.attach(10);
  Wire.begin(6);
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int bytes) {
  while(1){
    int rem = Wire.available();
    if (rem == 0) {
      break;
    }
    int val = Wire.read();
    switch (rem) {
      case 3:
        servo1.write(map(val, 0, 127, 0, 180));
        break;
      case 2:
        servo2.write(map(val, 0, 127, 0, 180));
        break;
      case 1:
        digitalWrite(13, val);
        break;
    }
  }
}

void loop() {
  delay(10);
}
