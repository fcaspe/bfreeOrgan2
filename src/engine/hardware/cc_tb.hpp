
/*
Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
*/
/** @file cc_tb.hpp
    @brief Drawbar CC emulator header file. Simply reads the values from a file.
	@author Franco Caspe
	@date Documented - 18 Mar 2019.
*/

#include "cc_base.hpp"

using namespace std;

namespace engine
	{
	namespace cc_if
		{
	
		/** @brief CC Drawbar Set emulator.
	
			Loads the CC vector with the drawbar values parsed from a file.
		*/
		class cc_tb : public cc_base
			{
			public:
			/** Class constructor */
			cc_tb(bool debug);
			bool init();
			cc::value_t *get_bind_addr();
	
			private:
			bool _debug;
			cc::value_t cc_values[9];

			};
		}
	}
