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
	FILE: env_click.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
          
*/

/** @file env_click.hpp
    @brief bfreeOrgan2 Click generator class header file.
	@author Franco Caspe
	@date Documented - 21 Mar 2019.
*/

#include "env_base.hpp"
#include <array>

namespace synth
	{
	namespace env
		{
		/** Click wave lenght definition */
		constexpr uint32_t CLICK_LEN = 158;
		/** Click wave that describes the envelope.
		*/
		const std::array<uint8_t,CLICK_LEN> click_wave =
			{255,255,
			0,0,0,0,
			255,255,
			0,0,
			255,255,
			0,0,
			255,255,
			0,0,0,0,
			255,255,255,255,
			0,0,0,0,0,0,0,0,
			255,255,255,255,
			0,0,
			255,255,255,255,255,255,255,255,255,255,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			255,255,255,255,255,255,
			0,0,
			255,255,
			0,0,0,0,
			255,255,
			0,0,0,0,0,0,0,0,0,0,
			255,255,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			255,255,
			0,0,0,0,0,0,
			255,255,
			0,0,0,0,0,0,0,0,0,0,
			255,255,
			0,0,0,0,
			255,255,255,255,
			0,0,0,0};

		/**	@brief Hack Envelope Generator class for clicking key action.
		*
		*	Roughly recreates the switch action over a tonewheel as the one described in the work
		*	https://doi.org/10.1121/1.5003796
		*	By romplaying a click wave, at sampling frequency.
		*	@todo We can try to modulate in frequency this click wave by adding a fractional phase accumulator and setting the attack/release speed according to the MIDI pitch of the note.
		*/
		class env_click : public env_base
			{
			public:
			void set_speed(const env_feature_t feat,uint32_t speed) override;
			uint8_t get_level() override;
			void attack();
			void release();
			env_click();
			private:
			/** Index of the click wave */
			uint32_t click_wave_idx;
			/** Updates the level and state envelope registers.
			*/
			void update_registers();
			};
	
		}
	}