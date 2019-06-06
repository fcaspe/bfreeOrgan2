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
	FILE: output_discovery.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0

*/

#include "output_base.hpp"

namespace engine
	{
	namespace aout_if
		{
		extern "C"
			{
			void I2S_DMA_Config(const uint32_t buff_size,void* cbuffer0,void* cbuffer1);
	
			void DMA1_Stream7_IRQHandler();
			}
	
		constexpr uint32_t FEAT_BUFF_SIZE = 64;

		enum sampling_action_t : uint32_t
			{
			WAIT = 0,
			CONTINUE,
			DEADLINE_NOT_MET	
			};

		class output_discovery : public output_base
			{
			public:
			//Inits the timer that check deadline of synthesis at the intended sampling rate.
			bool init();
			//Starts the timer that controls sampling.
			void enable();
			output_discovery();

			inline void push_sample(const synth::sample_t sample)
				{
				//CS43L22 accepts two-complement data in a frame that is L + R.
				// We copy the same sample on both L + R channels.
				current_buffer[sample_idx] = static_cast<uint16_t>(sample);
				sample_idx++;
				current_buffer[sample_idx] = static_cast<uint16_t>(sample);
				sample_idx++;
				if(sample_idx == FEAT_BUFF_SIZE)
					{
					sample_idx = 0;
					//Swap the buffers
					current_buffer = 
						current_buffer == output_buffer_0 ? output_buffer_1 : output_buffer_0;
					}
				}

			inline sampling_action_t check_deadline()
				{
				//Check if it's the first time we enter after synthesis.
				if(trigger_sampling_complete == false)
					{
					//First time we enter, register entrance and check if we comply with the deadline
					trigger_sampling_complete = true;
			
					if(trigger_deadline == true)
						{
						//The deadline was triggered before we ended synthesizing!
						return sampling_action_t::DEADLINE_NOT_MET;
						}
					}
				else
					{
					if(trigger_deadline == true)
						{
						// The deadline was triggered after we finished synthesizing. 
						// Reset flags and loop.
						trigger_sampling_complete = false;
						trigger_deadline = false;
						return sampling_action_t::CONTINUE;
						}
					}
				//If the deadline is not triggered, we just have to wait.
				return sampling_action_t::WAIT;
				}

			private:
			volatile static bool trigger_deadline;
			bool trigger_sampling_complete;
			/*  These buffers may go in the stack if we dont declare them as static. 
				We need them to be in main ram otherwise the DMA cannot access them. */
			static uint16_t output_buffer_0[FEAT_BUFF_SIZE];
			static uint16_t output_buffer_1[FEAT_BUFF_SIZE];
			uint16_t *current_buffer;
			uint32_t sample_idx;
			friend 
			void DMA1_Stream7_IRQHandler();
			};
		}
	}