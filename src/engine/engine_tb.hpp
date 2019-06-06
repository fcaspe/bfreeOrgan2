/*
Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
*/

/** @file engine_tb.hpp
    @brief Derived class for an engine that runs over ALSA.
	@author Franco Caspe
	@date Documented - 18 Mar 2019.
*/

#include <iostream>
#include "engine_base.hpp"
#include "output_alsa.hpp"
#include "cc_tb.hpp"
#include "midi_alsa.hpp"

using namespace std;
namespace engine
	{
	
	/** @brief Linux Synthesizer Engine for ALSA */
	class engine_tb : public engine_base
	{
		public:
		bool init();
		void run();
		/** Class Constructor */
		engine_tb(bool debug);
	
		private:
		bool _debug;
		aout_if::output_tb output;
		midi_if::midi_seq sequencer;
	};
	}