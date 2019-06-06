
/*
Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
*/

/** @file output_alsa.hpp
    @brief ALSA-based audio output controller.
	@author Franco Caspe
	@date Documented - 18 Mar 2019.
*/

#include <alsa/asoundlib.h>
#include "output_base.hpp"
namespace engine
	{
	namespace aout_if
		{
		/** @brief Audio output wrapper for ALSA.

		  Implements an ALSA audio output port (MONO) at 44100 Hz.
	  
		  @todo Move to 48Khz, since this will be the frequency also in the stm32f4 version.
		*/
	
		class output_tb : public output_base
			{
			public:
			bool init();
			void push_sample(synth::sample_t sample);
			output_tb(uint32_t buffer_sizeX8,bool debug);
			~output_tb();
			private:
			snd_pcm_t *pcm_handle;

			unsigned int rate; /** Sample rate */
			int periods; /** Number of periods on which the ring buffer is composed*/
			snd_pcm_uframes_t periodsize; /** Periodsize (bytes) */
			int16_t *period_buffer; /** Period buffer pointer */
			uint32_t frame_counter;
	
			bool _debug;
			};
		}
	}