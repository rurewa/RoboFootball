// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Test for ps2x motor shield v5.2 & PS2X controll, Joysticks
// V 1.0
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
#include <Wire.h>
#include "PS2X_lib.h"
#include "QGPMaker_MotorShield.h"
// Объект шины I2C
QGPMaker_MotorShield  AFMS = QGPMaker_MotorShield();
// Класс PS2 контроллера
PS2X ps2x;
// Объекты моторов (M3) или (M1) на шилде
QGPMaker_DCMotor *DCMotor_3 = AFMS.getMotor(3);
QGPMaker_DCMotor *DCMotor_1 = AFMS.getMotor(1);

void setup() {
  AFMS.begin(50); // Инициализация библиотеки
  int error = 0;
  do {
    error = ps2x.config_gamepad(13, 11, 10, 12, true, true);
    if (error == 0) {
      break;
    }
    else {
      delay(100);
    }
  }
  while (true);
  for (size_t i = 0; i < 50; i++) {
    ps2x.read_gamepad(false, 0);
    delay(10);
  }
}

void loop() {
  ps2x.read_gamepad(false, 0); // Считаем контроллер
  delay(3);
  if (ps2x.Button(PSB_CROSS)) {
    ps2x.read_gamepad(true, 200);
    delay(300);
    ps2x.read_gamepad(false, 0);

  }
  // Левый джойстик
  if (ps2x.Analog(PSS_LY) > 130) {
    DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 130, 255, 30, 255)));
    DCMotor_3->run(FORWARD);

  }
  else if (ps2x.Analog(PSS_LY) < 125) {
    DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 125, 0, 30, 255)));
    DCMotor_3->run(BACKWARD);
  }
  else {
    DCMotor_3->setSpeed(0);
    DCMotor_3->run(RELEASE);

  }
  // Правый джойстик
  if (ps2x.Analog(PSS_RY) > 130) {
    DCMotor_1->setSpeed((map(ps2x.Analog(PSS_RY), 130, 255, 30, 255)));
    DCMotor_1->run(FORWARD);

  }
  else if (ps2x.Analog(PSS_RY) < 125) {
    DCMotor_1->setSpeed((map(ps2x.Analog(PSS_RY), 125, 0, 30, 255)));
    DCMotor_1->run(BACKWARD);
  }
  else {
    DCMotor_1->setSpeed(0);
    DCMotor_1->run(RELEASE);
  }
}