/*
Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
*/

/** @file midi_alsa.cpp
    @brief Derived ALSA-based MIDI processing class. Extracted from: http://fundamental-code.com/midi/midi-listen.c
	@author Franco Caspe
	@date Documented - 18 Mar 2019.
*/

#include "midi_alsa.hpp"
#include <alsa/asoundlib.h>
#include <iostream>

/** Checks for an error code returned by the ALSA Driver */
#define CHK(stmt, msg) if((stmt) < 0) {puts("ERROR: "#msg); exit(1);}

using namespace std;
namespace engine
	{
	namespace midi_if
		{
		snd_seq_t* midi_seq::seq_handle;

		int midi_seq::in_port;

		midi_seq::midi_seq(bool debug):_debug(debug)
			{

			}
	
		midi_seq::~midi_seq()
			{

			if(_debug)
				{
				cout << "midi_seq::~midi_seq() Destructor Called" << endl;
				}
			}
	
		bool midi_seq::init()
			{

			CHK(snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, SND_SEQ_NONBLOCK),
					"Could not open sequencer");
			

			CHK(snd_seq_set_client_name(seq_handle, "flexYnth MIDI controller"),
					"Could not set client name");
			CHK(in_port = snd_seq_create_simple_port(seq_handle, "midi_in",
						SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
						SND_SEQ_PORT_TYPE_APPLICATION),
					"Could not open port");
			return true;
			}
	
		midi::command_t midi_seq::get_command()
			{
			midi::command_t cmd;
			cmd.status = midi::status_t::NODATA;

			snd_seq_event_t *ev = NULL;
			snd_seq_event_input(seq_handle, &ev);
			if(!ev)	
				{
				return cmd;
				}

			cmd.channel = ev->data.control.channel;
			switch (ev->type) 
				{
				case SND_SEQ_EVENT_NOTEON:
					//fprintf(stderr, "Note On event on Channel %2d: %5d       \r",
					//ev->data.control.channel, ev->data.note.note);
					cmd.status = midi::status_t::NOTE_ON;
					cmd.data = ev->data.note.note;
					break;
				case SND_SEQ_EVENT_NOTEOFF: 
					//fprintf(stderr, "Note Off event on Channel %2d: %5d      \r",         
					//ev->data.control.channel, ev->data.note.note); 
					cmd.status = midi::status_t::NOTE_OFF;
					cmd.data = ev->data.note.note;  
					cmd.value = ev->data.note.velocity;        
					break;
				case SND_SEQ_EVENT_CONTROLLER:
					cmd.status = midi::status_t::CONTROLLER_CHANGE;
					cmd.data = ev->data.control.param; //Control ID
					cmd.value = ev->data.control.value; //Control Value
					break;
				}
				snd_seq_free_event(ev);

			return cmd;
			}
		}
	}