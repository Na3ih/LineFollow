/*
 * STM32_L298N_SPL.c
 *
 *  Created on: 06.01.2019
 *      Author: na3ih
 *
 * ------------------------------------------------------------------------------------
 * Library allows control DC Motors by L298N H-Bridge module
 * (https://botland.com.pl/pl/sterowniki-silnikow-moduly/3164-l298n-dwukanalowy-sterownik-silnikow-modul-12v2a.html)
 *
 * Includes functions for each motor or for two motors
 *
 * MotorA - right motor
 * MotorB - left motor
 */


#include "STM32_L298N_SPL.h"

// ----------- Static Helper Functions Declarations-------------
static void MotorA_GPIO_Config(void);
static void MotorA_ClockConfig(void);
static void MotorB_GPIO_Config(void);
static void MotorB_ClockConfig(void);

// --------- MotorA Functions ------------

void MotorA_Init(uint16_t ENA_Pin, GPIO_TypeDef* ENA_Port, uint16_t IN1_Pin, GPIO_TypeDef* IN1_Port, uint16_t IN2_Pin, GPIO_TypeDef* IN2_Port)
{
	ENA_PIN = ENA_Pin;		ENA_PORT = ENA_Port;
	IN1_PIN = IN1_Pin;		IN1_PORT = IN1_Port;
	IN2_PIN = IN2_Pin;		IN2_PORT = IN2_Port;

	MotorA_ClockConfig();
	MotorA_GPIO_Config();
	MotorA_Enable();
}

void MotorA_Enable(void)
{
	GPIO_SetBits(ENA_PORT, ENA_PIN);
}

void MotorA_Disable(void)
{
	GPIO_ResetBits(ENA_PORT, ENA_PIN);
}

void MotorA_Forward(void)
{
	GPIO_SetBits(IN1_PORT, IN1_PIN);
	GPIO_ResetBits(IN2_PORT, IN2_PIN);
}

void MotorA_Backward(void)
{
	GPIO_ResetBits(IN1_PORT, IN1_PIN);
	GPIO_SetBits(IN2_PORT, IN2_PIN);
}

void MotorA_Brake(void)
{
	GPIO_SetBits(IN1_PORT, IN1_PIN);
	GPIO_SetBits(IN2_PORT, IN2_PIN);
}

void MotorA_Coast(void)
{
	GPIO_ResetBits(IN1_PORT, IN1_PIN);
	GPIO_ResetBits(IN2_PORT, IN2_PIN);
}



// --------- MotorB Functions ------------

void MotorB_Init(uint16_t ENB_Pin, GPIO_TypeDef* ENB_Port, uint16_t IN3_Pin, GPIO_TypeDef* IN3_Port, uint16_t IN4_Pin, GPIO_TypeDef* IN4_Port)
{
	ENB_PIN = ENB_Pin;		ENB_PORT = ENB_Port;
	IN3_PIN = IN3_Pin;		IN3_PORT = IN3_Port;
	IN4_PIN = IN4_Pin;		IN4_PORT = IN4_Port;

	MotorB_ClockConfig();
	MotorB_GPIO_Config();
	MotorB_Enable();
}

void MotorB_Enable(void)
{
	GPIO_SetBits(ENB_PORT, ENB_PIN);
}

void MotorB_Disable(void)
{
	GPIO_ResetBits(ENB_PORT, ENB_PIN);
}

void MotorB_Forward(void)
{
	GPIO_SetBits(IN3_PORT, IN3_PIN);
	GPIO_ResetBits(IN4_PORT, IN4_PIN);
}

void MotorB_Backward(void)
{
	GPIO_ResetBits(IN3_PORT, IN3_PIN);
	GPIO_SetBits(IN4_PORT, IN4_PIN);
}

void MotorB_Brake(void)
{
	GPIO_SetBits(IN3_PORT, IN3_PIN);
	GPIO_SetBits(IN4_PORT, IN4_PIN);
}

void MotorB_Coast(void)
{
	GPIO_ResetBits(IN3_PORT, IN3_PIN);
	GPIO_ResetBits(IN4_PORT, IN4_PIN);
}


// --------- Functions for 2 motors ------------

void Motors_Init(uint16_t ENA_Pin, GPIO_TypeDef* ENA_Port, uint16_t IN1_Pin, GPIO_TypeDef* IN1_Port, uint16_t IN2_Pin, GPIO_TypeDef* IN2_Port,
				 uint16_t ENB_Pin, GPIO_TypeDef* ENB_Port, uint16_t IN3_Pin, GPIO_TypeDef* IN3_Port, uint16_t IN4_Pin, GPIO_TypeDef* IN4_Port)
{
	ENA_PIN = ENA_Pin;		ENA_PORT = ENA_Port;
	IN1_PIN = IN1_Pin;		IN1_PORT = IN1_Port;
	IN2_PIN = IN2_Pin;		IN2_PORT = IN2_Port;

	ENB_PIN = ENB_Pin;		ENB_PORT = ENB_Port;
	IN3_PIN = IN3_Pin;		IN3_PORT = IN3_Port;
	IN4_PIN = IN4_Pin;		IN4_PORT = IN4_Port;

	MotorA_ClockConfig();
	MotorB_ClockConfig();

	MotorA_GPIO_Config();
	MotorB_GPIO_Config();

	MotorA_Enable();
	MotorB_Enable();
}

void GoForward(void)
{
	MotorA_Forward();
	MotorB_Forward();
}

void GoBackward(void)
{
	MotorA_Backward();
	MotorB_Backward();
}

void TurnLeft(void)
{
	MotorA_Forward();
	MotorB_Backward();
}

void TurnRight(void)
{
	MotorA_Backward();
	MotorB_Forward();
}

void Brake(void)
{
	MotorA_Brake();
	MotorB_Brake();
}

void Coast(void)
{
	MotorA_Coast();
	MotorB_Coast();
}


// ----------- Static Helper Functions -------------
static void MotorA_ClockConfig()
{
	if((ENA_PORT == GPIOA) || (IN1_PORT == GPIOA) || (IN2_PORT == GPIOA)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	if((ENA_PORT == GPIOB) || (IN1_PORT == GPIOB) || (IN2_PORT == GPIOB)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	if((ENA_PORT == GPIOC) || (IN1_PORT == GPIOC) || (IN2_PORT == GPIOC)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	if((ENA_PORT == GPIOD) || (IN1_PORT == GPIOD) || (IN2_PORT == GPIOD)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	if((ENA_PORT == GPIOE) || (IN1_PORT == GPIOE) || (IN2_PORT == GPIOE)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	if((ENA_PORT == GPIOF) || (IN1_PORT == GPIOF) || (IN2_PORT == GPIOF)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
}

static void MotorA_GPIO_Config(void)
{
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;

	gpio.GPIO_Pin = ENA_PIN;
	GPIO_Init(ENA_PORT, &gpio);

	gpio.GPIO_Pin = IN1_PIN;
	GPIO_Init(IN1_PORT, &gpio);

	gpio.GPIO_Pin = IN2_PIN;
	GPIO_Init(IN2_PORT, &gpio);
}

static void MotorB_ClockConfig(void)
{
	if((ENB_PORT == GPIOA) || (IN3_PORT == GPIOA) || (IN4_PORT == GPIOA)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	if((ENB_PORT == GPIOB) || (IN3_PORT == GPIOB) || (IN4_PORT == GPIOB)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	if((ENB_PORT == GPIOC) || (IN3_PORT == GPIOC) || (IN4_PORT == GPIOC)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	if((ENB_PORT == GPIOD) || (IN3_PORT == GPIOD) || (IN4_PORT == GPIOD)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	if((ENB_PORT == GPIOE) || (IN3_PORT == GPIOE) || (IN4_PORT == GPIOE)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	if((ENB_PORT == GPIOF) || (IN3_PORT == GPIOF) || (IN4_PORT == GPIOF)) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
}

static void MotorB_GPIO_Config(void)
{
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;

	gpio.GPIO_Pin = ENB_PIN;
	GPIO_Init(ENB_PORT, &gpio);

	gpio.GPIO_Pin = IN3_PIN;
	GPIO_Init(IN3_PORT, &gpio);

	gpio.GPIO_Pin = IN4_PIN;
	GPIO_Init(IN4_PORT, &gpio);
}
