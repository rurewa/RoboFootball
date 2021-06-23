// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Release for ps2x motor shield v5.2 & PS2X gamepad.
// Правый джойстик - быстрая езда. Левый джойстик и блок кнопок слева - медленная езда.
// Для перезапуска связи контроллера с радиомодулем шилда нажать на кнопку START (ещё не готово)
// Осталось настроить чувствительность правого джойстика.
// Свободные пины шилда: D6
// V 1.0
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
#include "PS2X_lib.h"  // for v1.6
#include "QGPMaker_MotorShield.h"

// Пины радиомодуля
const int PS2_DAT = 12;
const int PS2_CMD = 11;
const int PS2_SEL = 10;
const int PS2_CLK = 13;

PS2X ps2x; // Класс PS2 контроллера

const int actuator = 5; // Соленоид
const int SPEED_PWM_SLOW = 10;
const int SPEED_PWM_FAST= 50;
byte vibrate = 0;
// Функции управления
void movePSBpad(); // Движение кнопками
void leftJoystick(); // Медленное движение
void rightJoystick(); // Быстрое движение
void flashActuator(); // Соленоид

QGPMaker_MotorShield AFMS = QGPMaker_MotorShield(); // Объект шины I2C
// Объекты моторов (M3) или (M1) на шилде
QGPMaker_DCMotor  *DCMotor_3 = AFMS.getMotor(3); // Левая сторона
QGPMaker_DCMotor  *DCMotor_1 = AFMS.getMotor(1); // Правая сторона

void setup() {
  Serial.begin(9600);
  AFMS.begin(50); // Инициализация библиотеки. Задаём частоту. По умолчанию 1.6KHz
  Serial.println("===========beging==========");
  int error = 0; // Диагностика радиомодуля для PS2X на шилде
  do {
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
    if (error == 0) { Serial.println("\nConfigured successful! "); break; }
    else { delay(100); }
  }
  while (true);
  for(size_t i = 0; i < 50; i++) { // Очистка частот радиоканала
    ps2x.read_gamepad(false, 0);
    delay(10);
  }
  pinMode(actuator, OUTPUT);
}

void loop() {
  ps2x.read_gamepad(false, vibrate); // Читаем контроллер и установаем вибро-мотор
  if (ps2x.Button(PSB_CROSS)) { // Проверка работы джойстика (запуск вибро-мотора)
    Serial.println("X-X-X-X");
    ps2x.read_gamepad(true, 200);
    delay(300);
    ps2x.read_gamepad(false, 0);
  }
  flashActuator(); // Соленоид
  movePSBpad(); // Движение кнопками
  leftJoystick(); // Медленное движение
  rightJoystick(); // Быстрое движение
  delay(40);
}

void movePSBpad() {
  if (ps2x.Button(PSB_PAD_UP)) { // Движение вперёд
    DCMotor_3->setSpeed(SPEED_PWM_SLOW);
    DCMotor_1->setSpeed(SPEED_PWM_SLOW);
    DCMotor_3->run(FORWARD);
    DCMotor_1->run(FORWARD);
  }
  else  if (ps2x.Button(PSB_PAD_RIGHT)) { // Движение вправо
    DCMotor_3->setSpeed(SPEED_PWM_SLOW);
    DCMotor_1->setSpeed(SPEED_PWM_SLOW);
    DCMotor_3->run(FORWARD);
    DCMotor_1->run(BACKWARD);
  }
  else  if (ps2x.Button(PSB_PAD_LEFT)) { // Движение влево
    DCMotor_3->setSpeed(SPEED_PWM_SLOW);
    DCMotor_1->setSpeed(SPEED_PWM_SLOW);
    DCMotor_3->run(BACKWARD);
    DCMotor_1->run(FORWARD);
  }
  else  if (ps2x.Button(PSB_PAD_DOWN)) { // Движение назад
    DCMotor_3->setSpeed(SPEED_PWM_SLOW);
    DCMotor_1->setSpeed(SPEED_PWM_SLOW);
    DCMotor_3->run(BACKWARD);
    DCMotor_1->run(BACKWARD);
  }
  else { // Останов
    DCMotor_3->run(RELEASE);
    DCMotor_1->run(RELEASE);
  }
}

void leftJoystick() { // Левый джойстик (медленная скорость).
  if (ps2x.Analog(PSS_LY) > 130) { // Ось Y > 130. Назад
    DCMotor_3->setSpeed(SPEED_PWM_SLOW);
    DCMotor_1->setSpeed(SPEED_PWM_SLOW);
    DCMotor_3->run(BACKWARD);
    DCMotor_1->run(BACKWARD);
  }
  else if (ps2x.Analog(PSS_LY) < 120) { // < 125. Вперёд
    DCMotor_3->setSpeed(SPEED_PWM_SLOW);
    DCMotor_1->setSpeed(SPEED_PWM_SLOW);
    DCMotor_3->run(FORWARD);
    DCMotor_1->run(FORWARD);
  }
  else {} // Останов
  if (ps2x.Analog(PSS_LX) > 128) { // Ось X > 128. Вправо
    DCMotor_3->setSpeed(SPEED_PWM_SLOW);
    DCMotor_1->setSpeed(SPEED_PWM_SLOW);
    DCMotor_3->run(BACKWARD);
    DCMotor_1->run(FORWARD);
  }
  else if (ps2x.Analog(PSS_LX) < 125) { // < 125. Влево
    DCMotor_3->setSpeed(SPEED_PWM_SLOW);
    DCMotor_1->setSpeed(SPEED_PWM_SLOW);
    DCMotor_3->run(FORWARD);
    DCMotor_1->run(BACKWARD);
  }
  else {} // Останов
}

void rightJoystick() { // Правый джойстик (регулируемая скорость.
  if (ps2x.Analog(PSS_RY) > 130) { // По оси Y > 130. Назад
    DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 130, 255, 30, 255)));
    DCMotor_1->setSpeed((map(ps2x.Analog(PSS_LY), 130, 255, 30, 255)));
    DCMotor_3->run(BACKWARD);
    DCMotor_1->run(BACKWARD);
  }
  else if (ps2x.Analog(PSS_RY) < 120) { // < 125. Вперёд
    DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 130, 255, 30, 255)));
    DCMotor_1->setSpeed((map(ps2x.Analog(PSS_LY), 130, 255, 30, 255)));
    DCMotor_3->run(FORWARD);
    DCMotor_1->run(FORWARD);
  }
  else {} // Останов
  if (ps2x.Analog(PSS_RX) > 128) { // По оси X > 128. Вправо
    DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 130, 255, 30, 255)));
    DCMotor_1->setSpeed((map(ps2x.Analog(PSS_LY), 130, 255, 30, 255)));
    DCMotor_3->run(BACKWARD);
    DCMotor_1->run(FORWARD);
  }
  else if (ps2x.Analog(PSS_RX) < 125) { // < 125. Влево
    DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 130, 255, 30, 255)));
    DCMotor_1->setSpeed((map(ps2x.Analog(PSS_LY), 130, 255, 30, 255)));
    DCMotor_3->run(FORWARD);
    DCMotor_1->run(BACKWARD);
  }
  else {} // Останов
}

void flashActuator() { // Нажатие кнопки пульта для выполнения соленоида
  if (ps2x.NewButtonState()) {
    if (ps2x.Button(PSB_L2) || ps2x.Button(PSB_R2)) {
      Serial.println("L2 and R2 pressed"); digitalWrite(actuator, HIGH); }
    else { digitalWrite(actuator, LOW); }
  }
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
/*
  if (ps2x.Button(PSB_START))        // Перезапускает связь с контроллером
    Serial.println("Start is being held");
  if (ps2x.Button(PSB_SELECT))
    Serial.println("Select is being held");
  */
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// END FILE
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //