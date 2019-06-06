/*
Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
*/

/** @file cc_tb.cpp
    @brief Drawbar CC emulator. Simply reads the values from a file.
	@author Franco Caspe
	@date Documented - 18 Mar 2019.
*/

#include <iostream>
#include <cstdint>
#include "cc_tb.hpp"
#include <fstream>

using namespace std;

namespace engine
	{
	namespace cc_if
		{
		cc_tb::cc_tb(bool debug):_debug(debug)
			{
			if(_debug)
				{
				cout << "cc_tb::cc_tb() INFO: Controller Created." << endl;
				}

			}

		bool cc_tb::init()
			{
			ifstream file("./tb_controls/drawbars.txt",ifstream::in);
			if(file.is_open() == false)
				{
				cout << "cc_tb::init() ERROR! Drawbar file (drawbars.txt) not found!" << endl;
				cout << "cc_tb::init() Enter Drawbar Values " << endl;
				for(int i=0;i<9;i++)
					{
					cout << i << ": ";
					scanf("%hhu",&cc_values[i]);
					}
				return true;
				}

			int i = 0;
			uint16_t value;
			while(file >> value && i<9)
				{
				cc_values[i] = static_cast<uint8_t>(value);
				if(_debug)
					{
					cout << "Drawbar: " << i << " = " << value << endl;
					}
				i++;
				}

			return true;
			}

		cc::value_t *cc_tb::get_bind_addr()
			{
			return cc_values;
			}
		}
	}