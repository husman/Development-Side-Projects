// Author: Haleeq Usman
// Libraries: oRTP, PortAudio
// Standards: TU-T Rec. G.711

#include "globals.h"
#include <portaudio.h>
#include <ortp/ortp.h>
#include <ortp/rtp.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <winsock2.h>

#include <time.h>
#include <sys/time.h>

int runcond=1;

void stophandler(int signum) {
	runcond=0;
}

void mycallback(void *packet) {
	//printf("Packet is no longer needed!\n");
}

// Alaw and Ulaw encoders (retrieved from TU-T Rec. G.711)
/* ................... Begin of alaw_expand() ..................... */
/*
  ==========================================================================

   FUNCTION NAME: alaw_expand

   DESCRIPTION: ALaw decoding rule according ITU-T Rec. G.711.

   PROTOTYPE: void alaw_expand(long lseg, short *logbuf, short *linbuf)

   PARAMETERS:
     lseg:	(In)  number of samples
     logbuf:	(In)  buffer with compressed samples (8 bit right justified,
                      without sign extension)
     linbuf:	(Out) buffer with linear samples (13 bits left justified)

   RETURN VALUE: none.

   HISTORY:
   10.Dec.91	1.0	Separated A-law expansion function

  ============================================================================
 */
void            alaw_expand(long lseg, short *logbuf, short *linbuf)
{
	short           ix, mant, iexp;
	long            n;

	for (n = 0; n < lseg; n++)
	{
		ix = logbuf[n] ^ (0x0055);	/* re-toggle toggled bits */

		ix &= (0x007F);		/* remove sign bit */
		iexp = ix >> 4;		/* extract exponent */
		mant = ix & (0x000F);	/* now get mantissa */
		if (iexp > 0)
			mant = mant + 16;		/* add leading '1', if exponent > 0 */

		mant = (mant << 4) + (0x0008);	/* now mantissa left justified and */
		/* 1/2 quantization step added */
		if (iexp > 1)		/* now left shift according exponent */
			mant = mant << (iexp - 1);

		linbuf[n] = logbuf[n] > 127	/* invert, if negative sample */
				? mant
						: -mant;
	}
}
/* ................... End of alaw_expand() ..................... */

/* ................... Begin of ulaw_expand() ..................... */
/*
  ==========================================================================

   FUNCTION NAME: ulaw_expand

   DESCRIPTION: Mu law decoding rule according ITU-T Rec. G.711.

   PROTOTYPE: void ulaw_expand(long lseg, short *logbuf, short *linbuf)

   PARAMETERS:
     lseg:	(In)  number of samples
     logbuf:	(In)  buffer with compressed samples (8 bit right justified,
                      without sign extension)
     linbuf:	(Out) buffer with linear samples (14 bits left justified)

   RETURN VALUE: none.

   HISTORY:
   10.Dec.91	1.0	Separated mu law expansion function

  ============================================================================
 */

void ulaw_expand(long lseg, short *logbuf, short *linbuf)
{
	long            n;		/* aux.var. */
	short           segment;	/* segment (Table 2/G711, column 1) */
	short           mantissa;	/* low  nibble of log companded sample */
	short           exponent;	/* high nibble of log companded sample */
	short           sign;		/* sign of output sample */
	short           step;

	for (n = 0; n < lseg; n++)
	{
		sign = logbuf[n] < (0x0080) /* sign-bit = 1 for positiv values */
        								? -1 : 1;
		mantissa = ~logbuf[n];	/* 1's complement of input value */
		exponent = (mantissa >> 4) & (0x0007);	/* extract exponent */
		segment = exponent + 1;	/* compute segment number */
		mantissa = mantissa & (0x000F);	/* extract mantissa */

		/* Compute Quantized Sample (14 bit left justified!) */
		step = (4) << segment;	/* position of the LSB */
		/* = 1 quantization step) */
		linbuf[n] = sign *		/* sign */
				(((0x0080) << exponent)	/* '1', preceding the mantissa */
						+ step * mantissa	/* left shift of mantissa */
						+ step / 2		/* 1/2 quantization step */
						- 4 * 33
				);
	}
}
/* ................... End of ulaw_expand() ..................... */


// Converts the 16-bit unsigned paramter into the current cpu's endian
void cpu_uint16(uint16_t *uval, int val_endian)
{
	if(cpu_endian() == val_endian)
		return;

	uint16_t val = *uval;

	val = (val << 8) | ((val >> 8) & 0x00FF);

	*uval = val;
}

// Converts the 32-bit unsigned paramter into the current cpu's endian
void cpu_uint32(uint32_t *uval, int val_endian)
{
	if(cpu_endian() == val_endian)
		return;

	uint32_t val = *uval;

	val = (val << 24)
		  | ((val << 8)  & 0x00FF0000)
          | ((val >> 8)  & 0x0000FF00)
          | ((val >> 24) & 0x000000FF);

	*uval = val;
}

// Reads multiple bytes from the file buffer into the destination
// Returns the number of bytes read.
int read_bytes(uint8_t *dest_data, uint32_t num_bytes, FILE *file) {
	return fread(dest_data, 1, num_bytes, file);
}

// Reads bytes in multiples of 2 bytes from the file buffer into the destination
// Returns the number of bytes read.
int read_bytes_short(short *dest_data, uint32_t num_bytes, FILE *file) {
	return fread(dest_data, 2, num_bytes, file);
}

// Reads a single byte from the file buffer
// Returns the number of bytes read.
int read_uint8(uint8_t *uint8_val, FILE *file) {
	return fread(uint8_val, 1, 1, file);
}

// Reads two bytes from the file buffer
// Returns the number of bytes read.
int read_uint16(uint16_t *uint16_val, FILE *file) {
	return fread(uint16_val, 2, 1, file);
}

// Reads four bytes from the file buffer
// Returns the number of bytes read.
int read_uint32(uint32_t *uint32_val, FILE *file) {
	return fread(uint32_val, 4, 1, file);
}

// Skips num_bytes number of bytes in the file buffer.
// Returns the number of bytes skipped.
int skip_bytes(uint32_t num_bytes, FILE *file)
{
	uint8_t *buf;
	buf = (uint8_t *)malloc(num_bytes);

	if(buf == NULL)
		return -1;

	return fread(buf, 1, num_bytes, file);
}

// Changes the sample rate of the wave.
void changeSampleRate(WaveInfo *wInfo, uint32_t new_rate)
{
	wInfo->dwSamplesPerSec = new_rate;
	wInfo->dwAvgBytesPerSec = wInfo->dwSamplesPerSec*wInfo->wChannels*wInfo->wBitsPerSample/8;
}

// Simple WAVE decoder
int decodeWave(char *filename, WaveInfo *wInfo)
{
	FILE *file;
	uint16_t word;
	uint32_t dword;

	uint32_t bytes_remaining;

	file = fopen(filename, "rb");
	if(file == NULL) {
		printf("File not found or could not be opened!\n");
		return -1;
	}

	while(read_uint32(&dword, file) > 0) {
		cpu_uint32(&dword, BIG_ENDIAN);
		switch(dword) {
		case RIFF:
		case RIFX:
			wInfo->riff_chunk_id = dword;
			if(wInfo->riff_chunk_id == RIFF) {
				wInfo->endian = LITTLE_ENDIAN;
				printf("<riff-ck>:\n");
			} else {
				wInfo->endian = BIG_ENDIAN;
				printf("<rifx-ck>:\n");
			}

			// Process the chunk size
			read_uint32(&dword, file);
			cpu_uint32(&dword, wInfo->endian);
			wInfo->riff_chunk_size = dword;
			printf("chunk size: %08X\n", wInfo->riff_chunk_size);

			// Process the 'RIFF or RIFX' chunk format/type
			// this should be 'WAVE'. If it is not, we error.
			read_uint32(&dword, file);
			cpu_uint32(&dword, BIG_ENDIAN);
			wInfo->riff_format = dword;
			if(wInfo->riff_format == WAVE)
				printf("We have a WAVE file!\n\n");
			else
				printf("We do not have a WAVE file!\n\n");
			break;
		case FMT:
			printf("<fmt-ck>:\n");

			// Process the chunk size
			read_uint32(&dword, file);
			cpu_uint32(&dword, wInfo->endian);
			wInfo->fmt_chunk_size = dword;
			bytes_remaining = wInfo->fmt_chunk_size;
			printf("chunk size: %08X\n", wInfo->fmt_chunk_size);

			// Process audio format
			read_uint16(&word, file);
			cpu_uint16(&word, wInfo->endian);
			wInfo->wFormatTag = word;
			bytes_remaining -= 2;
			printf("Audio format: %u\n", wInfo->wFormatTag);

			// Process number of channels
			read_uint16(&word, file);
			cpu_uint16(&word, wInfo->endian);
			wInfo->wChannels = word;
			bytes_remaining -= 2;
			printf("Channels: %u\n", wInfo->wChannels);

			// Process the sample rate
			read_uint32(&dword, file);
			cpu_uint32(&dword, wInfo->endian);
			wInfo->dwSamplesPerSec = dword;
			bytes_remaining -= 2;
			printf("Sample rate: %u\n", wInfo->dwSamplesPerSec);

			// Process the byte rate
			read_uint32(&dword, file);
			cpu_uint32(&dword, wInfo->endian);
			wInfo->dwAvgBytesPerSec = dword;
			bytes_remaining -= 4;
			printf("Byte rate: %u\n", wInfo->dwAvgBytesPerSec);

			// Process block alignment
			read_uint16(&word, file);
			cpu_uint16(&word, wInfo->endian);
			wInfo->wBlockAlign = word;
			bytes_remaining -= 4;
			printf("Block align: %u\n", wInfo->wBlockAlign);

			// Process bits per sample
			read_uint16(&word, file);
			cpu_uint16(&word, wInfo->endian);
			wInfo->wBitsPerSample = word;
			bytes_remaining -= 2;
			printf("Bits/sample: %u\n", wInfo->wBitsPerSample);

			// At this point, we expect there to be no more data for this
			// chunk if the audio format is 1 (PCM). The RIFF standard
			// requires that no additional fields be added if the audio
			// format is PCM. However, some encoders still add one or
			// two of these fields. Therefore, we skip the remaining
			// bytes of this chunk if our audio format is PCM.
			// Otherwise, we will record the field values.
			if(bytes_remaining == 0)
				continue;
			else if(bytes_remaining > 0 && wInfo->wFormatTag == MY_WAVE_FORMAT_PCM) {
				if(skip_bytes(bytes_remaining, file) == bytes_remaining)
					printf("remaining %u bytes are being skipped for PCM.\n", bytes_remaining);
				else {
					printf("The file terminated prematurely, or could not allocate enough memory.");
					return -1;
				}
			}
			else if(bytes_remaining < 0) // Was the wrong chunk size given to us?
				return -1;
			else {
				// Process extra params
				read_uint16(&word, file);
				cpu_uint16(&word, wInfo->endian);
				wInfo->wExtraParamSize = word;
				bytes_remaining -= 2;
				printf("Extra param size: %04X\n", wInfo->wExtraParamSize);

				// allocate extra params based on extra param size
				wInfo->extraParams = (uint8_t *)malloc(wInfo->wExtraParamSize);

				if(wInfo->extraParams == NULL) {
					printf("Could not allocate enough memory.\n");
					return -1;
				}

				if(read_bytes(wInfo->extraParams, wInfo->wExtraParamSize, file) < wInfo->wExtraParamSize) {
					printf("The file terminated prematurely or is corrupt.\n");
					return -1;
				}
				bytes_remaining -= wInfo->wExtraParamSize;

				// Now we expect bytes remaining to be zero (0). If not, error
				if(bytes_remaining != 0) {
					printf("The file could not be read or is corrupt.\n");
					return -1;
				}
			}
			printf("\n");
			break;
		case DATA:
			printf("<data-ck>:\n");

			// Process the chunk size
			read_uint32(&dword, file);
			cpu_uint32(&dword, wInfo->endian);
			wInfo->data_chunk_size = dword;
			bytes_remaining = wInfo->data_chunk_size;
			printf("chunk size: %u\n", wInfo->data_chunk_size);

			// Process the WAVE data
			// allocate extra params based on extra param size
			wInfo->data = (short *)malloc(sizeof(short)*wInfo->data_chunk_size/2);
			if(fread(wInfo->data, sizeof(short),wInfo->data_chunk_size/2, file) < wInfo->data_chunk_size/2) {
				printf("The file terminated prematurely or is corrupt.\n");
				return -1;
			}
			bytes_remaining -= wInfo->data_chunk_size;

			// Now we expect bytes remaining to be zero (0). If not, error
			if(bytes_remaining != 0) {
				printf("The file could not be read or is corrupt.\n");
				return -1;
			}
			break;
		}
	}
	fclose(file);
	printf("\nDone decoding WAVE file.\n\n");
	return 1;
}

// Some necessary constants/global variables
#define SERVER "67.85.190.87"  //ip address of udp server
#define BUFLEN 512  //Max length of buffer
#define PORT 1234   //The port on which to listen for incoming data

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
	queue *que;
	uint32_t consumed;
	uint32_t size;
	uint32_t sample_rate;
}
paTestData;

static int playLock = 0;
static int start_playback = 0;

typedef struct {
	WaveInfo wInfo;
	queue *que;
	HANDLE *pThreads;
} DData;

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
	short *out = (short*)outputBuffer;
	uint32_t p,i;

	if(data->que->count < data->sample_rate)
		return paContinue;

	(void) timeInfo; /* Prevent unused variable warnings. */
	(void) statusFlags;
	(void) inputBuffer;

	playLock = 1;
	for(p=0; p<data->sample_rate; ++p)
		for(i=0; i<data->num_channels; ++i)
			*out++ = dequeue(data->que);
	//printf("played\n");

	playLock = 0;
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

DWORD WINAPI playThread(void* userData) {
	DData *tdata = (DData *) userData;
	PaStreamParameters outputParameters;
	PaStream *stream;
	PaError err;
	paTestData *data;
	data = (paTestData *)malloc(sizeof(paTestData));

	data->sine = tdata->wInfo.data;
	data->num_channels = tdata->wInfo.wChannels;
	data->que = tdata->que;
	data->consumed = 0;
	data->sample_rate = tdata->wInfo.dwSamplesPerSec;
	data->size = tdata->wInfo.data_chunk_size;

	err = Pa_Initialize();
	if( err != paNoError ) goto error;

	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	if (outputParameters.device == paNoDevice) {
		fprintf(stderr,"Error: No default output device.\n");
		goto error;
	}
	outputParameters.channelCount = tdata->wInfo.wChannels;       /* stereo output */
	outputParameters.sampleFormat = paInt16; /* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(
			&stream,
			NULL, /* no input */
			&outputParameters,
			tdata->wInfo.dwSamplesPerSec,
			tdata->wInfo.dwSamplesPerSec,
			paClipOff,      /* we won't output out of range samples so don't bother clipping them */
			patestCallback,
			data );
	if( err != paNoError ) goto error;

	sprintf( data->message, "No Message" );
	err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
	if( err != paNoError ) goto error;

	err = Pa_StartStream( stream );
	if( err != paNoError ) goto error;
	start_playback = 1;

	printf("Playing Song...\n");
	WaitForSingleObject(tdata->pThreads[0], INFINITE);

	start_playback = 0;
	err = Pa_StopStream( stream );
	if( err != paNoError ) goto error;

	err = Pa_CloseStream( stream );
	if( err != paNoError ) goto error;

	Pa_Terminate();

	return err;
	error:
	Pa_Terminate();
	fprintf( stderr, "An error occured while using the portaudio stream\n" );
	fprintf( stderr, "Error number: %d\n", err );
	fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
	return err;
}

int main(int argc, char *argv[])
{
	if(argc < 4) {
		printf("Usage: ms_test file.wav remote_ip remote_port\n");
		return 0;
	}
	WaveInfo wInfo;
	if(decodeWave(argv[1], &wInfo) == -1) {
		printf("Failed to decode input file.\n");
		return 0;
	}

	int nbBytes;
	/*Holds the state of the encoder*/
	void *state;
	/*Holds bits so they can be read and written to by the Speex routines*/
	SpeexBits bits;
	int tmp;
	size_t frame_size;

	/*Initialization of the structure that holds the bits*/
	speex_bits_init(&bits);

	/*Create a new encoder state in narrowband mode*/
	state = speex_encoder_init(&speex_wb_mode);

	speex_encoder_ctl(state,SPEEX_GET_FRAME_SIZE,&frame_size);

	printf("frame size = %d\n",frame_size);

	/*Set the quality to 8 (15 kbps)*/
	tmp=8;
	speex_encoder_ctl(state, SPEEX_SET_QUALITY, &tmp);

	// oRTP Client
	RtpSession *session;
	char *ssrc;
	uint32_t user_ts=0;
	int jitter=20;

	ortp_init();
	ortp_scheduler_init();
	ortp_set_log_level_mask(ORTP_DEBUG|ORTP_MESSAGE|ORTP_WARNING|ORTP_ERROR);
	session=rtp_session_new(RTP_SESSION_SENDONLY);

	rtp_session_set_scheduling_mode(session,1);
	rtp_session_set_blocking_mode(session,0);
	rtp_session_set_connected_mode(session,TRUE);
	rtp_session_set_remote_addr(session,argv[2],atoi(argv[3]));
	rtp_session_set_payload_type(session,0);
	rtp_session_set_recv_buf_size(session,8000);

	ssrc=getenv("SSRC");
	if (ssrc!=NULL) {
	printf("using SSRC=%i.\n",atoi(ssrc));
	rtp_session_set_ssrc(session,atoi(ssrc));
	}

	signal(SIGINT,stophandler);
	//start communication
	while(runcond)
	{
		printf("Starting Music...\n");
		uint32_t size = 0;
		char out[320];
		uint32_t max_chunks = wInfo.data_chunk_size/wInfo.dwAvgBytesPerSec;
		uint32_t chunks = 0;
		short *data_ptr = wInfo.data;
		int p = 0;
		while(size<wInfo.data_chunk_size && chunks < max_chunks && runcond) {

			/*Flush all the bits in the struct so we can encode a new frame*/
			speex_bits_reset(&bits);

			/*Encode the frame*/
			speex_encode_int(state, data_ptr, &bits);
			data_ptr += frame_size;

			/*Copy the bits to an array of char that can be written*/
			nbBytes = speex_bits_write(&bits, out, frame_size);

			//send the message
			mblk_t *packet;
			packet = rtp_session_create_packet_with_data(session,out,nbBytes,NULL);
			rtp_session_sendm_with_ts(session,packet,user_ts);
			user_ts+=20;

			/*this will simulate a burst of late packets */
			/*if (jitter && (user_ts%(8000)==0)) {
				ortp_message("Simulating late packets now (%i milliseconds)",jitter);
				Sleep(jitter);
			}*/

			Sleep(20);

			//printf("sent a packet %d\n", p++);
			size += frame_size;
			if(size % wInfo.dwSamplesPerSec == 0) {
				++chunks;
				printf("max_chunks = %u, chunk = %u\n", max_chunks, chunks);
			}
		}
		printf("Ending Music...\n");
	}

	rtp_session_destroy(session);
	ortp_exit();
	ortp_global_stats_display();

	printf("end\n");

	return 0;
}
