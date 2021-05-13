/*
  本範程式為PS2無線手把馬達驅動擴展板範例程式
  
  範例很簡單，讓馬達依：前進、後退、停止、左轉、右轉，各1秒，不斷重複
  
  安裝擴展板的整合函式庫。
  下載網址：http://jmaker.banner.tw/doc/Motor_Shield_Library_V5.zip

  下載後解壓縮，把整個目錄放到電腦中的「Documents\Arduino\libraries」。
  
  傑森創工製作
  
  PS2無線手把馬達驅動擴展板賣場：
  https://reurl.cc/GkjQdv
  
  粉絲團：
  https://www.facebook.com/jasonshow
  
  傑森創工部落格：
  https://blog.jmaker.com.tw/

  傑森創工賣場：https://goo.gl/EWoPQ4

 */

#include <Wire.h>
#include <Adafruit_MotorShield.h>

#define carSpeed  150  //馬達速度，本範例固定這個速度

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *myMotor = AFMS.getMotor(1);  //左馬達接在M1
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(4); //右馬達接在M4


void setup() {
  Serial.begin(9600);
  Serial.println("DC Motor test!");

  AFMS.begin();
  
  myMotor->setSpeed(carSpeed);
  myMotor2->setSpeed(carSpeed);
  myMotor->run(FORWARD);
  myMotor2->run(FORWARD);
  myMotor->run(RELEASE);
  myMotor2->run(RELEASE);
}

void forward(){
  myMotor->run(FORWARD);
  myMotor2->run(FORWARD);
}
void backward(){
  myMotor->run(BACKWARD);
  myMotor2->run(BACKWARD);
}
void stopcar(){
  myMotor->run(RELEASE); 
  myMotor2->run(RELEASE);
}
void turnLeft(){
  myMotor->run(FORWARD);
  myMotor2->run(RELEASE);
}
void turnRight (){
  myMotor->run(RELEASE);
  myMotor2->run(FORWARD );
}
void loop() {

  forward();
  delay(1000);
  backward();
  delay(1000);  
  stopcar();
  delay(1000); 
  turnLeft();
  delay(1000);  
  turnRight();
  delay(1000);
}