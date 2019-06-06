/*
****************************************************************
*     _      __                                           ____  
*    | |__  / _|_ __ ___  ___  ___  _ __ __ _  __ _ _ __ |___ \ 
*    | '_ \| |_| '__/ _ \/ _ \/ _ \| '__/ _` |/ _` | '_ \  __) |
*    | |_) |  _| | |  __/  __/ (_) | | | (_| | (_| | | | |/ __/ 
*    |_.__/|_| |_|  \___|\___|\___/|_|  \__, |\__,_|_| |_|_____|
*                                       |___/   
*	  A flexynh based synchronous drawbar organ synthesizer.
****************************************************************	
	FILE: efx_chorus.cpp
	VERSION: 0.2
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
          
*/

#include "efx_chorus.hpp"
#include <algorithm>

#ifdef CHORUS_DEBUG
#include <iostream>
#endif
namespace synth
	{
	namespace efx
		{
		/** Template for advance and wrap around a circular buffer */
		template <typename T>
		T inc_n_wrap_lfo(T value)
			{
			constexpr T lfo_wrap_value = LFO_WAVE_LEN -1;
			return (++value)&(lfo_wrap_value);
			}

		efx_chorus::efx_chorus()
			{
			tap_accumulator = 0;
			buffer_tail = 0ul;
			std::for_each(delay_line.begin(),
					delay_line.end(),
					[](auto &value){value = static_cast<sample_t>(0);});
			#ifdef CHORUS_DEBUG
			debug_counter = 0ul;
			#endif
			const uint8_t rate_1_hz = 0x08; //Rate codified in q4:3
			const uint8_t rate_8_hz = 0x08<<3; //Rate codified in q4:3
			set_rate(rate_1_hz);
			set_amplitude(127);
			}
	
		inline void efx_chorus::activate()
			{
			_is_active = true;
			}
	
		inline void efx_chorus::deactivate()
			{
			_is_active = false;
			}
		void efx_chorus::set_rate(uint8_t rate)
			{
			/**
			* Auto constant for delay line tap accumulation.
			*
			* @details: The LFO wavetable step si calculated with 16bits of fractional precision
			* that is: 13 bits (0x1FFFul) + 3 fractional bits from the LFO Rate defined as Q4:3
			* We compute the period of the lfo wave, the desired LFO rate and the sampling frequency.
			*/
			constexpr uint32_t TAP_STEP_K = (0x1FFFul*LFO_WAVE_LEN)/(SAMPLING_FREQ);

			tap_step = TAP_STEP_K*(rate&0x7F);
			}
		void efx_chorus::set_amplitude(uint8_t amplitude)
			{
			const uint8_t amp_7bit = amplitude & 0x7F;
		
			/* Compute new tap center */
			tap_center = ((amp_7bit*DELAY_LINE_SIZE)>>7)/2;
			}
	
		efx::sample_t efx_chorus::run_through(const synth::internal_sample_t input)
			{
			if(_is_active == false)
				{
				return static_cast<synth::internal_sample_t>(input);
				}
		
			/* Push a new sample in the circular buffer. */
			push_sample(static_cast<efx::sample_t>(input));
			/* Read the value pointed by the tap. */
			const efx::sample_t output = tap_read_sample();
			/* Update the tap position */
			update_tap();
			/* Wet and dry at 50% each */
			return output/2 + input/2;
			}
		inline efx::sample_t efx_chorus::tap_read_sample()
			{
			int const lfo_amplitude = (int)tap_center;
			auto const delay_line_wrap_value = tap_center*2+1;
		
			/* Get current tap position & value from delay line */
			const int current_tap_index = ((lfo_sine[tap_accumulator>>16]*lfo_amplitude)>>15) + tap_center;
			const int next_tap_index = ((lfo_sine[inc_n_wrap_lfo(tap_accumulator>>16)]*lfo_amplitude)>>15) + tap_center;
		
			/* Translate indexes from circular buffer to linear indexes in the buffer */
			const uint32_t current_output_idx = (buffer_tail + current_tap_index) % (delay_line_wrap_value);
			const uint32_t next_output_idx = (buffer_tail + next_tap_index) % (delay_line_wrap_value);
		
			/* Extract the fractional tap value and its difference from the unity */
			const uint16_t frac_idx = static_cast<uint16_t>(tap_accumulator&0xFFFF);
			const uint16_t frac_difference = static_cast<uint16_t>(0xfffful - frac_idx);
		
			/* Perform a linear interpolation with the tap position between the previous and the next sample */
			const efx::sample_t output = (delay_line[current_output_idx]>>16) * frac_difference +
										 (delay_line[next_output_idx]>>16) * frac_idx;

			#ifdef CHORUS_DEBUG
			std::cout << "efx_chorus::tap_read_sample(): SAMPLE "<< ++debug_counter <<std::endl;
			std::cout << "efx_chorus::tap_read_sample(): lfo wave idx is at: " << (tap_accumulator>>16) << " Next idx is: " << inc_n_wrap_lfo(tap_accumulator>>16) << std::endl;
			std::cout << "efx_chorus::tap_read_sample(): Tap is at: " << current_tap_index << " Next tap is: " << next_tap_index << std::endl;
			std::cout << "efx_chorus::tap_read_sample(): VALUES   : " << (delay_line[current_output_idx]>>24)  << "  And: " << (delay_line[next_output_idx]>>24) << std::endl;
			std::cout << "efx_chorus::tap_read_sample(): DIFF     : " << (frac_difference>>8)  << " FRAC: " << (frac_idx>>8) <<std::endl;
			std::cout << "efx_chorus::tap_read_sample(): Output: " << (int16_t)(output>>24) << std::endl;
			#endif
			return output;
			}
		inline void efx_chorus::push_sample(const efx::sample_t sample)
			{
			//We first update buffer_tail for the tap to read the fresh value from buffer when tap is zero.
			buffer_tail = dec_n_wrap_buffer(buffer_tail);
			//Push sample to circular buffer
			delay_line[buffer_tail] = sample;
			#ifdef CHORUS_DEBUG
			std::cout << "efx_chorus::push_sample(): Pushing: " << (int16_t)(delay_line[buffer_tail]>>24) << std::endl;
			#endif
			}
		inline void efx_chorus::update_tap()
			{
			constexpr uint32_t accumulator_wrap_value = (LFO_WAVE_LEN <<16) -1;
			tap_accumulator += tap_step;
			tap_accumulator &= accumulator_wrap_value;
			}
		inline uint32_t efx_chorus::dec_n_wrap_buffer(uint32_t value)
			{
			if(value == 0)
				{
				value = tap_center*2;
				}
			else
				{
				--value;
				}		
			return value;
			}
		}
	}