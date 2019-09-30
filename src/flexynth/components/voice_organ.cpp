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
	FILE: voice_organ.cpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
              
*/
#include <cstdint>
#include "voice_organ.hpp"

#ifdef TESTBENCH
#include <iostream>
#endif
namespace synth
	{
	namespace voice
		{
		/** Slope Values for AR Envelope. */
		constexpr uint32_t clean_cut_value = 5000;
		/** Slope Values for AR Envelope. */
		constexpr uint32_t cleanest_cut_value = 2000;
	
		const cc::value_t *voice_organ::_drawbars;
		const uint32_t *voice_organ::_phase_table;

		voice_organ::voice_organ()
			{
			envelope.set_speed(env::env_feature_t::ATTACK,clean_cut_value);
			envelope.set_speed(env::env_feature_t::RELEASE,clean_cut_value);

			#ifdef TESTBENCH
			std::cout << "voice_organ::voice_organ() INFO: Voice Created!" << std::endl;
			#endif
		
			}
	
		void voice_organ::init(const cc::value_t *drawbar_vector,const uint32_t *phase_table)
			{
			_drawbars = drawbar_vector;
			_phase_table = phase_table;
			}

		inline void voice_organ::activate(uint8_t note)
			{
			if(is_active() == true)
				{
				//We play a new note if we are active.
				//return;
				}
			envelope.attack();
			_is_active = true;
			my_note = note;
			}
		
		inline void voice_organ::deactivate()
			{
			envelope.release();
			}
		
		inline bool voice_organ::is_active()
			{
			return _is_active;
			}
		
		void voice_organ::set_attack(uint32_t value)
			{
			envelope.set_speed(env::env_feature_t::ATTACK,value);
			}
		void voice_organ::set_release(uint32_t value)
			{
			envelope.set_speed(env::env_feature_t::RELEASE,value);
			}
				
		inline sample_t voice_organ::get_sample()
			{
			if(_is_active == false)
				{
				return static_cast<voice::sample_t>(0ul);
				}
		
			int i = 9;
			voice::sample_t sample = 0;
			while(i)
				{
				//sample += get_tonewheel(--i);
				//TEST IF WE CAN STORE IN TABLE A WAVE BETTER THAT 8 BIT
				sample += get_tonewheel(--i);
				}
		
			const uint8_t env_level = static_cast<uint8_t>(envelope.get_level());

			//Check activity of envelope generator.
			_is_active = envelope.is_running();
			#ifdef TESTBENCH
			if(sample!=0)
			std::cout << "voice_organ::get_sample() INFO: Got Sample: " << std::hex << sample << std::endl;
			#endif
			return static_cast<voice::sample_t>(sample*env_level);
			}
	
		inline voice::sample_t voice_organ::get_tonewheel(uint8_t tone)
			{
			const uint8_t tonewheel_note = tables::switchbox[my_note*9 + tone];

			const uint16_t wave_index = tables::phase_to_wave_index(_phase_table[tonewheel_note]);
		
			const voice::sample_t wave_value = static_cast<voice::sample_t>(tables::wave_sine[wave_index]);
				
			const uint8_t drawbar_value = static_cast<uint8_t>(_drawbars[tone]);

			return static_cast<voice::sample_t>(wave_value*drawbar_value);
			}
		}
	}