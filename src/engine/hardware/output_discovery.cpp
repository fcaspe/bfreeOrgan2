
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
	FILE: output_discovery.cpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0

*/

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_spi.h"
#include "misc.h"
#include "output_discovery.hpp"
#include "cs43l22.h"
namespace engine
	{
	namespace aout_if
		{
		volatile bool output_discovery::trigger_deadline = false;
	
		uint16_t output_discovery::output_buffer_0[FEAT_BUFF_SIZE];
		uint16_t output_discovery::output_buffer_1[FEAT_BUFF_SIZE];
	
		output_discovery::output_discovery()
			{
			/** @todo move this to output_discovery::init() */
			trigger_sampling_complete = false;
			trigger_deadline = false;
			sample_idx = 0;
			current_buffer = output_buffer_0;
			}
	
		bool output_discovery::init()
			{
			/* Init the I2S & I2C peripheral for the cs43l2. */
			cs43l22_phy_startup();
			/* Configure the cs43l22 over I2C. */
			cs43l22_ctrl_startup();
	
			I2S_DMA_Config( FEAT_BUFF_SIZE,
							static_cast<void*>(output_buffer_0),
							static_cast<void*>(output_buffer_1));
			return true;
			}
		
		void output_discovery::enable()
			{	
			DMA_Cmd(DMA1_Stream7, ENABLE);
			I2S_Cmd(CODEC_I2S, ENABLE);
			}
	
		/**
		 * @brief ISR DMA1 Stream7 Transfer Complete.
		 * 
		 * It triggers when the whole buffer has been sent.
		 */
		void DMA1_Stream7_IRQHandler(void)
			{
			/* Test on DMA Stream Transfer Complete interrupt */
			if (DMA_GetITStatus(DMA1_Stream7, DMA_IT_TCIF7))
				{
				GPIO_ToggleBits(GPIOD,GPIO_Pin_15);
				output_discovery::trigger_deadline = true;	

				}
			/* Clear DMA Stream Transfer Complete interrupt pending bit */
			DMA_ClearITPendingBit(DMA1_Stream7, DMA_IT_TCIF7);

			}


		/*
		 * Configures DMA1 triggered by SPI3.
		 */
		void I2S_DMA_Config(const uint32_t buff_size,void* cbuffer0,void* cbuffer1)
			{
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);


			/*	DMA CONFIGURATION	*/

			DMA_InitTypeDef DMA_INIT;

			DMA_DeInit(DMA1_Stream7);
			DMA_INIT.DMA_Channel            = DMA_Channel_0;
			DMA_INIT.DMA_PeripheralBaseAddr = (uint32_t)&(CODEC_I2S->DR);
			DMA_INIT.DMA_Memory0BaseAddr    = (uint32_t)(cbuffer0);
			DMA_INIT.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
			DMA_INIT.DMA_BufferSize         = buff_size;
			DMA_INIT.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
			DMA_INIT.DMA_MemoryInc          = DMA_MemoryInc_Enable;
			DMA_INIT.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
			DMA_INIT.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
			DMA_INIT.DMA_Mode               = DMA_Mode_Circular;
			DMA_INIT.DMA_Priority           = DMA_Priority_High;
			DMA_INIT.DMA_FIFOMode           = DMA_FIFOMode_Disable;
			DMA_INIT.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
			DMA_INIT.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
			DMA_INIT.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;


			//Two functions must be called before calling the DMA_Init() function.
			//Extracted from: http://www.hanese.nl/STM32/stm32f2stdlibrary/html/group___d_m_a___group3.html

			DMA_DoubleBufferModeConfig(DMA1_Stream7, (uint32_t)cbuffer1,(uint32_t)cbuffer0);

			DMA_DoubleBufferModeCmd(DMA1_Stream7, ENABLE);

			DMA_Init(DMA1_Stream7, &DMA_INIT);

			//DMA Interrupt config.

			NVIC_InitTypeDef NVIC_InitStructure;

			/* Configure the Priority Group to 2 bits */
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

			/* Enable the DMA Stream IRQ Channel */
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			DMA_ITConfig(DMA1_Stream7, DMA_IT_TC, ENABLE);

			}
		}
	}