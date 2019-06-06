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
	FILE: env_ar.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
              
*/

/** @file env_ar.hpp
    @brief bfreeOrgan2 Attack-Release envelope generator class header file.
	@author Franco Caspe
	@date Documented - 21 Mar 2019.
*/
#include "env_base.hpp"
namespace synth
	{
	namespace env
		{
		/** Defines the maximum resolution of the phase accumulator for the envelope states.
		*/
		constexpr uint32_t full_ar_range = 0xFFFFFul; //20-bit register resolution.
	
		/** @brief Attack-Release simple envelope generator.
		*
		* Used to supress organ toneweel "contact" clicking.
		*/
		class env_ar : public env_base
			{
			public:
			void set_speed(const env_feature_t feat,uint32_t speed) override;
			/** Returns the current envelope level.
			*
			* @details Evelope is 7 bits wide.
			*/
			uint8_t get_level() override;
			void attack();
			void release();
			env_ar();
			private:
			/** Updates the level and state envelope registers.
			*/
			void update_registers();
			};
	
		}
	}