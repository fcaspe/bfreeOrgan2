/*
*****************************************************************************
    ___  _                                        _						
   / __)| |                _     _           _   | |						
 _| |__ | |  _____  _   _ | |___| | ____   _| |_ | |__						
(_   __)| | | ___ |( \ / )|  _____||  _ \ (_   _)|  _ \		 __    __  ____ 
  | |   | | | ____| ) X (  \ \____ | | | |  | |_ | | | |	(  )  (  )(  _ \
  |_|    \_)|_____)(_/ \_)  \_____)|_| |_|   \__)|_| |_|	/ (_/\ )(  ) _ (
					A FLEXIBLE SYNTHESIZER DEV LIBRARY		\____/(__)(____/
****************************************************************************										
	FILE: env_base.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0

*/
/** @file env_base.hpp
    @brief Envelope Generator class header file.
	@author Franco Caspe
	@date Documented - 21 Mar 2019.
*/
#pragma once
namespace synth
	{
	/** @brief Envelope Generator
	*/
	namespace env
		{
		/** Basic feature enumeration.
		* @details It is not mandatory to implement all envelope states.
		*/
		enum env_feature_t : uint8_t
			{
			ATTACK = 0,
			DECAY,
			SUSTAIN,
			RELEASE
			};

		/** @brief Base Class for an Envelope Generator.
		*
		* Provides the basic interface to apply an envelope to the output of a Synthesizer Component Voice. It should be implemented and called inside the voice.
		* The envelope generator works on a per-sample basis. All the internal components evolve in time when a sample is pushed in.
		*/
		class env_base
			{
			public:
			/**
			* Sets the speed of the feature selected (This is also called ENVELOPE RATE).
			* @todo We have to implement also a maximum value for each feature (The classic ENVELOPE LEVEL).
			*/
			virtual void set_speed(const env_feature_t feat,uint32_t speed) = 0;
			//virtual void set_level(const env_feature_t feat,uint32_t level) = 0;
			/** Gets the current envelope level and updates the state of the envelope.
			* @details This is usually employed to perform a voice sample multiplication.
			* @return The actual envelope level.
			*/
			virtual uint8_t get_level() = 0;
			/** Gate Control.
			* Resets the Envelope Generator to perform an attack.
			*/
			virtual void attack() = 0;
			/** Gate Control.
			* Commands the Envelope Generator to perform a release envelope.
			* @details The env will be active until the envelope level reaches zero, then it deactivates. (_is_running = false)
			*/
			virtual void release() = 0;
			/** Checks whether the envelope generator is performing an envelope
			* or it has already finished releasing the note.
			*/
			inline bool is_running()
				{
				return _is_running;
				}
			protected:
			env_base() 
				{
				_is_running = false;
				}
			/**
			* Array of speed values, one for each feature.
			*/
			uint32_t feat_speed[4];
			//uint32_t feat_level[4];		
			//Register current values of each feature
			/**
			* Phase accumulator for the current state.
			*/
			uint32_t current_register;
			/**
			* Current Envelope Generator State.
			*/	
			env_feature_t state;
			/** Determines whether the Envelope is being executed or it's idle.
			@details This variable is used primarly for determining when a release envelope has ended, and then the voice that was using it can be freed.
			*/
			bool _is_running;
	
			};
		}
	}