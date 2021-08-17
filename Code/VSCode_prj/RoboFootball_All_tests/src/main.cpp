// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Test for ps2x motor shield v5.2 & PS2X controll
// V 1.0
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
#include "PS2X_lib.h"  //for v1.6
#include "QGPMaker_Encoder.h"
#include "QGPMaker_MotorShield.h"

// Пины радиомодуля
const int PS2_DAT = 12;
const int PS2_CMD = 11;
const int PS2_SEL = 10;
const int PS2_CLK = 13;

/******************************************************************
   select modes of PS2 controller:
     - pressures = analog reading of push-butttons
     - rumble    = motor rumbling
   uncomment 1 of the lines for each mode selection
 ******************************************************************/
//const bool pressures = false;
const bool pressures = true; // Аналоговое управление кнопками
//const bool rumble = false;
const bool rumble = true; // Вибрация контроллера PS2

PS2X ps2x; // Класс PS2 контроллера

int error = 0;
byte type = 0;
byte vibrate = 0;

int lx = 0;
int ly =  0;

// Объект шины I2C
QGPMaker_MotorShield AFMS = QGPMaker_MotorShield();
// Объекты моторов (M3) или (M1) на шилде
QGPMaker_DCMotor  *DCMotor_3 = AFMS.getMotor(3);
QGPMaker_DCMotor  *DCMotor_1 = AFMS.getMotor(1);

void setup() {
  Serial.begin(9600); // Инициализация библиотеки
  AFMS.begin(50);
  Serial.println("===========beging==========");
  // Диагностика контроллера PS2X
  int error = 0;
  do {
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
    if (error == 0) {
      Serial.println("\nConfigured successful! ");
      break;
    }
    else {
      delay(100);
    }
  }
  while (true);
  AFMS.begin(50);  // Задаём частоту по умолчанию 1.6KHz
}

void loop() {
  ps2x.read_gamepad(false, vibrate); // Считаем контроллер и установаем вибро-мотор на вращение с «вибрирующей» скоростью
  if (ps2x.Button(PSB_START))        // Будет ИСТИНА, пока нажата кнопка
    Serial.println("Start is being held");
  if (ps2x.Button(PSB_SELECT))
    Serial.println("Select is being held");
  // Движение вперёд
  if (ps2x.Button(PSB_PAD_UP)) {     // Будет ИСТИНА, пока нажата кнопка
    Serial.print("Up held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
    DCMotor_3->setSpeed(250);
    DCMotor_1->setSpeed(250);
    DCMotor_3->run(FORWARD);
    DCMotor_1->run(FORWARD);
  }
  else  if (ps2x.Button(PSB_PAD_RIGHT)) {
    Serial.print("Right held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
  }
  else  if (ps2x.Button(PSB_PAD_LEFT)) {
    Serial.print("LEFT held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
  }
  // Движение назад
  else  if (ps2x.Button(PSB_PAD_DOWN)) {
    Serial.print("DOWN held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
    DCMotor_3->setSpeed(250);
    DCMotor_1->setSpeed(250);
    DCMotor_3->run(BACKWARD);
    DCMotor_1->run(BACKWARD);
  }
  // Останов
  else {
    DCMotor_3->setSpeed(0);
    DCMotor_3->run(RELEASE);
    DCMotor_1->setSpeed(0);
    DCMotor_1->run(RELEASE);
  }

  if (ps2x.Button(PSB_CROSS)) {
    Serial.println("X-X-X-X");
    ps2x.read_gamepad(true, 200);
    delay(300);
    ps2x.read_gamepad(false, 0);
  }
  // Это установит силу вибро-мотора в зависимости от того, насколько сильно вы нажимаете синюю кнопку (X)
  vibrate = ps2x.Analog(PSAB_CROSS);
  // Будет ИСТИНА, если какая-либо кнопка изменит состояние (включено на выключено или выключено на включено)
  if (ps2x.NewButtonState()) {
    if (ps2x.Button(PSB_L3)) { Serial.println("L3 pressed"); }
    if (ps2x.Button(PSB_R3)) { Serial.println("R3 pressed"); }
    if (ps2x.Button(PSB_L2)) { Serial.println("L2 pressed"); }
    if (ps2x.Button(PSB_R2)) { Serial.println("R2 pressed"); }
    // Кнопка с треугольником
    if (ps2x.Button(PSB_TRIANGLE)) { Serial.println("Triangle pressed"); }
  }
  // Кнопка с кругом
  if (ps2x.ButtonPressed(PSB_CIRCLE)) { Serial.println("Circle just pressed"); }
  // Кнопка с крестом
  if (ps2x.NewButtonState(PSB_CROSS)) { Serial.println("X just changed"); }
  // Кнопка с квадратом
  if (ps2x.ButtonReleased(PSB_SQUARE)) { Serial.println("Square just released"); }

  if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) {
    Serial.print("Stick Values:");
    Serial.print(ps2x.Analog(PSS_LY), DEC); // Левый джойстик, ось Y. Другие варианты: LX, RY, RX
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_LX), DEC);
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_RY), DEC);
    Serial.print(",");
    Serial.println(ps2x.Analog(PSS_RX), DEC);
  }
  delay(50);
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// END FILE
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
