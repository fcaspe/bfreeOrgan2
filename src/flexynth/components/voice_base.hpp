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
	FILE: voice_base.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0

*/
/** @file voice_base.hpp
    @brief Synthesizer Voice class header file.
	@author Franco Caspe
	@date Documented - 21 Mar 2019.
*/
#include <cstdint>
namespace synth
	{
	/** @brief Synthesizer Voice 
	*/
	namespace voice
		{
		/** Native sample width definition.
		* @details Defined here to comply with the standard base class interface.
		*/
		typedef int32_t sample_t;
	
		/** Invalid note error code.
		*/
		constexpr uint8_t INVALID_NOTE = 0xFF;

		/** @brief Base Class for the a Synthesizer Voice. 
		*
		* A voice is a single signal path in your synth. Each signal path can generate a sound.
		* The Synthesizer Voice class should implement all the components of the audio chain that work in parallel inside the synthesizer.
		* @details Normally it would implement a DDS over a shared LUT and then the corresponding envelope, filter and specific voice processing if needed.
		*/
		class voice_base
			{
			public:
			
			/** Gate Control.
			* Activates the voice.
			* @param note A voice pitch number id (may be in MIDI standard or not).
			*/
			virtual void activate(uint8_t note) = 0;
			
			/** Gate Control.
			* Deactivates the voice.
			* @details A deactivated voice returns zero when get_sample() method is called.
			*/	
			virtual void deactivate() = 0;
			/** Checks voice state.
			* @return true if the voice is activated.
			*/	
			virtual bool is_active() = 0;
			/** Executes the signal path (if voice is active)
			* and returns the 
			* @return A voice sample in the Synthesizer Voice native format.
			*/	
			virtual sample_t get_sample() = 0;
			/** Gets voice's active note number register.
			* @return A voice pitch number id (may be in MIDI standard or not).
			*/			
			inline uint8_t get_note()
				{
				if(_is_active == true)
					{
					return my_note;
					}
				else
					{
					return INVALID_NOTE;
					}
				}
			protected:
			voice_base() 
				{
				_is_active = false;
				}
			/** Voice activity register.
			*/	
			bool _is_active;
			/** Voice note number id register.
			*/			
			uint8_t my_note;
			};
		}
	}