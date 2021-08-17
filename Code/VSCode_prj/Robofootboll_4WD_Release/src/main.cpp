// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// Release for ps2x motor shield v5.2 & PS2X gamepad.
// Левый джойстик - вперёд/назад. Правый джойстик - влево/вправо. Блок кнопок слева - медленный ход во все стороны
// Для перезапуска связи контроллера с радиомодулем шилда нажать на кнопку START (ещё не готово)
// Осталось настроить чувствительность правого джойстика.
// Свободные пины шилда: D2, D3, D4, D5, D6, D7 + все аналоговые.
// V 1.0
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
#include <PS2X_lib.h>
#include <QGPMaker_MotorShield.h>

const int SPEED_PWM_SLOW = 40; //скорость для прицеливания
const int MAX_SPEED = 150;     // максимальная скорость для ШИМ регулировки
const int MIN_SPEED = 40;      // минимальная скорость для ШИМ регулировки
const int n = 3;               // коэффициент для усиления подруливаний

// Пины радиомодуля
const int PS2_DAT = 12;
const int PS2_CMD = 11;
const int PS2_SEL = 10;
const int PS2_CLK = 13;

PS2X ps2x; // Класс PS2 контроллера

const int ACTUATOR = 5; // Пин соленоид

// Функции управления
void movePSBpad();   // Движение кнопками
void Joysticks();    // Быстрое движение
void kickActuator(); // Соленоид

QGPMaker_MotorShield AFMS = QGPMaker_MotorShield(); // Объект шины I2C
// Объекты моторов (M3) или (M1) на шилде
QGPMaker_DCMotor *DCMotor_4 = AFMS.getMotor(4); // Левая сторона
QGPMaker_DCMotor *DCMotor_3 = AFMS.getMotor(3); // Левая сторона
QGPMaker_DCMotor *DCMotor_2 = AFMS.getMotor(2); // Правая сторона
QGPMaker_DCMotor *DCMotor_1 = AFMS.getMotor(1); // Правая сторона

void setup()
{
    Serial.begin(9600);
    AFMS.begin(50); // Инициализация библиотеки. Задаём частоту. По умолчанию 1.6KHz
    Serial.println("===========beging==========");
    int error = 0; // Диагностика радиомодуля для PS2X на шилде
    do
    {
        error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
        if (error == 0)
        {
            Serial.println("\nConfigured successful! ");
            break;
        }
        else
        {
            delay(100);
        }
    } while (true);
    pinMode(ACTUATOR, OUTPUT);
}

void loop()
{
    byte vibrate = 0;                  // Вибро-мотор пульта
    ps2x.read_gamepad(false, vibrate); // Читаем контроллер и устанавливаем вибро-мотор
    kickActuator();                    // Соленоид
    movePSBpad();                      // Медленное движение кнопками (прицеливание)
    Joysticks();                       // быстрое движение с ШИМ
    delay(40);                         // Небольшая задержка для стабилизации
}

void movePSBpad()
{
    if (ps2x.Button(PSB_PAD_UP))
    { // Движение вперёд
        DCMotor_4->setSpeed(SPEED_PWM_SLOW);
        DCMotor_3->setSpeed(SPEED_PWM_SLOW);
        DCMotor_2->setSpeed(SPEED_PWM_SLOW);
        DCMotor_1->setSpeed(SPEED_PWM_SLOW);
        DCMotor_4->run(FORWARD);
        DCMotor_3->run(FORWARD);
        DCMotor_2->run(FORWARD);
        DCMotor_1->run(FORWARD);
    }
    else if (ps2x.Button(PSB_PAD_RIGHT))
    { // Движение вправо
        DCMotor_4->setSpeed(SPEED_PWM_SLOW);
        DCMotor_3->setSpeed(SPEED_PWM_SLOW);
        DCMotor_2->setSpeed(SPEED_PWM_SLOW);
        DCMotor_1->setSpeed(SPEED_PWM_SLOW);
        DCMotor_4->run(FORWARD);
        DCMotor_3->run(FORWARD);
        DCMotor_2->run(BACKWARD);
        DCMotor_1->run(BACKWARD);
    }
    else if (ps2x.Button(PSB_PAD_LEFT))
    { // Движение влево
        DCMotor_4->setSpeed(SPEED_PWM_SLOW);
        DCMotor_3->setSpeed(SPEED_PWM_SLOW);
        DCMotor_2->setSpeed(SPEED_PWM_SLOW);
        DCMotor_1->setSpeed(SPEED_PWM_SLOW);
        DCMotor_4->run(BACKWARD);
        DCMotor_3->run(BACKWARD);
        DCMotor_2->run(FORWARD);
        DCMotor_1->run(FORWARD);
    }
    else if (ps2x.Button(PSB_PAD_DOWN))
    { // Движение назад
        DCMotor_4->setSpeed(SPEED_PWM_SLOW);
        DCMotor_3->setSpeed(SPEED_PWM_SLOW);
        DCMotor_2->setSpeed(SPEED_PWM_SLOW);
        DCMotor_1->setSpeed(SPEED_PWM_SLOW);
        DCMotor_4->run(BACKWARD);
        DCMotor_3->run(BACKWARD);
        DCMotor_2->run(BACKWARD);
        DCMotor_1->run(BACKWARD);
    }
    else
    { // Останов
        DCMotor_4->run(RELEASE);
        DCMotor_3->run(RELEASE);
        DCMotor_2->run(RELEASE);
        DCMotor_1->run(RELEASE);
    }
}

void Joysticks()
{ // Левый джойстик вперед/назад. правый джойстик право/лево
    // назад //
    if (ps2x.Analog(PSS_LY) > 132 && ps2x.Analog(PSS_RX) < 132 && ps2x.Analog(PSS_RX) > 123)
    { // По оси Y > 130. Назад
        DCMotor_4->setSpeed((map(ps2x.Analog(PSS_LY), 132, 255, MIN_SPEED, MAX_SPEED)));
        DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 132, 255, MIN_SPEED, MAX_SPEED)));
        DCMotor_2->setSpeed((map(ps2x.Analog(PSS_LY), 132, 255, MIN_SPEED, MAX_SPEED)));
        DCMotor_1->setSpeed((map(ps2x.Analog(PSS_LY), 132, 255, MIN_SPEED, MAX_SPEED)));
        DCMotor_4->run(BACKWARD);
        DCMotor_3->run(BACKWARD);
        DCMotor_2->run(BACKWARD);
        DCMotor_1->run(BACKWARD);
    }
    // вперед //
    if (ps2x.Analog(PSS_LY) < 123 && ps2x.Analog(PSS_RX) < 132 && ps2x.Analog(PSS_RX) > 123)
    { // По оси Y < 125. Вперед
        DCMotor_4->setSpeed((map(ps2x.Analog(PSS_LY), 123, 0, MIN_SPEED, MAX_SPEED)));
        DCMotor_3->setSpeed((map(ps2x.Analog(PSS_LY), 123, 0, MIN_SPEED, MAX_SPEED)));
        DCMotor_2->setSpeed((map(ps2x.Analog(PSS_LY), 123, 0, MIN_SPEED, MAX_SPEED)));
        DCMotor_1->setSpeed((map(ps2x.Analog(PSS_LY), 123, 0, MIN_SPEED, MAX_SPEED)));
        DCMotor_4->run(FORWARD);
        DCMotor_3->run(FORWARD);
        DCMotor_2->run(FORWARD);
        DCMotor_1->run(FORWARD);
    }
    // подруливание вправо вперед //
    if (ps2x.Analog(PSS_LY) < 123 && ps2x.Analog(PSS_RX) > 132 && ps2x.Analog(PSS_RX) <= 255)
    {
        DCMotor_4->setSpeed((map(ps2x.Analog(PSS_RX), 132, 255, MIN_SPEED, MAX_SPEED) * n));
        DCMotor_3->setSpeed((map(ps2x.Analog(PSS_RX), 132, 255, MIN_SPEED, MAX_SPEED) * n));
        DCMotor_2->setSpeed((map(ps2x.Analog(PSS_RX), 132, 255, MAX_SPEED, MIN_SPEED) / n));
        DCMotor_1->setSpeed((map(ps2x.Analog(PSS_RX), 132, 255, MAX_SPEED, MIN_SPEED) / n));
        DCMotor_4->run(FORWARD);
        DCMotor_3->run(FORWARD);
        DCMotor_2->run(FORWARD);
        DCMotor_1->run(FORWARD);
    }
    // подруливание влево вперед //
    if (ps2x.Analog(PSS_LY) < 123 && ps2x.Analog(PSS_RX) < 123 && ps2x.Analog(PSS_RX) >= 0)
    {
        DCMotor_4->setSpeed((map(ps2x.Analog(PSS_RX), 123, 0, MAX_SPEED, MIN_SPEED) / n));
        DCMotor_3->setSpeed((map(ps2x.Analog(PSS_RX), 123, 0, MAX_SPEED, MIN_SPEED) / n));
        DCMotor_2->setSpeed((map(ps2x.Analog(PSS_RX), 123, 0, MIN_SPEED, MAX_SPEED) * n));
        DCMotor_1->setSpeed((map(ps2x.Analog(PSS_RX), 123, 0, MIN_SPEED, MAX_SPEED) * n));
        DCMotor_4->run(FORWARD);
        DCMotor_3->run(FORWARD);
        DCMotor_2->run(FORWARD);
        DCMotor_1->run(FORWARD);
    }
    // подруливание вправо назад //
    if (ps2x.Analog(PSS_LY) > 132 && ps2x.Analog(PSS_RX) > 132 && ps2x.Analog(PSS_RX) <= 255)
    {
        DCMotor_4->setSpeed((map(ps2x.Analog(PSS_RX), 132, 255, MIN_SPEED, MAX_SPEED) * n));
        DCMotor_3->setSpeed((map(ps2x.Analog(PSS_RX), 132, 255, MIN_SPEED, MAX_SPEED) * n));
        DCMotor_2->setSpeed((map(ps2x.Analog(PSS_RX), 132, 255, MAX_SPEED, MIN_SPEED) / n));
        DCMotor_1->setSpeed((map(ps2x.Analog(PSS_RX), 132, 255, MAX_SPEED, MIN_SPEED) / n));
        DCMotor_4->run(BACKWARD);
        DCMotor_3->run(BACKWARD);
        DCMotor_2->run(BACKWARD);
        DCMotor_1->run(BACKWARD);
    }
    // подруливание влево назад //
    if (ps2x.Analog(PSS_LY) > 132 && ps2x.Analog(PSS_RX) < 123 && ps2x.Analog(PSS_RX) >= 0)
    {
        DCMotor_4->setSpeed((map(ps2x.Analog(PSS_RX), 123, 0, MAX_SPEED, MIN_SPEED) / n));
        DCMotor_3->setSpeed((map(ps2x.Analog(PSS_RX), 123, 0, MAX_SPEED, MIN_SPEED) / n));
        DCMotor_2->setSpeed((map(ps2x.Analog(PSS_RX), 123, 0, MIN_SPEED, MAX_SPEED) * n));
        DCMotor_1->setSpeed((map(ps2x.Analog(PSS_RX), 123, 0, MIN_SPEED, MAX_SPEED) * n));
        DCMotor_4->run(BACKWARD);
        DCMotor_3->run(BACKWARD);
        DCMotor_2->run(BACKWARD);
        DCMotor_1->run(BACKWARD);
    }
    // кручение вправо //
    if (ps2x.Analog(PSS_RX) > 132 && ps2x.Analog(PSS_LY) < 132 && ps2x.Analog(PSS_LY) > 123)
    { // По оси X > 128. Вправо
        DCMotor_4->setSpeed((map(ps2x.Analog(PSS_RX), 132, 255, MIN_SPEED, MAX_SPEED)));
        DCMotor_3->setSpeed((map(ps2x.Analog(PSS_RX), 132, 255, MIN_SPEED, MAX_SPEED)));
        DCMotor_2->setSpeed((map(ps2x.Analog(PSS_RX), 132, 255, MIN_SPEED, MAX_SPEED)));
        DCMotor_1->setSpeed((map(ps2x.Analog(PSS_RX), 132, 255, MIN_SPEED, MAX_SPEED)));
        DCMotor_4->run(FORWARD);
        DCMotor_3->run(FORWARD);
        DCMotor_2->run(BACKWARD);
        DCMotor_1->run(BACKWARD);
    }
    // кручение влево //
    if (ps2x.Analog(PSS_RX) < 123 && ps2x.Analog(PSS_LY) < 132 && ps2x.Analog(PSS_LY) > 123)
    { // По оси X > 128. Вправо
        DCMotor_4->setSpeed((map(ps2x.Analog(PSS_RX), 123, 0, MIN_SPEED, MAX_SPEED)));
        DCMotor_3->setSpeed((map(ps2x.Analog(PSS_RX), 123, 0, MIN_SPEED, MAX_SPEED)));
        DCMotor_2->setSpeed((map(ps2x.Analog(PSS_RX), 123, 0, MIN_SPEED, MAX_SPEED)));
        DCMotor_1->setSpeed((map(ps2x.Analog(PSS_RX), 123, 0, MIN_SPEED, MAX_SPEED)));
        DCMotor_4->run(BACKWARD);
        DCMotor_3->run(BACKWARD);
        DCMotor_2->run(FORWARD);
        DCMotor_1->run(FORWARD);
    }
}

void kickActuator()
{ // Нажатие кнопки пульта для выполнения соленоида
    if (ps2x.NewButtonState())
    {
        if (ps2x.Button(PSB_L2) || ps2x.Button(PSB_R2))
        {
            Serial.println("L2 or R2");
            digitalWrite(ACTUATOR, HIGH);
        }
        else
        {
            digitalWrite(ACTUATOR, LOW);
        }
    }
}

void vibroCrossPSB()
{
    if (ps2x.Button(PSB_CROSS))
    { // Проверка работы джойстика (запуск вибро-мотора)
        Serial.println("X-X-X-X");
        ps2x.read_gamepad(true, 200);
        delay(300);
        ps2x.read_gamepad(false, 0);
    }
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
// END FILE
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- //
