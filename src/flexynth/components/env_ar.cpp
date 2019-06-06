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
	FILE: env_ar.cpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
              
*/
#include <cstdint>
#include "env_ar.hpp"

#ifdef ENV_DEBUG
#include <iostream>
#endif
namespace synth
	{
	namespace env
		{

		inline void env_ar::set_speed(const env_feature_t feat,uint32_t speed)
			{
			feat_speed[static_cast<uint8_t>(feat)] = speed & full_ar_range;
			}

		inline uint8_t env_ar::get_level()
			{
			if(is_running() == false)
				{
				return 0;
				}
			else
				{
				const uint8_t current_level = static_cast<uint8_t>(current_register >> 13);
				update_registers();
				return current_level;
				}
			}

		inline void env_ar::attack()
			{
			_is_running = true;
			state = env_feature_t::ATTACK;
			current_register = 0ul;
			}

		inline void env_ar::release()
			{
			state = env_feature_t::RELEASE;
			}

		inline void env_ar::update_registers()
			{
			if(state == env_feature_t::SUSTAIN)
				{
				}
			else if(state == env_feature_t::ATTACK)
				{
				current_register += feat_speed[static_cast<uint8_t>(env_feature_t::ATTACK)];
				#ifdef ENV_DEBUG
				//std::cout << "env_ar::update_registers() current_register = " << std::hex << current_register << std::endl;
				std::cout << std::hex << current_register << " ";
				#endif
				if(current_register > full_ar_range)
					{
					current_register = full_ar_range;
					state = env_feature_t::SUSTAIN;
					}
				}
			else if(state == env_feature_t::RELEASE)
				{
				if(current_register < feat_speed[static_cast<uint8_t>(env_feature_t::RELEASE)])
					{
					current_register = 0ul;
					_is_running = false;
					}
				else
					{
					current_register -= feat_speed[static_cast<uint8_t>(env_feature_t::RELEASE)];
					}
				}
			return;
			}
	
		env_ar::env_ar()
			{
			}
		
		}
	}
