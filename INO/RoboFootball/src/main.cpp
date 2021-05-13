// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
// Программа тестирования джойстика PS2X
// V 1.0
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
#include <Arduino.h>
#include "PS2X_lib.h" // for v1.6

/******************************************************************
 * Установить контакты, подключенные к контроллеру PS2:
 ******************************************************************/
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

PS2X ps2x; // Создаём объект для PS2X Controller

// После подключения контроллера нужно перезапустить Arduino

int error = 0;
byte type = 0;
byte vibrate = 0;

// Reset func
void (*resetFunc)(void) = 0;

void setup() {
  Serial.begin(9600);
  delay(500); // Задержка, чтобы дать беспроводному модулю PS2X дать время для запуска перед его настройкой
  // Проверка на наличии ошибок связи
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  if (error == 0)   {
    Serial.print("Найденый контроллер настроен успешно\n");
    Serial.print("нажатие = ");
    if (pressures)
      Serial.println("true ");
    else
      Serial.println("false");
    Serial.print("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");
    Serial.println("Попробуйте. Вибрация будет тем сильней, чем сильней нажатие на кнопки");
    Serial.println("Удерживая одновременно L1 и R1 выводит значение аналогового датчика");
    Serial.println("Примечание. Посетите www.billporter.info для получения обновлений и сообщений об ошибках");
  }
  else if (error == 1)
    Serial.println("Контроллер не найден, проверьте подключение, см. Readme.txt, чтобы включить отладку");

  else if (error == 2)
    Serial.println("Контроллер обнаружен, но не принимает команды. см. readme.txt, чтобы включить отладку. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Контроллер отказывается войти в режим определения силы нажатия на кнопки, может не поддерживать его");

  type = ps2x.readType();
  switch (type)
  {
  case 0:
    Serial.println("Обнаружен неизвестный тип контроллера");
    break;
  case 1:
    Serial.println("Контроллер DualShock найден");
    break;
  case 2:
    Serial.println("Найден контроллер GuitarHero");
    break;
  case 3:
    Serial.println("Обнаружен беспроводной контроллер Sony DualShock");
    break;
  }
}

void loop() {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */
  if (error == true) { // Пропустить цикл, если контроллер не найден
    resetFunc();
  }

  if (type == 2) { // Контроллер Guitar Hero
    ps2x.read_gamepad(); // Считываем данные с контроллера

    if (ps2x.ButtonPressed(GREEN_FRET))
      Serial.println("Green Fret Pressed");
    if (ps2x.ButtonPressed(RED_FRET))
      Serial.println("Red Fret Pressed");
    if (ps2x.ButtonPressed(YELLOW_FRET))
      Serial.println("Yellow Fret Pressed");
    if (ps2x.ButtonPressed(BLUE_FRET))
      Serial.println("Blue Fret Pressed");
    if (ps2x.ButtonPressed(ORANGE_FRET))
      Serial.println("Orange Fret Pressed");

    if (ps2x.ButtonPressed(STAR_POWER))
      Serial.println("Star Power Command");

    if (ps2x.Button(UP_STRUM)) //will be TRUE as long as button is pressed
      Serial.println("Up Strum");
    if (ps2x.Button(DOWN_STRUM))
      Serial.println("DOWN Strum");

    if (ps2x.Button(PSB_START)) //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
    if (ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");

    if (ps2x.Button(ORANGE_FRET))
    { // print stick value IF TRUE
      Serial.print("Wammy Bar Position:");
      Serial.println(ps2x.Analog(WHAMMY_BAR), DEC);
    }
  }
  else { // DualShock Controller
    ps2x.read_gamepad(false, vibrate); // read controller and set large motor to spin at 'vibrate' speed

    if (ps2x.Button(PSB_START)) // will be TRUE as long as button is pressed
      Serial.println("Стартуем");
    if (ps2x.Button(PSB_SELECT))
      Serial.println("Удерживаем выбор");

    if (ps2x.Button(PSB_PAD_UP)) { //will be TRUE as long as button is pressed
      Serial.print("Up held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
    }
    if (ps2x.Button(PSB_PAD_RIGHT)) {
      Serial.print("Right held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
    }
    if (ps2x.Button(PSB_PAD_LEFT)) {
      Serial.print("LEFT held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
    }
    if (ps2x.Button(PSB_PAD_DOWN)) {
      Serial.print("DOWN held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
    }

    vibrate = ps2x.Analog(PSAB_CROSS); // this will set the large motor vibrate speed based on how hard you press the blue (X) button
    if (ps2x.NewButtonState()) { //will be TRUE if any button changes state (on to off, or off to on)
      if (ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
      if (ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");
      if (ps2x.Button(PSB_L2))
        Serial.println("L2 pressed");
      if (ps2x.Button(PSB_R2))
        Serial.println("R2 pressed");
      if (ps2x.Button(PSB_TRIANGLE))
        Serial.println("Triangle pressed");
    }

    if (ps2x.ButtonPressed(PSB_CIRCLE)) //will be TRUE if button was JUST pressed
      Serial.println("Circle just pressed");
    if (ps2x.NewButtonState(PSB_CROSS)) //will be TRUE if button was JUST pressed OR released
      Serial.println("X just changed");
    if (ps2x.ButtonReleased(PSB_SQUARE)) //will be TRUE if button was JUST released
      Serial.println("Square just released");

    if (ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //print stick values if either is TRUE
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC);
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC);
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC);
    }
  }
  delay(50);
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
// END FILE
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//