/*
Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
*/

/** @file midi_alsa.hpp
    @brief Derived ALSA-based MIDI processing class.
	@author Franco Caspe
	@date Documented - 18 Mar 2019.
*/

#include "midi_base.hpp"
#include <alsa/asoundlib.h>

namespace engine
	{
	namespace midi_if
		{
	
		/**  @brief ALSA-based MIDI processor for Linux.
		Utilizes ALSA library to open a MIDI port to receive MIDI events.
		*/
		class midi_seq : public midi_base
			{
			public:
			bool init();
			midi::command_t get_command();

			/** Class constructor */
			midi_seq(bool debug);
			~midi_seq();
			private:
			bool _debug;
			/** Handle for the ALSA MIDI port */
			static snd_seq_t *seq_handle;

			static int in_port;
			};
		}
	}