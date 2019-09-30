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
	FILE: voice_organ.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
              
*/

/** @file voice_organ.hpp
    @brief Synchronous Organ Voice class header file.
	@author Franco Caspe
	@date Documented - 21 Mar 2019.
*/
#include "voice_base.hpp"
#include "tables.hpp"
#include "env_ar.hpp"
#include "env_click.hpp"
#include "cc_base.hpp"
namespace synth
	{	
	namespace voice
		{
		/** @brief Synchronous organ voice class for bfreeOrgan2.
		*/
		class voice_organ : public voice_base
			{
			public:
			/** 
			* An independent init method to bind CC controllers and the external phase table.
			* @todo The method input should be a reference/pointer to an std::array of 9 values.
			*/
			static void init(const cc::value_t *drawbar_vector,const uint32_t *phase_table);
			void activate(uint8_t note) override;
			void deactivate() override;
			bool is_active() override;
		
			/** @details Each voice sample is 9*(21bits tonewheel WORD)*(7bit envelope WORD) = 
			*   4 + 21 + 7 = 32 bits wide. 
			*/
			voice::sample_t get_sample() override;
	
			voice_organ();
			
			void set_attack(uint32_t value);
			void set_release(uint32_t value);

			private:
			#ifdef CLICK_TEST
			/** 
			* The click generator.
			*/
			env::env_click envelope;
			#else
			/** 
			* The AR envelope
			*/
			env::env_ar envelope;
			#endif
			/** 
			* Shared drawbars register between organ voices.
			*/	
			static const cc::value_t *_drawbars;
			/** 
			* Shared phase table between organ voices.
			*/ 
			static const uint32_t *_phase_table;
			/** 
			* Gets a sample from a single tonewheel.
			*
			* @detail Each tonewheel sample is (14bit table WORD) * (7 bit drawbar WORD) = 14 + 7 = 21 bits.
			*/	
			voice::sample_t get_tonewheel(uint8_t tone);
			};

		}
	}