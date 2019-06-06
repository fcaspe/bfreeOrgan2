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
	FILE: output_base.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0

*/

/** @file output_base.hpp
    @brief Audio output controller wrapper class.
	@author Franco Caspe
	@date Documented - 18 Mar 2019.
*/

#include "flexynth_base.hpp"

namespace engine
	{
	/** @brief Audio Output Controller
	 */
	namespace aout_if
		{

		/**  @brief Base Class for the audio output controller

		  Wraps around an output driver that usually controlls an output buffer.
		  and a sound card or DMA solution attached to a DAC.
		*/
		class output_base
			{
			public:
			/**
			* Used to initialize the output controller.
			* @return Returns true if initialization was successful.
			*/
			virtual bool init() = 0;
			/**
			* Sends one audio interleaved frame to the output.
			* Currently, only MONO audio is supported.
			* @param sample A frame which size is native to the Synthesis Architecture.
			* @todo We should probably expect a frame that is native to the hardware, and do the conversion outside this class.
			*/
			virtual void push_sample(synth::sample_t sample) = 0;
			protected:
			output_base() {}
			};
		}
	}