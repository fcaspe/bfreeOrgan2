/*
*****************************************************************************
    ___  _                                        _						
   / __)| |                _     _           _   | |						
 _| |__ | |  _____  _   _ | |___| | ____   _| |_ | |__						
(_   __)| | | ___ |( \ / )|  _____||  _ \ (_   _)|  _ \		 __    __  ____ 
  | |   | | | ____| ) X (  \ \____ | | | |  | |_ | | | |	(  )  (  )(  _ \
  |_|    \_)|_____)(_/ \_)  \_____)|_| |_|   \__)|_| |_|	/ (_/\ )(  ) _ (
					A FLEXIBLE SYNTHESIZER DEV LIBRARY		\____/(__)(____/
****************************************************************************										
	FILE: cc_drawbars.cpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0

*/
#include "cc_drawbars.hpp"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_gpio.h"

namespace engine
	{
	namespace cc_if
		{
		cc::value_t cc_drawbars::drawbar_register[9];
		cc_drawbars::cc_drawbars() 
			{
			}

		bool cc_drawbars::init() 
			{
			drawbar_sampling_config(drawbar_register);
			return true;
			}

		cc::value_t* cc_drawbars::get_bind_addr()
			{
			return drawbar_register;
			}

		void cc_drawbars::enable()
			{
			DMA_Cmd(DMA2_Stream0, ENABLE);
			ADC_DMACmd(ADC1, ENABLE);
			ADC_Cmd(ADC1, ENABLE);
			TIM_Cmd(TIM3,ENABLE);
			}
	
		void cc_drawbars::disable()
			{
			TIM_Cmd(TIM3,DISABLE);
			ADC_Cmd(ADC1, DISABLE);
			ADC_DMACmd(ADC1, DISABLE);
			DMA_Cmd(DMA2_Stream0, DISABLE);
			}



		void drawbar_sampling_config(const cc::value_t *buffer_drawbars)
			  /* Conversion sequence definition. */
			  /*
			   * |---|----------|-------|---------------|
			   * |POS|DRAWBAR	|GPIO	|ADC CHANNEL	|
			   * |---|----------|-------|---------------|
			   * |0	 |16		|PB0	|8				|
			   * |1	 |5 1/3		|PC4	|14				|
			   * |2	 |8			|PA2	|2				|
			   * |3	 |4			|PC2	|12				|
			   * |4	 |2 2/3		|PC1	|11				|
			   * |5	 |2			|PA1	|1				|
			   * |6	 |1 3/5		|PA3	|3				|
			   * |7	 |1 1/3		|PC5	|15				|
			   * |8	 |1			|PB1	|9				|
			   * |---|----------|-------|---------------|
			   */
			{
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			ADC_InitTypeDef       ADC_InitStructure;
			ADC_CommonInitTypeDef ADC_CommonInitStructure;
			DMA_InitTypeDef       DMA_InitStructure;
			GPIO_InitTypeDef		GPIO_InitStructure;

			RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_DMA2 |
									RCC_AHB1Periph_GPIOC |
									RCC_AHB1Periph_GPIOB, 
									ENABLE);
	
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,
									ENABLE);
	
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,
									ENABLE);
	
			/* TIMER 3 (Drawbar Sampler) Configuration */

			const uint16_t PrescalerValue = 84000000/100000 -1; //100KHz

			TIM_TimeBaseStructure.TIM_Period = 100; //Periodo de disparo 1KHz
			TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
			TIM_TimeBaseStructure.TIM_ClockDivision = 0;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

			TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

			/* Select Output Trigger for TIM3 */
			TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update); // ADC will be Triggered by ADC_ExternalTrigConv_T3_TRGO

			/* -- TIM3 END CONFIG--*/

			/*	GPIO Config.  */

			/* Configura como entrada analógica los pines 1,2,4,5 del Puerto C, los 1,2,3 del Puerto A y 0,1 de Puerto B.*/

			GPIO_InitStructure.GPIO_Pin					= GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 ;
			GPIO_InitStructure.GPIO_Mode				= GPIO_Mode_AN;		//GPIO Analog Mode
			GPIO_InitStructure.GPIO_PuPd				= GPIO_PuPd_NOPULL;	//NoPull para entrada analogica
			GPIO_Init(GPIOC, &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin					= GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin					= GPIO_Pin_1 | GPIO_Pin_0;
			GPIO_Init(GPIOB, &GPIO_InitStructure);

			/*	DMA Config.		*/
			constexpr uint32_t ADC1_DR_ADDRESS = ((uint32_t)0x4001204C);
			DMA_DeInit(DMA2_Stream0);
			DMA_InitStructure.DMA_Channel				= DMA_Channel_0;
			DMA_InitStructure.DMA_PeripheralBaseAddr	= (uint32_t)ADC1_DR_ADDRESS;
			DMA_InitStructure.DMA_Memory0BaseAddr		= (uint32_t)buffer_drawbars;
			DMA_InitStructure.DMA_DIR					= DMA_DIR_PeripheralToMemory;

			DMA_InitStructure.DMA_BufferSize 			= 9;
			DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;
			DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
			DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
			DMA_InitStructure.DMA_MemoryDataSize     	= DMA_MemoryDataSize_Byte;

			DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;
			DMA_InitStructure.DMA_Priority 				= DMA_Priority_High;
			DMA_InitStructure.DMA_MemoryBurst 			= DMA_MemoryBurst_Single;
			DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
			DMA_InitStructure.DMA_FIFOThreshold 		= DMA_FIFOThreshold_Full;
			DMA_InitStructure.DMA_FIFOMode 				= DMA_FIFOMode_Disable;

			DMA_Init(DMA2_Stream0, &DMA_InitStructure);
			//DMA_Cmd(DMA2_Stream0, ENABLE); //MOVED TO ENABLE/DISABLE METHOD

			/*	ADC Config.		*/

			ADC_DeInit();
			ADC_CommonInitStructure.ADC_Mode			= ADC_Mode_Independent;
			ADC_CommonInitStructure.ADC_Prescaler		= ADC_Prescaler_Div2;				//PCLK2 ->divide PCLK2
			ADC_CommonInitStructure.ADC_DMAAccessMode	= ADC_DMAAccessMode_Disabled;	//Acceso DMA sirve solo en modo MULTI ADC
  

			//ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //Delay entre Conversiones


			ADC_CommonInit(&ADC_CommonInitStructure);


			ADC_InitStructure.ADC_Resolution			= ADC_Resolution_8b;
			ADC_InitStructure.ADC_ScanConvMode			= ENABLE;		//SCAN MODE OFF (escanea multiples canales)
			ADC_InitStructure.ADC_ContinuousConvMode	= DISABLE;
			ADC_InitStructure.ADC_ExternalTrigConv		= ADC_ExternalTrigConv_T3_TRGO;	// TIM3 TRIGGER EXTERNO
			ADC_InitStructure.ADC_ExternalTrigConvEdge	= ADC_ExternalTrigConvEdge_Rising; //Activo por Flanco positivo

			ADC_InitStructure.ADC_DataAlign				= ADC_DataAlign_Right;
			ADC_InitStructure.ADC_NbrOfConversion		= 9;	//Numero de Conversiones a realizar
																//En regular channel conversion sequence (REGULAR GROUP, VARIOS CANALES)
			ADC_Init(ADC1, &ADC_InitStructure);

			//ADC_DMACmd(ADC1, ENABLE); //MOVED TO ENABLE/DISABLE METHOD

			ADC_RegularChannelConfig(ADC1, ADC_Channel_8 ,1, ADC_SampleTime_3Cycles);
			ADC_RegularChannelConfig(ADC1, ADC_Channel_14,2, ADC_SampleTime_3Cycles);
			ADC_RegularChannelConfig(ADC1, ADC_Channel_2 ,3, ADC_SampleTime_3Cycles);
			ADC_RegularChannelConfig(ADC1, ADC_Channel_12,4, ADC_SampleTime_3Cycles);

			ADC_RegularChannelConfig(ADC1, ADC_Channel_11,5, ADC_SampleTime_3Cycles);
			ADC_RegularChannelConfig(ADC1, ADC_Channel_1 ,6, ADC_SampleTime_3Cycles);
			ADC_RegularChannelConfig(ADC1, ADC_Channel_3 ,7, ADC_SampleTime_3Cycles);
			ADC_RegularChannelConfig(ADC1, ADC_Channel_15,8, ADC_SampleTime_3Cycles);
			ADC_RegularChannelConfig(ADC1, ADC_Channel_9 ,9, ADC_SampleTime_3Cycles);

			ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

			//ADC_Cmd(ADC1, ENABLE); //MOVED TO ENABLE/DISABLE METHOD

			}
		}
	}