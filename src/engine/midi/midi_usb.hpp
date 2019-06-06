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
	FILE: midi_usb.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0


*/

#include "midi_base.hpp"
#include <cstdint>

#include "usbd_midi_core.h"
#include "usbd_conf.h"

/** Assign C linking to ISR functions */
extern "C"
	{
	#ifdef USE_USB_OTG_FS
	void OTG_FS_WKUP_IRQHandler(void);
	#endif
	#ifdef USE_USB_OTG_HS
	void OTG_HS_WKUP_IRQHandler(void)
	#endif
	#ifdef USE_USB_OTG_HS
	void OTG_HS_IRQHandler(void);
	#else
	void OTG_FS_IRQHandler(void);
	#endif
	#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
	void OTG_HS_EP1_IN_IRQHandler(void);
	void OTG_HS_EP1_OUT_IRQHandler(void);
	#endif
	}

/** Externalize Callback Structure, since it's used in usbd_conf.h */
extern MIDI_IF_Prop_TypeDef MIDI_fops;
namespace engine
	{
	namespace midi_if
		{

		constexpr uint32_t FEAT_MIDI_BUFF_LEN = 64;
	
		/** MIDI transfer callback Functions Declaration */
		extern "C"
			{
			uint16_t MIDI_DataRx(uint8_t *msg, uint16_t length);
			uint16_t MIDI_DataTx(uint8_t *msg, uint16_t length);
			}
	
		/** @brief MIDI USB Device class wrapper

		Each MIDI NOTE_ON and NOTE_OFF message is received in a 32bit chunk.
		A MIDI RX callback is triggered by the host and goest trough the ST Microelectronics API
		The callback should copy as fast as it can the 32bit chunks into a buffer owned by this class.
		This class then processes the messages, one by one.
		*/
		class midi_usb : public midi_base
			{
			public:
			bool init() override;
		
			/** @brief Reads one 32bit entry in the buffer, and processes the MIDI message that is in the 24 LSB's. */
			midi::command_t get_command() override;
			midi_usb();
			private:
			static uint32_t buffer[FEAT_MIDI_BUFF_LEN]; /** USB MIDI Chunk buffer */
			volatile static uint16_t last_idx;		/** Last buffer index processed.*/
			volatile static uint16_t current_idx;	/** Current buffer tail.*/
			friend
			uint16_t MIDI_DataRx(uint8_t *msg, uint16_t length);

			};
		}
	}