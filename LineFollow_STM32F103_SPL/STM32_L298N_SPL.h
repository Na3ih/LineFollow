/* Library allows control DC Motors by L298N H-Bridge module
 * (https://botland.com.pl/pl/sterowniki-silnikow-moduly/3164-l298n-dwukanalowy-sterownik-silnikow-modul-12v2a.html)
 *
 * Includes functions for each motor or for two motors
 *
 * MotorA - right motor
 * MotorB - left motor
 */



#ifndef STM32_L298N_SPL_h
#define STM32_L298N_SPL_h

#include "stm32f10x.h"


// --------- MotorA Functions ------------
void MotorA_Init(uint16_t ENA_Pin, GPIO_TypeDef* ENA_Port, uint16_t IN1_Pin, GPIO_TypeDef* IN1_Port, uint16_t IN2_Pin, GPIO_TypeDef* IN2_Port);
void MotorA_Enable(void);
void MotorA_Disable(void);
void MotorA_Forward(void);
void MotorA_Backward(void);
void MotorA_Left(void);
void MotorA_Right(void);
void MotorA_Brake(void);
void MotorA_Coast(void);

// --------- MotorB Functions ------------
void MotorB_Init(uint16_t ENB_Pin, GPIO_TypeDef* ENB_Port, uint16_t IN3_Pin, GPIO_TypeDef* IN3_Port, uint16_t IN4_Pin, GPIO_TypeDef* IN4_Port);
void MotorB_Enable(void);
void MotorB_Disable(void);
void MotorB_Forward(void);
void MotorB_Backward(void);
void MotorB_Left(void);
void MotorB_Right(void);
void MotorB_Brake(void);
void MotorB_Coast(void);

// --------- Functions for 2 motors ------------
void Motors_Init(uint16_t ENA_Pin, GPIO_TypeDef* ENA_Port, uint16_t IN1_Pin, GPIO_TypeDef* IN1_Port, uint16_t IN2_Pin, GPIO_TypeDef* IN2_Port,
				 uint16_t ENB_Pin, GPIO_TypeDef* ENB_Port, uint16_t IN3_Pin, GPIO_TypeDef* IN3_Port, uint16_t IN4_Pin, GPIO_TypeDef* IN4_Port);
void Motors_Enable(void);
void Motors_Disable(void);
void GoForward(void);
void GoBackward(void);
void TurnLeft(void);
void TurnRight(void);
void Brake(void);
void Coast(void);


uint16_t ENA_PIN, IN1_PIN, IN2_PIN, ENB_PIN, IN3_PIN, IN4_PIN;

GPIO_TypeDef *ENA_PORT, *IN1_PORT, *IN2_PORT, *ENB_PORT, *IN3_PORT, *IN4_PORT;


#endif
