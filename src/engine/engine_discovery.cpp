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
	FILE: engine_discovery.cpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
     
*/

#include "engine_discovery.hpp"
#include "stm32f4xx_gpio.h"
#include "synth_organ.hpp"
#include "cc_base.hpp"
#include "midi_base.hpp"


namespace engine
	{
	engine_discovery::engine_discovery()
		{

		}

	bool engine_discovery::init()
		{
		
		output.init();
		//drawbars.init();
		midi.init();
		return true;
		}

		

	void engine_discovery::run()
		{													
		synth::synth_organ synthesizer;

		
		output.enable();		
		/*
		midi::command_t fixed;
		fixed.status = midi::status_t::NOTE_ON;
		for (int i = 0; i < 1; i++)
			{
			fixed.data = 47+i;
			synthesizer.push_midi_cmd(fixed);
			}
		*/
		do
			{
			
			auto cmd = midi.get_command();
			if(cmd.status == midi::status_t::NOTE_ON)
				{
				GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
				}
			if(cmd.status == midi::status_t::NOTE_OFF)
				{
				GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
				}
			synthesizer.push_midi_cmd(cmd);
			//Synthesize samples and fill the buffer.
			aout_if::sampling_action_t action;
			/* cs43l22 has a frame that is L+R. We use it in mono.*/
			constexpr uint32_t FEAT_FRAME_BUFF_SIZE = aout_if::FEAT_BUFF_SIZE/2;
			for(uint32_t i=0;i<FEAT_FRAME_BUFF_SIZE;i++)
				{
				auto const sample = synthesizer.synthesize_sample();
				output.push_sample(sample);
				}
			
			//Turn on orange led while we wait
			//ORANGE LED Represents the time we have to spare in midi processing.
			GPIO_SetBits(GPIOD,GPIO_Pin_13);
			while((action = output.check_deadline()) == aout_if::sampling_action_t::WAIT);

			//Turn off organge led.
			GPIO_ResetBits(GPIOD,GPIO_Pin_13);
			
			//Show if we met the real-time deadline.
			if(action == aout_if::sampling_action_t::CONTINUE)
				{
				//GPIO_SetBits(GPIOD,GPIO_Pin_12);
				//GPIO_ResetBits(GPIOD,GPIO_Pin_14);
				}
			else
				{
				//GPIO_SetBits(GPIOD,GPIO_Pin_14);
				//GPIO_ResetBits(GPIOD,GPIO_Pin_12);
				}
			}while(1);

		return;
		}
	}