#include <Wire.h>
#include "QGPMaker_MotorShield.h"

//Объект шины I2C address
QGPMaker_MotorShield AFMS = QGPMaker_MotorShield();

// Объекты моторов (3) или (1) на шилде
QGPMaker_DCMotor *myMotorR = AFMS.getMotor(3);
QGPMaker_DCMotor *myMotorL = AFMS.getMotor(1);

void setup() {
  Serial.begin(9600);
  Serial.println("DC Motor test!");
  AFMS.begin();  // Задаём частоты по умолчанию 1.6KHz
}

void loop() {
  // Run forward
  myMotorR->run(FORWARD);
  myMotorR->setSpeed(155);
  myMotorL->run(FORWARD);
  myMotorL->setSpeed(155);
  delay(2000);
  // Stop
  myMotorR->run(RELEASE);
  myMotorL->run(RELEASE);
  delay(1000);
  // Run backward
  myMotorR->run(BACKWARD);
  myMotorR->setSpeed(155);
  myMotorL->run(BACKWARD);
  myMotorL->setSpeed(155);
  delay(2000);
  // Stop
  myMotorR->run(RELEASE);
  myMotorL->run(RELEASE);
  delay(1000);
  // Run left
  myMotorR->run(FORWARD);
  myMotorR->setSpeed(155);
  myMotorL->run(BACKWARD);
  myMotorL->setSpeed(155);
  delay(2000);
  // Stop
  myMotorR->run(RELEASE);
  myMotorL->run(RELEASE);
  delay(1000);
  // Run right
  myMotorR->run(BACKWARD);
  myMotorR->setSpeed(155);
  myMotorL->run(FORWARD);
  myMotorL->setSpeed(155);
  delay(2000);
  // Stop
  myMotorR->run(RELEASE);
  myMotorL->run(RELEASE);
  delay(1000);  
  /*
  myMotorR->run(FORWARD);
  myMotorL->run(FORWARD);
  for (int i = 0; i < 255; i++) {
    myMotorR->setSpeed(i);
    myMotorL->setSpeed(i);
    delay(10);
  }
  for (int i = 255; i != 0; i--) {
    myMotorR->setSpeed(i);
    myMotorL->setSpeed(i);
    delay(10);
  }
  myMotorR->run(BACKWARD);
  for (int i = 0; i < 255; i++) {
    myMotorR->setSpeed(i);
    myMotorL->setSpeed(i);
    delay(10);
  }
  for (int i = 255; i != 0; i--) {
    myMotorR->setSpeed(i);
    myMotorL->setSpeed(i);
    delay(10);
  }
  // Останов моторов
  myMotorR->run(RELEASE);
  myMotorL->run(RELEASE);
  delay(1000);
  */
}
