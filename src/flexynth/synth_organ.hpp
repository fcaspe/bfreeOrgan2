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
	FILE: synth_organ.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
            
*/

/** @file synth_organ.hpp
    @brief Synthesizer Architecture header file for bfreeOrgan2.
	@author Franco Caspe
	@date Documented - 20 Mar 2019.
*/
#include "flexynth_base.hpp"
#include "tables.hpp"
#include "midi_base.hpp"
#include "voice_organ.hpp"
#include "pb_base.hpp"
#include "cc_base.hpp"
#include "efx_chorus.hpp"

namespace synth
	{
	
	/**
	* Defines the polyphony of the synthesizer.
	*
	* @details Defined at 12 voices, so that it can coincide with the STM32F4 Discovery Capabilities.
	*/	
	constexpr uint32_t FEAT_POLIPHONY = 12;
	
	/** MIDI CC ID of lower drawbar
	*
	* @details Drawbars' MIDI CC ID are contiguous.
	*/
	constexpr uint8_t CC_ID_DRAWBAR_16 = 20;
	/** MIDI CC ID of higher drawbar */
	constexpr uint8_t CC_ID_DRAWBAR_1 = 28;
	/** MIDI CC ID of Chrous ENABLE/DISABLE */
	constexpr uint8_t CC_ID_CHORUS_ON_OFF = 70;
	/** MIDI CC ID of Chorus Rate */
	constexpr uint8_t CC_ID_CHORUS_RATE = 71;
	/** MIDI CC ID of Chorus Amplitude */
	constexpr uint8_t CC_ID_CHORUS_DEPTH = 72;

	/* TEST CC'S */
	/** MIDI CC ID of Voice Envelope Attack*/
	constexpr uint8_t CC_ID_ENV_ATTACK = 77;
	/** MIDI CC ID of Voice Envelope Release */
	constexpr uint8_t CC_ID_ENV_RELEASE = 78;
	
	/** @brief The bfreeorgan2 Synthesizer Architecture. 
	*
	*	This implements is a synchronous with click and chorus effect.
	*/
	class synth_organ : public flexynth_base
		{
		public:
		void push_midi_cmd(const midi::command_t &cmd) override;
		void push_pb_cmd(const pushbutton::command_t &cmd) override;
		
		/** @details Each Sample is (12 poliphony) * (32bit voice) = 4 + 32 bits = 36 bits wide.
		*/
		synth::sample_t synthesize_sample() override;
	
		/**
		* Constructor of the organ synthesizer class.
		*/
		synth_organ();
		private:
		/**
		* Updates the global phase table.
		* @details For this Synthesizer Architecture, the phase table is independent of the Voice Components, as this is a synchronous organ.
		*/
		void update_phase_table();
		/**
		* Converts from native sample width to output width.
		* @todo Move this to the output controller. We should do a template for bit shifting in output_base.hpp ;)
		*/
		synth::sample_t to_external_sample(synth::internal_sample_t sample);
		/**
		* The Organ Voices.
		*/
		std::array<voice::voice_organ,FEAT_POLIPHONY> oscillators;
		/**
		* The Phase Table.
		*/	
		std::array<uint32_t,tables::FEAT_NUMBER_OF_GEARS> phase_table;
		/**
		* An effect unit.
		*/	
		efx::efx_chorus fx;
		/**
		* A Continuous Controller Vector that registers each drawbar update.
		*/
		std::array<cc::value_t,9> cc_drawbars;			
		};
	}