#include "L298N.h"
#include <Arduino.h>


L298N::L298N(int ENABLE_pin, int IN1_pin, int IN2_pin)
{
  ENABLE = ENABLE_pin;
  IN1 = IN1_pin;
  IN2 = IN2_pin;
}

void L298N::GPIO_Config()
{
	pinMode(ENABLE, OUTPUT);
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
}

void L298N::EnableMotor(void)
{
  digitalWrite(ENABLE, HIGH);
}

void L298N::DisableMotor(void)
{
  digitalWrite(ENABLE, LOW);
}

void L298N::Forward(void)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void L298N::Backward(void)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void L298N::Brake(void)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
}

void L298N::Coast(void)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);  
}

void L298N::SetSpeed(int pwm)
{
  analogWrite(ENABLE, pwm);
}

//void L298N::TurnLeft(int ms);
//void L298N::TurnRight(int ms);