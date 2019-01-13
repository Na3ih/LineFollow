/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/
/* This is the simple code for LineFollower using 2 photoresistors and L298N H-Bridge.
 * Prepared and tested on STM32F103C8T6 (blue pill).
 * It use a Standard Peripherial Library and also include a L298N functions library.
 * Works fine!
 */

#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"
#include<stdio.h>

#include "STM32_L298N_SPL.h"

/* - - - - - - - PINOUT - - - - - - - - */
static const uint16_t ena_pin = GPIO_Pin_8;
static const uint16_t in1_pin = GPIO_Pin_10;
static const uint16_t in2_pin = GPIO_Pin_1;

static GPIO_TypeDef * ena_port = GPIOB;
static GPIO_TypeDef * in1_port = GPIOB;
static GPIO_TypeDef * in2_port = GPIOB;

static const uint16_t enb_pin = GPIO_Pin_9;
static const uint16_t in3_pin = GPIO_Pin_7;
static const uint16_t in4_pin = GPIO_Pin_6;

static GPIO_TypeDef * enb_port = GPIOB;
static GPIO_TypeDef * in3_port = GPIOA;
static GPIO_TypeDef * in4_port = GPIOA;


/*  - - - - -  global variables  - - - - */

#define ADC_CHANNELS_NBR 2

volatile uint16_t adc_values[ADC_CHANNELS_NBR];		// memory buffer for adc measurement values ([0] - left sensor)

int prog = 1500;		// adc units threshold for sensing white or red

uint16_t pwm = 7000;	// motors speed


/* - - - - - functions declarations - - - - - */
void RCC_Config(void);
void DMA_Config(void);
void ADC_Config(void);

void TIMER_Config(void);
	void TIMER_GPIO_Config(void);
	void TIMER_CHANNELS_Config(uint16_t pulse);		// pwm
/* ===================== MAIN ============================= */

int main(void)
{
	RCC_Config();
	DMA_Config();
	ADC_Config();

	Motors_Init(ena_pin, ena_port, in1_pin, in1_port, in2_pin, in2_port, enb_pin, enb_port, in3_pin, in3_port, in4_pin, in4_port);
	TIMER_Config();		// pwm on enable pins

	while(1)
	{
		if((adc_values[0] > prog) && (adc_values[1] > prog))  GoForward(); 								// adc_values[0] - left sensor
		else if((adc_values[0] < prog) && (adc_values[1] > prog)) { MotorA_Brake(); MotorB_Forward();} 	// turn left
		else if((adc_values[0] > prog) && (adc_values[1] < prog)) { MotorB_Brake(); MotorA_Forward();}	// turn right
		else Brake();
	}
}

/* ====================================================================== */
/* =======================  FUNCTIONS  =================================== */

/* - - - - - - - - - - -  Clock Configurations  - - - - - - - - - - - - - */

void RCC_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}


/* - - - - - - - - - - -  ADC Measuring by DMA  - - - - - - - - - - - - - */
void DMA_Config(void)
{
	DMA_InitTypeDef dma;
	DMA_StructInit(&dma);
	dma.DMA_PeripheralBaseAddr = (uint32_t)&ADC1 -> DR;
	dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				// sta³y adres (ten sam ADC) wiêc inkrementacja wy³¹czona
	dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	// uint16_t
	dma.DMA_MemoryBaseAddr = (uint32_t)adc_values;
	dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	dma.DMA_DIR = DMA_DIR_PeripheralSRC;
	dma.DMA_BufferSize = ADC_CHANNELS_NBR;
	dma.DMA_Mode = DMA_Mode_Circular;
	DMA_Init(DMA1_Channel1, &dma);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

void ADC_Config(void)
{
	ADC_InitTypeDef adc;
	ADC_StructInit(&adc);
	adc.ADC_ContinuousConvMode = ENABLE;			// praca ci¹g³a
	adc.ADC_ScanConvMode = ENABLE;					// wiele kana³ów
	adc.ADC_NbrOfChannel = ADC_CHANNELS_NBR;
	adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	// w³¹czymy sami programowo
	ADC_Init(ADC1, &adc);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);		//ustawienia parameytrów kana³ów
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);

	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));

	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		// programowe uruchomienie
}


/* - - - - - - - - -  PWM controlled enable pins for speed regulation  - - - - - - - - - - - */
void TIMER_CHANNELS_Config(uint16_t pulse)
{
	TIM_OCInitTypeDef chanel3;
	TIM_OCStructInit(&chanel3);
	chanel3.TIM_OCMode = TIM_OCMode_PWM1;
	chanel3.TIM_OutputState = TIM_OutputState_Enable;
	chanel3.TIM_OCPolarity = TIM_OCPolarity_High;
	chanel3.TIM_Pulse = pulse;
	TIM_OC3Init(TIM4, &chanel3);

	TIM_OCInitTypeDef chanel4;
	TIM_OCStructInit(&chanel4);
	chanel4.TIM_OCMode = TIM_OCMode_PWM1;
	chanel4.TIM_OutputState = TIM_OutputState_Enable;
	chanel4.TIM_OCPolarity = TIM_OCPolarity_High;
	chanel4.TIM_Pulse = pulse;
	TIM_OC4Init(TIM4, &chanel4);
}

void TIMER_GPIO_Config(void)
{
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;

	gpio.GPIO_Pin = ena_pin;
	GPIO_Init(ena_port, &gpio);

	gpio.GPIO_Pin = enb_pin;
	GPIO_Init(enb_port, &gpio);
}

void TIMER_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseInitTypeDef timer;
	TIM_TimeBaseStructInit(&timer);
	timer.TIM_ClockDivision = TIM_CKD_DIV1;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Prescaler = 64 - 1;
	timer.TIM_Period = 20000 - 1;
	TIM_TimeBaseInit(TIM4, &timer);

	TIMER_GPIO_Config();
	TIMER_CHANNELS_Config(pwm);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);		// buforowanie kana³u 1
	TIM_ARRPreloadConfig(TIM4, ENABLE);						// buforowanie licznika

	TIM_Cmd(TIM4, ENABLE);									// zezwolenie na przerwanie
}


/* ============================= END FILE =============================== */
