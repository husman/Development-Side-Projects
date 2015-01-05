 /*
  The oRTP library is an RTP (Realtime Transport Protocol - rfc3550) stack..
  Copyright (C) 2001  Simon MORLAT simon.morlat@linphone.org

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include <ortp/ortp.h>
#include <ortp/rtp.h>
#include <signal.h>
#include <stdlib.h>

#include <portaudio.h>

#ifndef _WIN32
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#endif

#include "globals.h"

#define NUM_SECONDS   (253)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (1)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (200)
typedef struct
{
    short *sine;
    int num_channels;
    char message[20];
}
paTestData;

int cond=1;

void stop_handler(int signum)
{
	cond=0;
}

void ssrc_cb(RtpSession *session)
{
	printf("hey, the ssrc has changed !\n");
}

static char *help="usage: rtprecv loc_port\n";

#if defined(__hpux) && HAVE_SYS_AUDIO_H
#include <sys/audio.h>

int sound_init(int format)
{
	int fd;
	fd=open("/dev/audio",O_WRONLY);
	if (fd<0){
		perror("Can't open /dev/audio");
		return -1;
	}
	ioctl(fd,AUDIO_RESET,0);
	ioctl(fd,AUDIO_SET_SAMPLE_RATE,8000);
	ioctl(fd,AUDIO_SET_CHANNELS,1);
	if (format==MULAW)
		ioctl(fd,AUDIO_SET_DATA_FORMAT,AUDIO_FORMAT_ULAW);
	else ioctl(fd,AUDIO_SET_DATA_FORMAT,AUDIO_FORMAT_ALAW);
	return fd;
}
#else
int sound_init(int format)
{
	return -1;
}
#endif

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    paTestData *data = (paTestData*)userData;
    short *out = (short *)outputBuffer;
    int i;

    //printf("patestCallback [START]\n");
    if(data == NULL || data->sine == NULL)
    	return paContinue;

    //printf("not null --> %d\n",z++);

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;

    for(i=0; i<data->num_channels; ++i) {
    	*out++ = *data->sine++;
    }

    //printf("patestCallback [END]");

    return paContinue;
}

/*
 * This routine is called by portaudio when playback is done.
 */
static void StreamFinished( void* userData )
{
   paTestData *data = (paTestData *) userData;
   printf( "Stream Completed: %s\n", data->message );
}

int main(int argc, char*argv[])
{
	RtpSession *session;
	uint32_t ts=0;
	int local_port;
	int jittcomp=40;
	bool_t adapt=TRUE;

	/* init the lib */
	if (argc<2){
		printf("%s",help);
		return -1;
	}
	local_port=atoi(argv[1]);
	if (local_port<=0) {
		printf("%s",help);
		return -1;
	}

	ortp_init();
	ortp_scheduler_init();
	ortp_set_log_level_mask(ORTP_DEBUG|ORTP_MESSAGE|ORTP_WARNING|ORTP_ERROR);
	signal(SIGINT,stop_handler);
	session=rtp_session_new(RTP_SESSION_RECVONLY);
	rtp_session_set_scheduling_mode(session,1);
	rtp_session_set_blocking_mode(session,1);
	rtp_session_set_local_addr(session,"0.0.0.0",atoi(argv[1]),-1);
	rtp_session_set_connected_mode(session,TRUE);
	rtp_session_set_symmetric_rtp(session,TRUE);
	rtp_session_enable_adaptive_jitter_compensation(session,adapt);
	rtp_session_set_jitter_compensation(session,jittcomp);
	rtp_session_set_payload_type(session,0);
	rtp_session_signal_connect(session,"ssrc_changed",(RtpCallback)ssrc_cb,0);
	rtp_session_signal_connect(session,"ssrc_changed",(RtpCallback)rtp_session_reset,0);

	PaStreamParameters outputParameters;
	PaStream *stream;
	PaError err;
	paTestData data;
	char *payload;
	short mdata[160];


	printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);

	data.num_channels = 2;

	err = Pa_Initialize();
	if( err != paNoError ) goto error;

	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	if (outputParameters.device == paNoDevice) {
	  fprintf(stderr,"Error: No default output device.\n");
	  goto error;
	}
	outputParameters.channelCount = 2;       /* stereo output */
	outputParameters.sampleFormat = paInt16; /* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(
			  &stream,
			  NULL, /* no input */
			  &outputParameters,
			  44100,
			  FRAMES_PER_BUFFER,
			  paClipOff,      /* we won't output out of range samples so don't bother clipping them */
			  patestCallback,
			  &data );
	if( err != paNoError ) goto error;

	sprintf( data.message, "No Message" );
	err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
	if( err != paNoError ) goto error;

	//err = Pa_StartStream( stream );
	//if( err != paNoError ) goto error;

	/*Holds the audio that will be written to file (16 bits per sample)*/
   /*Speex handle samples as float, so we need an array of floats*/
   /*Holds the state of the decoder*/
   void *state;
   /*Holds bits so they can be read and written to by the Speex routines*/
   SpeexBits bits;
   int tmp;

   /*Create a new decoder state in narrowband mode*/
  state = speex_decoder_init(&speex_nb_mode);

  /*Set the perceptual enhancement on*/
  tmp=1;
  speex_decoder_ctl(state, SPEEX_SET_ENH, &tmp);

  /*Initialization of the structure that holds the bits*/
  speex_bits_init(&bits);

  FILE *fout, *fout2;

  	data.sine = NULL;

  	int p=0;
	printf("Listening on port: %d\n", rtp_session_get_local_port(session));
	while(cond)
	{
		mblk_t *packet;
		int payload_size;

		packet=rtp_session_recvm_with_ts(session,ts);
		ts+=20;
		if(packet == NULL) {
			/*if(playingAudio) {
				err = Pa_StopStream( stream );
				if( err != paNoError ) goto error;
			}
			playingAudio=0;*/
			continue;
		}

		/*if(!playingAudio) {
			err = Pa_StartStream( stream );
			if( err != paNoError ) goto error;
			playingAudio=1;
		}*/
		payload_size=rtp_get_payload(packet,&payload);
		printf("received packet with payload size=%d\n",payload_size);

		fout2=fopen("logger2.txt","w");
		fwrite(payload,1,payload_size,fout2);
		fclose(fout2);

		speex_bits_read_from(&bits, payload, payload_size);

		/*Decode the data*/
		tmp=speex_decode_int(state, &bits, mdata);

		fout=fopen("logger.txt","w");
		fwrite(mdata,1,160,fout);
		fclose(fout);

		data.sine = mdata;

		//printf("Play for %d seconds.\n", NUM_SECONDS );
		Pa_Sleep(20);

	}

	err = Pa_CloseStream( stream );
	if( err != paNoError ) goto error;

	Pa_Terminate();
	printf("Test finished.\n");

	error:
	    Pa_Terminate();
	    fprintf( stderr, "An error occured while using the portaudio stream\n" );
	    fprintf( stderr, "Error number: %d\n", err );
	    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );

	rtp_session_destroy(session);
	ortp_exit();

	ortp_global_stats_display();

	return 0;
}
