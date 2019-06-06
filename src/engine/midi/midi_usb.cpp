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
	FILE: midi_usb.cpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0


*/

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"

#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_midi_core.h"
#include "usb_dcd_int.h"

#include "midi_usb.hpp"

	/** USB Device Handle */
	__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

	/** MIDI Callback Structure declaration and definition */	
	MIDI_IF_Prop_TypeDef MIDI_fops = {engine::midi_if::MIDI_DataRx, engine::midi_if::MIDI_DataTx};

namespace engine
	{
	namespace midi_if
		{
		/** Automatic constant for fast buffer wrapping */
		constexpr uint32_t MIDI_BUFF_WRAP = FEAT_MIDI_BUFF_LEN - 1;

		/** Template for index wrapping */
		template <typename T>
		uint32_t inc_n_wrap(const T &value)
			{
			return (static_cast<uint32_t>(value) + 1)&MIDI_BUFF_WRAP;
			}

		template <typename T1,typename T2>
		uint32_t sum_n_wrap(const T1 &a, const T2 &b)
			{
			return (static_cast<uint32_t>(a) + static_cast<uint32_t>(b))&MIDI_BUFF_WRAP;
			}
	
		/** Static midi_usb members definition. */

		uint32_t midi_usb::buffer[FEAT_MIDI_BUFF_LEN];
		volatile uint16_t midi_usb::last_idx;
		volatile uint16_t midi_usb::current_idx;


		midi_usb::midi_usb()
			{
			}
	
		bool midi_usb::init()
			{
			// ------------- USB -------------- //
			/* Initialize the midi driver */
			usbd_midi_init_driver(&MIDI_fops);
			/* Make the connection and initialize to USB_OTG/usbdc_core */
			USBD_Init(&USB_OTG_dev,
						USB_OTG_FS_CORE_ID,
						&USR_desc,
						&USBD_MIDI_cb,
						&USR_cb);
		
			/* Reset indexes */
			last_idx = 0;
			current_idx = 0;
			return true;
			}


		midi::command_t midi_usb::get_command()
			{
			midi::command_t cmd;
			cmd.status = midi::status_t::NODATA;
			if(last_idx == current_idx)
				{
				return cmd;
				}
			const uint8_t *buffer_entry = reinterpret_cast<uint8_t*>(&buffer[last_idx]);
			
			cmd.channel = buffer_entry[1] & 0x0F;
			const uint8_t status_field = buffer_entry[1] & 0xF0;
			if(status_field == 0x90) 
				{
				cmd.status = midi::status_t::NOTE_ON;
				}
			if(status_field == 0x80) 
				{
				cmd.status = midi::status_t::NOTE_OFF;
				}
			if(status_field == 0xB0) 
				{
				cmd.status = midi::status_t::CONTROLLER_CHANGE;
				}
			cmd.data = buffer_entry[2];
			cmd.value = buffer_entry[3];
		
			/* Increase processed buffer index. */
			last_idx = inc_n_wrap(last_idx);
		
			return cmd;
		
			}
	

	//////////////// USB MIDI CALLBACK FUNCTIONS IMPLEMENTATION ///////////////////////////


		/** This is the callback function that is called whenever a midi message is recieved.
		 */
		uint16_t MIDI_DataRx(uint8_t *msg, uint16_t length)
			{
			/* Calculate next buffer idx, vaid after copying the data to the midi_usb buffer. */
			auto const next_idx = sum_n_wrap(midi_usb::current_idx,length/4);

			uint32_t i;
			uint32_t j = 0;
			const uint32_t* msg_X32 = (reinterpret_cast<uint32_t*>(msg));
			for(i = midi_usb::current_idx; i != next_idx; i = inc_n_wrap(i))
				{
				/* Copy chunks of 32 bits to the midi_usb buffer. */
				midi_usb::buffer[i] = msg_X32[j++];
				}
	
			/* Update current index position. */
			midi_usb::current_idx = next_idx;
			return 0;
			}

		/** This function is called in order to send a midi message over USB.
		 */
		uint16_t MIDI_DataTx(uint8_t *msg, uint16_t length)
			{
			uint32_t i = 0;
			while (i < length) {
				APP_Rx_Buffer[APP_Rx_ptr_in] = *(msg + i);
				APP_Rx_ptr_in++;
				i++;
				/* To avoid buffer overflow */
				if (APP_Rx_ptr_in == APP_RX_DATA_SIZE) {
					APP_Rx_ptr_in = 0;
				}
			}

			return USBD_OK;
			}
	
		} //END NAMESPACE MIDI_IF
	}
	/******************************************************************************/
	/*                 STM32F4xx Peripherals Interrupt Handlers                   */
	/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
	/*  available peripheral interrupt handler's name please refer to the startup */
	/*  file (startup_stm32f4xx.s).                                               */
	/******************************************************************************/

	#ifdef USE_USB_OTG_FS
	void OTG_FS_WKUP_IRQHandler(void)
		{
		if(USB_OTG_dev.cfg.low_power)
			{
			*(uint32_t *)(0xE000ED10) &= 0xFFFFFFF9 ;
			SystemInit();
			USB_OTG_UngateClock(&USB_OTG_dev);
		}
		EXTI_ClearITPendingBit(EXTI_Line18);
		}
	#endif

	/**
	  * @brief  This function handles EXTI15_10_IRQ Handler.
	  * @param  None
	  * @retval None
	  */
	#ifdef USE_USB_OTG_HS
	void OTG_HS_WKUP_IRQHandler(void)
		{
		if(USB_OTG_dev.cfg.low_power)
			{
			*(uint32_t *)(0xE000ED10) &= 0xFFFFFFF9 ;
			SystemInit();
			USB_OTG_UngateClock(&USB_OTG_dev);
			}
		EXTI_ClearITPendingBit(EXTI_Line20);
		}
	#endif

	/**
	  * @brief  This function handles OTG_HS Handler.
	  * @param  None
	  * @retval None
	  */
	#ifdef USE_USB_OTG_HS
	void OTG_HS_IRQHandler(void)
	#else
	void OTG_FS_IRQHandler(void)
	#endif
		{
	  USBD_OTG_ISR_Handler (&USB_OTG_dev);
		}

	#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
	/**
	  * @brief  This function handles EP1_IN Handler.
	  * @param  None
	  * @retval None
	  */
	void OTG_HS_EP1_IN_IRQHandler(void)
		{
	  USBD_OTG_EP1IN_ISR_Handler (&USB_OTG_dev);
		}

	/**
	  * @brief  This function handles EP1_OUT Handler.
	  * @param  None
	  * @retval None
	  */
	void OTG_HS_EP1_OUT_IRQHandler(void)
		{
	  USBD_OTG_EP1OUT_ISR_Handler (&USB_OTG_dev);
		}
	#endif
