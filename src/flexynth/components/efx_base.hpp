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
	FILE: efx_base.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0

*/

/** @file efx_base.hpp
    @brief Base class file for effect processor.
	@author Franco Caspe
	@date Documented - 27 Feb 2019.
*/

#include "flexynth_base.hpp"

namespace synth
		{
	/** @brief Effect Processing
	*/
	namespace efx
		{
		/** The internal sample width
		*
		* @details Must Coincide with Synthesizer Word Width, so that we can process natively the output of the synthesizer.
		* @todo Should we fix the word lenght of the synth and the efx ? I think so
		*/
		typedef int64_t sample_t;

		/** @brief Base Class for Effect Processing Routines.
		*
		* Provides the basic interface between the Synthesizer Architecture and the selected effect processing algorythm to be implemented.
		* The effect unit works on a per-sample basis. All the internal components evolve in time when a sample is pushed in.
		*/
		class efx_base
			{
			public:
			/**
		   * Load an audio sample and returns it processed.
		   * @param input audio sample formatted as synthesizer architecture.
		   * @return Processed audio sample in native effect format.
		   * @todo Should bind the input and ouput of this method to the synthesizer format.
		   */
			virtual sample_t run_through(const synth::internal_sample_t input) = 0;
		   /**
		   * Activates the effect unit. The sample then can be processed with run_through() method.
		   */	
			virtual void activate() = 0;
		   /**
		   * Deactivates the effect unit. The effects unit becomes a passtrough unit.
		   */	
			virtual void deactivate() = 0;
		
			protected:
			/**
			* Protected constructor means this is a common interface between all effect processing routines to be implemented.
			*/
			efx_base() 
				{
				_is_active = false;
				}
			/** Determines if the effect processing routine has to process or bypass the fed sample. */
			bool _is_active;
			};
		}
	}