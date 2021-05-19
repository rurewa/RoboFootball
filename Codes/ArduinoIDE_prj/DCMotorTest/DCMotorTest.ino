// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Test motors for ps2x motor shield v5.2
// V 1.0
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
#include <Wire.h>
#include "QGPMaker_MotorShield.h"

//Объект шины I2C address
QGPMaker_MotorShield AFMS = QGPMaker_MotorShield();

// Объекты моторов (M3) или (M1) на шилде
QGPMaker_DCMotor *myMotorR = AFMS.getMotor(3);
QGPMaker_DCMotor *myMotorL = AFMS.getMotor(1);

// Скорость ШИМ для левой и правой стороны
const int SPEED_PWM_LEFT = 200;
const int SPEED_PWM_RIGHT = 200;

void setup() {
  Serial.begin(9600);
  AFMS.begin();  // Задаём частоты по умолчанию 1.6KHz
}

void loop() {
  // Run forward
  myMotorR->run(FORWARD);
  myMotorR->setSpeed(SPEED_PWM_RIGHT);
  myMotorL->run(FORWARD);
  myMotorL->setSpeed(SPEED_PWM_LEFT);
  delay(2000);
  // Stop
  myMotorR->run(RELEASE);
  myMotorL->run(RELEASE);
  delay(1000);
  // Run backward
  myMotorR->run(BACKWARD);
  myMotorR->setSpeed(SPEED_PWM_RIGHT);
  myMotorL->run(BACKWARD);
  myMotorL->setSpeed(SPEED_PWM_LEFT);
  delay(2000);
  // Stop
  myMotorR->run(RELEASE);
  myMotorL->run(RELEASE);
  delay(1000);
  // Run left
  myMotorR->run(FORWARD);
  myMotorR->setSpeed(SPEED_PWM_RIGHT);
  myMotorL->run(BACKWARD);
  myMotorL->setSpeed(SPEED_PWM_LEFT);
  delay(2000);
  // Stop
  myMotorR->run(RELEASE);
  myMotorL->run(RELEASE);
  delay(1000);
  // Run right
  myMotorR->run(BACKWARD);
  myMotorR->setSpeed(SPEED_PWM_RIGHT);
  myMotorL->run(FORWARD);
  myMotorL->setSpeed(SPEED_PWM_LEFT);
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
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// END FILE
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
