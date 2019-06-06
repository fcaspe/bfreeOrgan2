
/*
Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0
*/

/** @file output_alsa.cpp
    @brief ALSA-based audio output controller.
	@author Franco Caspe
	@date Documented - 18 Mar 2019.
*/

#include <cstdint>
#include <fstream>
#include <iostream>
#include "output_alsa.hpp"
#include <alsa/asoundlib.h>

using namespace std;
namespace engine
	{
	namespace aout_if
		{
		output_tb::output_tb(uint32_t buffer_sizeX8,bool debug):_debug(debug)
			{
			rate = 48000; /** Sample rate */
			periods = 32; /** Number of periods on which the ring buffer is composed*/
			periodsize = 64; /** Periodsize (bytes) */
			frame_counter = 0;
			}

		output_tb::~output_tb()
			{
			if(_debug)
				{
				cout << "output_tb::~output_tb() INFO: Destructor called." << endl;
				}

			}
	
		bool output_tb::init()
			{
     
			unsigned int exact_rate;	/** Sample rate returned by
											snd_pcm_hw_params_set_rate_near */ 
			//int dir;        /* exact_rate == rate --> dir = 0 */
							  /* exact_rate < rate  --> dir = false */
							  /* exact_rate > rate  --> dir = 1 */

			/* Playback stream */
			snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;

			/* This structure contains information about    */
			/* the hardware and can be used to specify the  */      
			/* configuration to be used for the PCM stream. */ 
			snd_pcm_hw_params_t *hwparams;
  
			/* Name of the PCM device, like plughw:0,0          */
			/* The first number is the number of the soundcard, */
			/* the second number is the number of the device.   */
			char *pcm_name;
	
			/* Init pcm_name. Of course, later you */
			/* will make this configurable ;-)     */
			pcm_name = strdup("plughw:0,0");
	
			/* Allocate the snd_pcm_hw_params_t structure on the stack. */
			snd_pcm_hw_params_alloca(&hwparams);
	
			/* Open PCM. The last parameter of this function is the mode. */
			/* If this is set to 0, the standard mode is used. Possible   */
			/* other values are SND_PCM_NONBLOCK and SND_PCM_ASYNC.       */ 
			/* If SND_PCM_NONBLOCK is used, read / write access to the    */
			/* PCM device will return immediately. If SND_PCM_ASYNC is    */
			/* specified, SIGIO will be emitted whenever a period has     */
			/* been completely processed by the soundcard.                */
			if (snd_pcm_open(&pcm_handle, pcm_name, stream, 0) < 0) {
			  fprintf(stderr, "Error opening PCM device %s\n", pcm_name);
			  return(false);
			}
	
	
			/* Init hwparams with full configuration space */
			if (snd_pcm_hw_params_any(pcm_handle, hwparams) < 0) {
			  fprintf(stderr, "Can not configure this PCM device.\n");
			  return(false);
			}
	
			/* Set access type. This can be either    */
			/* SND_PCM_ACCESS_RW_INTERLEAVED or       */
			/* SND_PCM_ACCESS_RW_NONINTERLEAVED.      */
			/* There are also access types for MMAPed */
			/* access, but this is beyond the scope   */
			/* of this introduction.                  */
			if (snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
			  fprintf(stderr, "Error setting access.\n");
			  return(false);
			}
  
			/* Set sample format */
			if (snd_pcm_hw_params_set_format(pcm_handle, hwparams, SND_PCM_FORMAT_S16_LE) < 0) {
			  fprintf(stderr, "Error setting format.\n");
			  return(false);
			}

			/* Set sample rate. If the exact rate is not supported */
			/* by the hardware, use nearest possible rate.         */ 
			exact_rate = rate;
			if (snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, &exact_rate, 0) < 0) {
			  fprintf(stderr, "Error setting rate.\n");
			  return(false);
			}
			if (rate != exact_rate) {
			  fprintf(stderr, "The rate %d Hz is not supported by your hardware.\n==> Using %d Hz instead.\n", rate, exact_rate);
			}

			/* Set number of channels */
			if (snd_pcm_hw_params_set_channels(pcm_handle, hwparams, 1) < 0) {
			  fprintf(stderr, "Error setting channels.\n");
			  return(false);
			}

			/* Set number of periods. Periods used to be called fragments. */ 
			if (snd_pcm_hw_params_set_periods(pcm_handle, hwparams, periods, 0) < 0) {
			  fprintf(stderr, "Error setting periods.\n");
			  return(false);
			}
	
	
			/* Set buffer size (in frames). The resulting latency is given by */
			/* latency = periodsize * periods / (rate * bytes_per_frame)     */
			if (snd_pcm_hw_params_set_buffer_size(pcm_handle, hwparams, (periodsize * periods)>>1) < 0) {
			  fprintf(stderr, "Error setting buffersize.\n");
			  return(false);
			}
	
     
			/* Apply HW parameter settings to */
			/* PCM device and prepare device  */
			if (snd_pcm_hw_params(pcm_handle, hwparams) < 0) {
			  fprintf(stderr, "Error setting HW params.\n");
			  return(false);
			}
	
			/* Try to alloc the period buffer */
			period_buffer = (int16_t *)malloc(periodsize);
			if(!period_buffer)
				{
				fprintf(stderr, "Error while alloc period buffer.\n");
				return false;
				}
			return true;
			}

		void output_tb::push_sample(synth::sample_t sample)
			{
			//Each frame has 2 bytes lenght.
			const uint32_t buff_frame_len = (periodsize << 1);
			int pcmreturn;
			period_buffer[frame_counter] = (int16_t)sample;
			frame_counter++;
	
			if(frame_counter == buff_frame_len)
				{
				frame_counter = 0;
				if((pcmreturn = snd_pcm_writei(pcm_handle, period_buffer, buff_frame_len)) < 0) 
					{
					snd_pcm_prepare(pcm_handle);
					fprintf(stderr, "<<<<<<<<<<<<<<< Buffer Underrun >>>>>>>>>>>>>>>\n");
					}
				}
		
			}
		} //End namespace audio_output
	} //End namespace engine