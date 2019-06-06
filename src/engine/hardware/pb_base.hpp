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
	FILE: pb_base.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			


Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0

*/

/** @file pb_base.hpp
    @brief Plattform Specific pushbuttons driver class file.
	@author Franco Caspe
	@date Documented - 18 Mar 2019.
*/
#pragma once
/** @brief Pushbutton Data Structures */
namespace pushbutton
	{
	/** @brief Button status enum */
	enum pb_status_t : uint8_t
			{
			NODATA,
			PUSHED_DOWN
			//HOLD
			//PUSHED UP
			};

	/** @brief Pushbutton structure to pass button commands. */
	typedef struct
			{
			/** The button status */
			pb_status_t status;
			/** The button ID */
			uint16_t pb_id;
			}command_t;
	}
namespace engine
	{
	/** @brief Native Pushbutton Controller
	*/
	namespace pushbutton_if
		{

		/** @brief Plattform Specific pushbuttons driver class.

			We may have the need to implement additional controlling functionalities that are not covered by the MIDI standard.
			In that case it's interesting to provide a driver class for our specific set of push buttons.
			@details NOT IMPLEMENTED YET.
		*/
			class pb_base
			{
				public:
				/** Initializes the peripherals/drivers associated to the button controller. */
				virtual bool init() = 0;
				/**
				* Consumes one Pushbutton Command from the buffer.
				* @return A valid command if found; an empty message 
				* (message.status = pb_status_t::NODATA) otherwise.
				*/	
				virtual pushbutton::command_t get_command() = 0;
				protected:
				pb_base() {}

			};
		}
	}
