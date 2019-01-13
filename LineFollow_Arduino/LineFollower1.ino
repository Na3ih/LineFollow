#include <L298N.h>

const static byte ENA = 11;
const static byte ENB = 10;
const static byte IN1 = 9;
const static byte IN2 = 8;
const static byte IN3 = 7;
const static byte IN4 = 6;

L298N Left((int)ENA, (int)IN1, (int)IN2);
L298N Right((int)ENB, (int)IN3, (int)IN4);

const static byte LeftSensor = A0;
const static byte RightSensor = A1;   // for backward mode!!

void setup() 
{
  Left.GPIO_Config();
  Right.GPIO_Config();

  Left.EnableMotor();
  Right.EnableMotor();
  
  Left.SetSpeed(100);
  Right.SetSpeed(100);

  Serial.begin(9600);
}

void loop() 
{
  int leftValue = analogRead(LeftSensor);   Serial.print(leftValue);
  int rightValue = analogRead(RightSensor); Serial.print("||");Serial.println(rightValue);

  if((leftValue > 200) && (rightValue > 200)) GoForward();
  else if((leftValue < 200) && (rightValue > 200)) GoLeft(); //GoRight();//
  else if((leftValue > 200) && (rightValue < 200)) GoRight();//GoLeft();//
  else Stop();
}

void GoForward()
{
  Left.Forward();
  Right.Forward();
}

void GoLeft()
{
  Left.Brake();
  Right.Forward();
}

void GoRight()
{
  Left.Forward();
  Right.Brake();
}

void Stop()
{
  Left.Brake();
  Right.Brake();
}

