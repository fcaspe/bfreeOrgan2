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
	FILE: engine_discovery.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
     
*/
#include "engine_base.hpp"
#include "output_discovery.hpp"
#include "cc_drawbars.hpp"
#include "midi_usb.hpp"
namespace engine
	{
	class engine_discovery : public engine_base
		{
		public:
		bool init();
		void run();
		engine_discovery();
	
		private:

		aout_if::output_discovery output;
		cc_if::cc_drawbars drawbars;
		midi_if::midi_usb midi;
	
		};
	}