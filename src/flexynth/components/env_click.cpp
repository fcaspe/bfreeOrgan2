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
	FILE: env_click.cpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
           
*/

#include <cstdint>
#include "env_click.hpp"

#ifdef ENV_DEBUG
#include <iostream>
#endif
namespace synth
	{
	namespace env
		{

		inline void env_click::set_speed(const env_feature_t feat,uint32_t speed)
			{
			return;
			}

		inline uint8_t env_click::get_level()
			{
			if(is_running() == false)
				{
				return 0;
				}
			else
				{
				update_registers();
				return static_cast<uint8_t>(current_register);
				}
			}

		inline void env_click::attack()
			{
			_is_running = true;
			click_wave_idx = 0;
			state = env_feature_t::ATTACK;
			current_register = 0ul;
			}

		inline void env_click::release()
			{
			click_wave_idx = CLICK_LEN;
			state = env_feature_t::RELEASE;
			}

		inline void env_click::update_registers()
			{
			if(state == env_feature_t::SUSTAIN)
				{
				}
			else if(state == env_feature_t::ATTACK)
				{
				if(click_wave_idx == CLICK_LEN)
					{
					state = env_feature_t::SUSTAIN;
					current_register = 255;
					}
				else
					{
					current_register = static_cast<uint32_t>(click_wave[click_wave_idx++]);
					#ifdef ENV_DEBUG
					//std::cout << "env_click::update_registers() current_register = " << std::hex << current_register << std::endl;
					//std::cout << std::hex << current_register << " ";
					#endif
					}
				}
			else if(state == env_feature_t::RELEASE)
				{
				if(click_wave_idx == 0)
					{
					current_register = 0ul;
					_is_running = false;
					}
				else
					{
					current_register = static_cast<uint32_t>(click_wave[click_wave_idx--]);
					}
				}
			return;
			}
	
		env_click::env_click()
			{
			click_wave_idx = 0;
			}
		
		}
	}
