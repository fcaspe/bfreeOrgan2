/*
Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
*/

/** @file engine_tb.cpp
    @brief Derived class for an engine that runs over ALSA.
	@author Franco Caspe
	@date Documented - 18 Mar 2019.
*/

#include <iostream>
#include <cstdint>
#include <chrono>
#include "engine_tb.hpp"
#include "synth_organ.hpp"

using namespace std;

namespace engine
	{
	engine_tb::engine_tb(bool debug):_debug(debug),
									 output(1024,debug),//Buffer size X8, Debug flag
									 sequencer(debug)	//Debug flag
		{

		if(_debug)
			{
			cout << "engine_tb::engine_tb() INFO: Engine Testbench Created!" << endl;
			}
		}

	bool engine_tb::init()
		{

		if(output.init() == false)
			{
			cout << "ENGINE: Error while starting output!" << endl;
			return false;
			}
		
		if(sequencer.init() == false)
			{
			cout << "ENGINE: Error while starting sequencer!" << endl;
			return false;
			}
		
		if(_debug)
			{
			cout << "engine_tb::init() INFO: Engine Testbench Init!" << endl;
			}
		return true;
		}

	void engine_tb::run()
		{
		synth::synth_organ synthesizer;
		
		if(_debug)
			{
			cout << "engine_tb::run() INFO: SYNTHESIZER INIT!" << endl;
			}

		while(1)
			{
			auto command = sequencer.get_command();
			//Check for stop condition.
			if(command.status == midi::status_t::SYSEX_EXCEPTION)
				{
				if(_debug)
					{
					std::cout << "engine_tb::run() Got Break Command!!" << std::endl;
					}
				break;
				}
			synthesizer.push_midi_cmd(command);
			for(uint32_t i=0;i<32;i++)
				{
				output.push_sample(synthesizer.synthesize_sample());
				}
			}
		
		return;
		}
	}