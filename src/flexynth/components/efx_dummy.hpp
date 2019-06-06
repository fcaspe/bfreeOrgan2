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
	FILE: efx_dummy.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			
	LICENSE:                
*/
#include "efx_base.hpp"

namespace efx
	{
	class efx_dummy : public efx_base
		{
		public
		sample_t run_through(synth::sample_t sample);
		void activate();
		void deactivate();

		};
	}