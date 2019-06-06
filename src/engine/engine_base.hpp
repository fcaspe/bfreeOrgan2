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
	FILE: engine_base.hpp
	VERSION: 0.1
	DATE: Feb 1st, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0


*/

/** @file engine_base.hpp
    @brief Base class file for the Synthesizer Engine.
	@author Franco Caspe
	@date Documented - 27 Feb 2019.
*/

/** @dir engine      
	@brief The engine directory contains the hardware and midi directories, and the current implementation of the Synthesizer Engine.
	@details All the contents of this directory, with the exception of the base classes are plattform dependent. 
*/

/** @dir hardware      
	@brief Contains the I/O wrapper and controller classes for the audio and 
	specific I/O pushbuttons and sliders that don't have a native MIDI interface assigned.
*/

/** @dir midi      
	@brief Contains one or several MIDI wrapper/driver classes.
*/


/** @brief Engine Scheme
*/
namespace engine
	{

	/** @brief Base Class for the Synthesizer Engine

		Provides the basic interface intended to run the engine right after booting up, usually right from main() function.
		The Engine defines the way (provides a bridge) for the Sythesizer Software to interact with the various I/O drivers available in the Synthesis Architecture.
		The Synthesizer Engine contains the main loop process that generates the audio, writes to output and manages the inputs.
		The Engine must be programmed for each Processor Architecture on which the selected Synthesizer Software has to be deployed.
	*/
	
	class engine_base
		{
		public:
		/**
		* Used to initialize the Peripheral Drivers.
		* @return Returns true if initialization was successful.
		*/
		virtual bool init() = 0; //Pure abstract. Mandatory interface for classes in inheritance.
		/**
		* Starts the Synthesizer and User Interface.
		* @return Should never return.
		*/
		virtual void run() = 0;
		protected:
		engine_base() {};

		};
	}