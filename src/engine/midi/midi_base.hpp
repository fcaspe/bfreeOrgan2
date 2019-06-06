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
	FILE: midi_base.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0


*/

/** @file midi_base.hpp
    @brief Base class header file for the MIDI processing class.
	The MIDI processing class wraps around a specific MIDI driver and MIDI buffer to be implemented into the derived classes.
	This file defines the inferface methods and the MIDI message structure that's used in the upper layers of the synthesizer.
	@author Franco Caspe
	@date Documented - 18 Mar 2019.
*/

#include <cstdint>
#pragma once

/** @brief Midi Data Structures */
namespace midi
	{
		/**
		* @brief Parsed status enum for the midi messages.
		*/
	enum status_t : uint8_t
		{
		NODATA,
		NOTE_ON,
		NOTE_OFF,
		CONTROLLER_CHANGE,
		SYSEX_EXCEPTION
		};

		/**
		* @brief A simple struct to pass MIDI messages
		* @todo We haven't implemented a channel read yet . . .
		* @todo We can implement a new HARDWARE CC midi controller wrapper.
		* @todo Implement MIDI messages as unions of different structs with appropiate names.
		*/
	typedef struct
		{
		/** MIDI Message Status */
		status_t status;
		/** MIDI Channel */
		uint8_t channel;
		/** MIDI First 7bit byte data value (Second MIDI message byte) */
		uint8_t data;
		/** MIDI Second 7bit byte data value (Third MIDI byte)*/
		uint8_t value;
		}command_t;

	
	}

namespace engine
	{
	/** @brief Midi Processor 
	*/
	namespace midi_if
		{
			/** @brief Base Class for the MIDI processor.
			*
			* Wraps around a specific MIDI driver to be implemented as a derived class.
			*/
		class midi_base
			{
			public:
			/**
			* Used to initialize the MIDI processor.
			* @return Returns true if initialization was successful.
			*/
			virtual bool init() = 0;
			/**
			* Consumes one MIDI message from the MIDI buffer.
			* @return A valid MIDI message if found; an empty message 
			* (message.status = midi_status_t::NODATA) otherwise.
			*/	
			virtual midi::command_t get_command() = 0;
			protected:
			// A protected default constructor.
			midi_base() { }
			private:
			// Private non-implemented copy constructor.
			midi_base(const midi_base&) = delete;
			// Private non-implemented copy assignment operator.
			const midi_base& operator=(const midi_base&) = delete;

			};
		}
	}
