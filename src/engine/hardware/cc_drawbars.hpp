#include "cc_base.hpp"
#include <cstdint>

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
	FILE: cc_drawbars.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0

*/

namespace engine
	{
	namespace cc_if
		{
		extern "C"
			{
			void drawbar_sampling_config(const cc::value_t *drawbar_buffer);
			}
	

		class cc_drawbars : public cc_base
			{
			public:
			bool init() override;
			cc::value_t* get_bind_addr() override;
			cc_drawbars();
			void enable();
			void disable();
			private:
			static cc::value_t drawbar_register[9];
			};
		}
	}