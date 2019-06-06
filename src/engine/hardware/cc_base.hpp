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
	FILE: cc_base.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0

*/

/** @file cc_base.hpp
    @brief Plattform specific continuous controllers driver class file.
	@author Franco Caspe
	@date Documented - 18 Mar 2019.
*/
#include <cstdint>

#pragma once
/** @brief Continuous Controller Data Structures.
 */
namespace cc
	{
	/** Each continuous controller value is 8-bits and volatile. */
	typedef volatile uint8_t value_t;
	}

namespace engine
	{
	/** @brief Native Continuous Controllers interface.
	 */
	namespace cc_if
		{

		/** @brief Plattform specific continuous controller driver.
	
			We may have the need to implement additional controlling functionalities not supported over MIDI.
			In that case it's interesting to provide a driver class for our specific set of continuous controllers.
			The specific CC's may be directly binded to a vector whose address could be loaded directly to the Synthesizer Architecture
			for it to read its values directly. This method is quite efficient and doesn't require additional data buffering nor MIDI parsing.

		  @todo Should implemement a query method to read CC's driver capabilities.
		*/
		class cc_base
			{
			public:
			/** Initializes the peripherals/drivers associated to the controllers */
			virtual bool init() = 0;
			/** Gets the Controller Vector Pointer */
			virtual cc::value_t* get_bind_addr() = 0;
			protected:
			cc_base() {}
			};
		}
	}
