// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
// Программа тестирования джойстика PS2X
// V 1.0
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
#include "PS2X_lib.h"  //for v1.6
// Пины радиомодуля
const int PS2_DAT = 12;
const int PS2_CMD = 11;
const int PS2_SEL = 10;
const int PS2_CLK = 13;
/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
//bool pressures = true;
bool pressures = false;
//bool rumble = true;
bool rumble = false;
PS2X ps2x; // объект для PS2X Controller
int error = 0;
byte type = 0;
byte vibrate = 0;

// Функция пропуска цикла
void (*resetFunc) (void) = 0;

void setup(){
  Serial.begin(9600);
  delay(500);  // Задержка, чтобы дать беспроводному модулю PS2X время для запуска перед его настройкой
  // Проверка на наличии ошибок связи
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  if (error == 0) {
    Serial.print("Found Controller, configured successful "); // Найденый контроллер настроен успешно
    Serial.print("pressures = "); // Нажатие
	  if (pressures) { Serial.println("true "); }
	  else {
	    Serial.println("false");
      Serial.print("rumble = ");
    }
	  if (rumble) { Serial.println("true)"); }
	  else {
	    Serial.println("false");
      // Вибрация будет тем сильней, чем сильней нажатие на кнопки
      Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
      // Удерживание одновременно L1 и R1 приводит к выводу значения аналогового датчика
      Serial.println("holding L1 or R1 will print out the analog stick values.");
      Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
    }
  }
  else if (error == 1) {
    // Контроллер не найден, проверьте подключение, см. Readme.txt, чтобы включить отладку
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
  }
  else if (error == 2) {
    // Контроллер обнаружен, но не принимает команды. см. readme.txt, чтобы включить отладку. Visit www.billporter.info for troubleshooting tips
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
  }
  else if (error == 3) {
    // Контроллер отказывается войти в режим определения силы нажатия на кнопки, может не поддерживать его
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  }
  type = ps2x.readType(); // Определяем тип контроллера
  switch(type) {
    case 0:
      // Обнаружен неизвестный тип контроллера
      Serial.println("Unknown Controller type found ");
      break;
    case 1:
      // Контроллер DualShock найден
      Serial.println("DualShock Controller found ");
      break;
    case 2:
      // Найден контроллер GuitarHero
      Serial.println("GuitarHero Controller found ");
      break;
	  case 3:
      // Обнаружен беспроводной контроллер Sony DualShock
      Serial.println("Wireless Sony DualShock Controller found ");
      break;
   }
}

void loop() {
  if (error == 1) { // Пропустить цикл, если контроллер не найден
    resetFunc();
  }

  if (type == 2) { // Guitar Hero контроллер (не наш)
    Serial.println("Not used");
  }
  else { // DualShock контроллер (наш)
    ps2x.read_gamepad(false, vibrate); // Вибрация контроллера

    if(ps2x.Button(PSB_START)) { Serial.println("Start is being held"); }
    if(ps2x.Button(PSB_SELECT)) { Serial.println("Select is being held"); }
    if(ps2x.Button(PSB_PAD_UP)) {
      Serial.print("Up held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
    }
    if(ps2x.Button(PSB_PAD_RIGHT)) {
      Serial.print("Right held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
    }
    if(ps2x.Button(PSB_PAD_LEFT)) {
      Serial.print("LEFT held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
    }
    if(ps2x.Button(PSB_PAD_DOWN)) {
      Serial.print("DOWN held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
    }

    vibrate = ps2x.Analog(PSAB_CROSS);  // Установка силы вибрации пульта управления
    if (ps2x.NewButtonState()) {
      if(ps2x.Button(PSB_L3)) { Serial.println("L3 pressed"); }
      if(ps2x.Button(PSB_R3)) { Serial.println("R3 pressed"); }
      if(ps2x.Button(PSB_L2)) { Serial.println("L2 pressed"); }
      if(ps2x.Button(PSB_R2)) { Serial.println("R2 pressed"); }
      if(ps2x.Button(PSB_TRIANGLE)) { Serial.println("Triangle pressed"); }
    }
    // Нажата кнопка с кругом
    if(ps2x.ButtonPressed(PSB_CIRCLE)) { Serial.println("Circle just pressed"); }
    // Нажата кнопка с X-м
    if(ps2x.NewButtonState(PSB_CROSS)) { Serial.println("X just changed"); }
    // Нажата кнопка с квадратом
    if(ps2x.ButtonReleased(PSB_SQUARE)) { Serial.println("Square just released"); }
    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) {
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); // Левый джойстик, ось Y. Другие варианты: LX, RY, RX
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC);
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC); // Правый джойстик, ось Y. Другие варианты: LX, RY, RX
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC);
    }
  }
  delay(50);
}