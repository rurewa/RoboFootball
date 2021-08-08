// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Возможный вариант функции для плавного старта моторов
// V 1.0
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
const int buttonPin = 3;

void setup() {
  Serial.begin(9600);
}

void loop() {
  smoothStart(255, 15, 200);
}
/*
void movePSBpad(bool ) {
  static int counter = 0;
  if (ps2x.Button(PSB_CIRCLE)) {
    counter += 15;
    delay(100);
    while (counter == SPEED_PWM_SLOW && ps2x.Button(PSB_CIRCLE)) {
      DCMotor_3->setSpeed(SPEED_PWM_SLOW);
      DCMotor_3->run(FORWARD);
    }
    Serial.println(counter);
  }
  else {
    counter = 0;
  }
}
*/

void smoothStart(int MAX_SPEED, int STEPS, int DELAYS) {
  static int counter = 0;
  if (digitalRead(buttonPin) == true) {
    counter += STEPS;
    delay(DELAYS);
    while ((counter == MAX_SPEED) && (digitalRead(buttonPin) == true)) {
      Serial.println("Max! ");
      Serial.println(counter);
    }
    Serial.println(counter);
  }
  else {
    counter = 0;
    Serial.println("Button OFF");
  }
}

void maxCounterFor() {
  if (digitalRead(buttonPin) == true) {
    for (int i = 15; i <= 255; i += 15) {
      delay(100);
      while (i == 255 && digitalRead(buttonPin) == true) {
        Serial.println("Max!");
      }
      Serial.println(i);
    }
  }
  else {
    Serial.println("Button OFF");
  }
}

void testButton() {
  if (digitalRead(buttonPin) == true) {
    Serial.print(buttonPin);
    Serial.print(" ");
    Serial.println("Button ON");
  }
  else {
    Serial.print(buttonPin);
    Serial.print(" ");
    Serial.println("Button OFF");
  }
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// END FILE
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
