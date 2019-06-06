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
	FILE: efx_chorus.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
            
*/
#include <cstdint>
#include <array>
#include "efx_base.hpp"

/** @file efx_chorus.hpp
    @brief bfreeOrgan2 chorus effect unit header.
	@author Franco Caspe
	@date Documented - 20 Mar 2019.
*/
namespace synth
	{
	namespace efx
		{
		/**
		* Defines the chorus delay line lenght.
		*/
		constexpr uint32_t DELAY_LINE_SIZE = 128;

		/**
		* LFO Wave Length.
		* MUST have a POWER OF TWO SIZE.
		*/
		constexpr uint32_t LFO_WAVE_LEN = 32;
	
		/**
		* Specify the calling frequency for this effect.
		*/
		constexpr uint32_t SAMPLING_FREQ = 48000;

		/** @brief Basic chorus effect unit class.
		*
		*	@details Algorithm extracted from https://ccrma.stanford.edu/~dattorro/EffectDesignPart2.pdf
		*/
		class efx_chorus : public efx_base
			{
			public:
			efx::sample_t run_through(const synth::internal_sample_t input) override;
			void activate() override;
			void deactivate() override;
			/**
			* Updates the LFO Rate.
			* @param rate A 7bit rate word in Hz, codified as Q4:3
			*/
			void set_rate(uint8_t rate);
			/**
			* Updates the LFO Amplitude
			* @param amplitude a 7bit amp word.
			*/
			void set_amplitude(uint8_t amplitude);
			efx_chorus();
			private:
			/**
			* LFO Sine Wavetable.
			* format Q0:15 signed
			*/
			int lfo_sine[LFO_WAVE_LEN] = { 
			 0,  6392, 12539, 18203, 23169, 27244, 30272, 32137, 32766, 32137, 30273, 27245, 23171, 18206, 12541,  6394,
			 2, -6389,-12536,-18201,-23167,-27242,-30271,-32136,-32766,-32138,-30274,-27247,-23172,-18208,-12543, -6397};
			/**
			* Updates the delay line fractional tap position.
			*/
			void update_tap();
			/**
			* Pushes a new sample onto the delay line.
			* @param sample The sample to be pushed.
			*/
			void push_sample(const efx::sample_t sample);
			/**
			* Reads a sample from delay line at tap position.
			*/
			efx::sample_t tap_read_sample();
			/**
			* Decreases an index and wraps around the current delay line size.
			*/
			uint32_t dec_n_wrap_buffer(uint32_t value);
			/**
			* The delay line (We use it as a circular buffer).
			*/
			std::array <sample_t,DELAY_LINE_SIZE> delay_line;
			/**
			* The tap step.
			*/		
			uint32_t tap_step;
			/**
			* The tap accumulator.
			*/
			uint32_t tap_accumulator;
		
			/* Variables used in amplitude calculation */
		
			/**
			* The center of the delay line.
			* @details It's the same value as the amplitude applied to LFO
			*		   And, 2*tap_center = delay_line_wrap_value.
			*/
			uint32_t tap_center;

			/**
			* The tail is the index of the oldest sample in the circular buffer (that implements the delay line).
			*/
			uint32_t buffer_tail;
			#ifdef CHORUS_DEBUG
			uint32_t debug_counter;
			#endif
			};
		}
	}