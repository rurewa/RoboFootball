// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Release for ps2x motor shield v5.2 & PS2X remote controll
// Настройка контроллера на новое управление: правый джойстик - быстрая езда
// Левый джойстик - медленная езда.
// Для перезапуска связи контроллера с радиомодулем шилда нажать на кнопку START
// Свободные пины шилда: D6
// V 1.0
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
#include "PS2X_lib.h"  // for v1.6
#include "QGPMaker_Encoder.h"
#include "QGPMaker_MotorShield.h"

// Пины радиомодуля
const int PS2_DAT = 12;
const int PS2_CMD = 11;
const int PS2_SEL = 10;
const int PS2_CLK = 13;

//const bool pressures = false;
//const bool pressures = true; // Аналоговое управление кнопками
//const bool rumble = false;
//const bool rumble = true; // Вибрация контроллера PS2

PS2X ps2x; // Класс PS2 контроллера

const int actuator = 5; // Соленоид
//int error = 0;
byte vibrate = 0;
const int SPEED_PWM = 40;
QGPMaker_MotorShield AFMS = QGPMaker_MotorShield(); // Объект шины I2C
// Объекты моторов (M3) или (M1) на шилде
QGPMaker_DCMotor  *DCMotor_3 = AFMS.getMotor(3); // Правая сторона
QGPMaker_DCMotor  *DCMotor_1 = AFMS.getMotor(1); // Левая сторона

void setup() {
  Serial.begin(9600); // Монитор порта
  AFMS.begin(50); // Инициализация библиотеки
  Serial.println("===========beging==========");
  // Диагностика контроллера PS2X
  int error = 0;
  do {
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
    if (error == 0) { Serial.println("\nConfigured successful! "); break; }
    else { delay(100); }
  }
  while (true);
  AFMS.begin(50);  // Задаём частоту.
  pinMode(actuator, OUTPUT); // Соленоид
}

void loop() {
  ps2x.read_gamepad(false, vibrate); // Читаем контроллер и устанавливаем вибро-мотор на
  // вращение с «вибрирующей» скоростью
  if (ps2x.Button(PSB_START))        // Для перезапуска связи с контроллером
    Serial.println("Start is being held");
  if (ps2x.Button(PSB_SELECT))
    Serial.println("Select is being held");
  if (ps2x.Button(PSB_CROSS)) {
    Serial.println("X-X-X-X");
    ps2x.read_gamepad(true, 200);
    delay(300);
    ps2x.read_gamepad(false, 0);
  }

  if (ps2x.NewButtonState()) {
    // Нажатие самых нижних кнопок с торца. Включение соленоида
    if (ps2x.Button(PSB_L2) || ps2x.Button(PSB_R2))
      { Serial.println("L2 pressed"); digitalWrite(5, HIGH); } else { digitalWrite(5, LOW); }
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // Управление джойстиками
  // Левый джойстик
  if (ps2x.Analog(PSS_LY) > 130) {
    DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 130, 255, 30, 255)));
    DCMotor_3->run(BACKWARD);
    Serial.println(ps2x.Analog(PSS_LY), DEC);
  }
  else if (ps2x.Analog(PSS_LY) < 125) {
    DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 125, 0, 30, 255)));
    DCMotor_3->run(FORWARD);
    Serial.println(ps2x.Analog(PSS_LY), DEC);
  }
  else {
    DCMotor_3->setSpeed(0);
    DCMotor_3->run(RELEASE);

  }
  // Правый джойстик (Можно попробовать использовать для левого, только заменить на LX)
  if (ps2x.Analog(PSS_RY) > 130) {
    DCMotor_1->setSpeed((map(ps2x.Analog(PSS_RY), 130, 255, 30, 255)));
    DCMotor_1->run(BACKWARD);

  }
  else if (ps2x.Analog(PSS_RY) < 125) {
    DCMotor_1->setSpeed((map(ps2x.Analog(PSS_RY), 125, 0, 30, 255)));
    DCMotor_1->run(FORWARD);
  }
  else { // Останов двигателей
    DCMotor_1->setSpeed(0);
    DCMotor_1->run(RELEASE);
  }
  delay(50);
}
// Тест моторов без джойстика
/*
// Вперёд
  DCMotor_3->setSpeed(SPEED_PWM);
  DCMotor_1->setSpeed(SPEED_PWM);
  DCMotor_3->run(FORWARD);
  DCMotor_1->run(FORWARD);
  // Останов
  delay(2000);
  DCMotor_3->run(RELEASE);
  DCMotor_1->run(RELEASE);
  delay(2000);
  // Назад
  DCMotor_3->setSpeed(SPEED_PWM);
  DCMotor_1->setSpeed(SPEED_PWM);
  DCMotor_3->run(BACKWARD);
  DCMotor_1->run(BACKWARD);
  delay(2000);
  // Останов
  DCMotor_3->run(RELEASE);
  DCMotor_1->run(RELEASE);
  delay(2000);
*/
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// END FILE
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //