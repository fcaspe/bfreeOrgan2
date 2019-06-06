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
	FILE: efx_dummy.cpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			
	LICENSE:                
*/

#include "efx_dummy.hpp"

namespace efx
	{
	efx_dummy::efx_dummy()
		{
		}
	
	inline efx_dummy::activate()
		{
		_is_active = true;
		}
	
	inline efx_dummy::deactivate()
		{
		_is_active = false
		}
	
	inline sample_t efx_dummy::run_through(synth::internal_sample_t sample)
		{
		if(_is_active == false)
			{
			return (sample_t)sample*0x0FFF;
			}
		if(_is_active == true)
			{
			return (sample_t)sample*0x0FFF;
			}
		
		}
	
	}