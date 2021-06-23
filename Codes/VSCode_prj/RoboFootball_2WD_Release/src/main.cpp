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

PS2X ps2x; // Класс PS2 контроллера

const int actuator = 5; // Соленоид
const int SPEED_PWM_SLOW = 10;
const int SPEED_PWM_FAST= 50;
int error = 0;
byte vibrate = 0;

void movePSBpad();
void flashActuator();

// Объект шины I2C
QGPMaker_MotorShield AFMS = QGPMaker_MotorShield();
// Объекты моторов (M3) или (M1) на шилде
QGPMaker_DCMotor  *DCMotor_3 = AFMS.getMotor(3); // Левая сторона
QGPMaker_DCMotor  *DCMotor_1 = AFMS.getMotor(1); // Правая сторона

void setup() {
  Serial.begin(9600); // Инициализация библиотеки
  AFMS.begin(50);
  Serial.println("===========beging==========");
  // Диагностика контроллера PS2X
  int error = 0;
  do {
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
    if (error == 0) { Serial.println("\nConfigured successful! "); break; }
    else { delay(100); }
  }
  while (true);
  AFMS.begin(50);  // Задаём частоту. По умолчанию 1.6KHz
  pinMode(actuator, OUTPUT);
}

void loop() {
  ps2x.read_gamepad(false, vibrate); // Читаем контроллер и установаем вибро-мотор
  if (ps2x.Button(PSB_START))        // Перезапускает связь с контроллером
    Serial.println("Start is being held");
  if (ps2x.Button(PSB_SELECT))
    Serial.println("Select is being held");
  if (ps2x.Button(PSB_CROSS)) {
    Serial.println("X-X-X-X");
    ps2x.read_gamepad(true, 200);
    delay(300);
    ps2x.read_gamepad(false, 0);
  }
  // Это установит силу вибро-мотора в зависимости от того, насколько сильно вы нажимаете синюю кнопку (X)
  vibrate = ps2x.Analog(PSAB_CROSS);
  //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  flashActuator(); // Соленоид
  //movePSBpad();

  // Левый джойстик (медленная скорость)
  if (ps2x.Analog(PSS_LY) > 130) { // > 130. Вперёд
    //DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 135, 255, 10, 10))); // 130, 255, 10, 20
    DCMotor_3->setSpeed(SPEED_PWM_SLOW); // 130, 255, 10, 20
    DCMotor_1->setSpeed(SPEED_PWM_SLOW); // 130, 255, 10, 20
    DCMotor_3->run(BACKWARD);
    DCMotor_1->run(BACKWARD);
  }
  else if (ps2x.Analog(PSS_LY) < 120) { // < 125. Назад
    //DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 125, 0, 10, 10)));
    DCMotor_3->setSpeed(SPEED_PWM_SLOW);
    DCMotor_1->setSpeed(SPEED_PWM_SLOW);
    DCMotor_3->run(FORWARD);
    DCMotor_1->run(FORWARD);
  }
  else {
    DCMotor_3->run(RELEASE);
    DCMotor_1->run(RELEASE);
  }
  // Левый джойстик. Медленно. Влево - вправо
  if (ps2x.Analog(PSS_LX) > 128) { // > 128. Вправо
    DCMotor_3->setSpeed(SPEED_PWM_SLOW); // 130, 255, 10, 20
    DCMotor_1->setSpeed(SPEED_PWM_SLOW); // 130, 255, 10, 20
    DCMotor_3->run(BACKWARD);
    DCMotor_1->run(FORWARD);

  }
  else if (ps2x.Analog(PSS_LX) < 125) { // < 125. Влево
    DCMotor_3->setSpeed(SPEED_PWM_SLOW);
    DCMotor_1->setSpeed(SPEED_PWM_SLOW);
    DCMotor_3->run(FORWARD);
    DCMotor_1->run(BACKWARD);
  }
  else {}
  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  // Правый джойстик (регулируемая скорость)/ По оси Y
  if (ps2x.Analog(PSS_RY) > 130) { // > 130. Вперёд
    //DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 135, 255, 10, 10))); // 130, 255, 10, 20
    DCMotor_3->setSpeed(SPEED_PWM_FAST); // 130, 255, 10, 20
    DCMotor_1->setSpeed(SPEED_PWM_FAST); // 130, 255, 10, 20
    DCMotor_3->run(BACKWARD);
    DCMotor_1->run(BACKWARD);
  }
  else if (ps2x.Analog(PSS_RY) < 120) { // < 125. Назад
    //DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 125, 0, 10, 10)));
    DCMotor_3->setSpeed(SPEED_PWM_FAST);
    DCMotor_1->setSpeed(SPEED_PWM_FAST);
    DCMotor_3->run(FORWARD);
    DCMotor_1->run(FORWARD);
  }
  // Останов
  else {}
  // По оси X
  if (ps2x.Analog(PSS_RX) > 128) { // > 128. Вправо
    DCMotor_3->setSpeed(SPEED_PWM_FAST); // 130, 255, 10, 20
    DCMotor_1->setSpeed(SPEED_PWM_FAST); // 130, 255, 10, 20
    DCMotor_3->run(BACKWARD);
    DCMotor_1->run(FORWARD);

  }
  else if (ps2x.Analog(PSS_RX) < 125) { // < 125. Влево
    DCMotor_3->setSpeed(SPEED_PWM_FAST);
    DCMotor_1->setSpeed(SPEED_PWM_FAST);
    DCMotor_3->run(FORWARD);
    DCMotor_1->run(BACKWARD);
  }
  else {}
  delay(40);
}

void movePSBpad() {
  // Движение вперёд
  if (ps2x.Button(PSB_PAD_UP)) {     // Будет ИСТИНА, пока нажата кнопка
    Serial.print("Up held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
    DCMotor_3->setSpeed(SPEED_PWM_SLOW);
    DCMotor_1->setSpeed(SPEED_PWM_SLOW);
    DCMotor_3->run(FORWARD);
    DCMotor_1->run(FORWARD);
  }
  else  if (ps2x.Button(PSB_PAD_RIGHT)) { // Движение вправо
    Serial.print("Right held this hard: ");
    DCMotor_3->setSpeed(SPEED_PWM_SLOW);
    DCMotor_1->setSpeed(SPEED_PWM_SLOW);
    DCMotor_3->run(FORWARD);
    DCMotor_1->run(BACKWARD);
    Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
  }
  else  if (ps2x.Button(PSB_PAD_LEFT)) { // Движение влево
    Serial.print("LEFT held this hard: ");
    DCMotor_3->setSpeed(SPEED_PWM_SLOW);
    DCMotor_1->setSpeed(SPEED_PWM_SLOW);
    DCMotor_3->run(BACKWARD);
    DCMotor_1->run(FORWARD);
    Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
  }
  // Движение назад
  else  if (ps2x.Button(PSB_PAD_DOWN)) {
    Serial.print("DOWN held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
    DCMotor_3->setSpeed(SPEED_PWM_SLOW);
    DCMotor_1->setSpeed(SPEED_PWM_SLOW);
    DCMotor_3->run(BACKWARD);
    DCMotor_1->run(BACKWARD);
  }
  // Останов
  else {}
}

void flashActuator() {
  if (ps2x.NewButtonState()) {
    // Нажатие кнопки пульта для выполнения соленоида
    if (ps2x.Button(PSB_L2) || ps2x.Button(PSB_R2)) { Serial.println("L2 and R2 pressed"); digitalWrite(actuator, HIGH); }
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
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// END FILE
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //