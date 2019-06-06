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
	FILE: synth_organ.cpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
     
*/
 
#include "synth_organ.hpp"
#include <algorithm>

#ifdef TESTBENCH
#include <iostream>
#endif

#ifdef CHORUS_DEBUG
#include <iostream>
#endif

namespace synth
	{
	synth_organ::synth_organ()
		{
		std::for_each(cc_drawbars.begin(),
					cc_drawbars.end(),
					[](cc::value_t &vector_value){vector_value = 127;});
		//Initialize common variables to all voices.
		voice::voice_organ::init(static_cast<const cc::value_t*>(cc_drawbars.begin()),
								static_cast<const uint32_t*>(phase_table.data()));
		
		std::for_each(phase_table.begin(),
					phase_table.end(),
					[](uint32_t &phase){phase = 0;});
		#ifdef CHORUS_TEST
		fx.activate();
		#endif
		}

	void synth_organ::push_midi_cmd(const midi::command_t &cmd)
		{
		if(cmd.status == midi::status_t::NODATA)
			{
			#ifdef TESTBENCH
			//std::cout << "synth_organ::push_midi_cmd(): Got NODATA." << std::endl;
			#endif
			return;
			}
		/* Listen only on MIDI Channel 1 */
		if(cmd.channel != 0)
			{
			return;
			}
		if(cmd.status == midi::status_t::NOTE_ON)
			{
			auto osc_iterator = std::find_if(oscillators.begin(),oscillators.end(),
											[](auto &osc) -> bool
											{return !osc.is_active();});
			if(osc_iterator != oscillators.end())
				{
				osc_iterator->activate(cmd.data - tables::FEAT_LOWEST_GEAR);
				}
			#ifdef TESTBENCH
			std::cout << "synth_organ::push_midi_cmd(): Activate oscillator. Note: " << cmd.data << std::endl;
			#endif
			return;
			}
		if(cmd.status == midi::status_t::NOTE_OFF)
			{
			#ifdef TESTBENCH
			std::cout << "synth_organ::push_midi_cmd(): Deactivate oscillator." << cmd.data << std::endl;
			#endif
			const uint8_t osc_note = cmd.data - tables::FEAT_LOWEST_GEAR;
			auto osc_iterator = std::find_if(oscillators.begin(),oscillators.end(),
								[&](auto &osc) -> bool
								{return osc.get_note() == osc_note ? true : false;});
			
			if(osc_iterator != oscillators.end())
				{
				osc_iterator->deactivate();
				}
			return;
			}
		if(cmd.status == midi::status_t::CONTROLLER_CHANGE)
			{
			
			if(cmd.data >= CC_ID_DRAWBAR_16 && cmd.data <= CC_ID_DRAWBAR_1)
				{
				const uint8_t drawbar_position = cmd.data - 20;
				cc_drawbars[drawbar_position] = cmd.value;
				}
			if(cmd.data == CC_ID_CHORUS_ON_OFF)
				{
				if(cmd.value <= 63)
					{
					fx.deactivate();
					}
				else
					{
					fx.activate();
					}
				}
			if(cmd.data == CC_ID_CHORUS_RATE)
				{
				fx.set_rate(cmd.value);
				}
			if(cmd.data == CC_ID_CHORUS_DEPTH)
				{
				fx.set_amplitude(cmd.value);
				}
			return;
			}
		}
	void synth_organ::push_pb_cmd(const pushbutton::command_t &cmd)
		{
		//Not implemented ...
		return;
		}
		
	synth::sample_t synth_organ::synthesize_sample() 
		{
		synth::internal_sample_t current_sample = 0;
		for(auto &osc : oscillators)
			{
			if(osc.is_active()) 
				{
				current_sample += osc.get_sample();
				}
			}
		
		#ifdef TESTBENCH
		if(current_sample!=0)
		std::cout << "synth_organ::synthesize_sample(): Got sample: " << current_sample << std::endl;
		#endif
		update_phase_table();
		const efx::sample_t fx_output = fx.run_through(current_sample);
		#ifdef CHORUS_DEBUG
		std::cout << "synth_organ::synthesize_sample(): Got chorus sample: " << (int16_t)(fx_output>>24) << std::endl;
		#endif
		return to_external_sample(fx_output);
		}
		
	inline void synth_organ::update_phase_table()
		{
		std::transform (phase_table.begin(),	//First element start
						phase_table.end(),		//First element end
						tables::step_table.begin(),		//Second element start
						phase_table.begin(),	//Result element start
						std::plus<uint32_t>());	//Binary Operator
		}
	
	inline synth::sample_t synth_organ::to_external_sample(synth::internal_sample_t sample)
			{
			return static_cast<synth::sample_t>(sample >> 20);
			}
	
	}