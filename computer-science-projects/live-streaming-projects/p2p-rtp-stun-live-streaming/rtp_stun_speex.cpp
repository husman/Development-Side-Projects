// Author: Haleeq Usman

// This is an experimental project that peaked my interest when Adobe introduced
// P2P support for the Flash Media Server. I combined my previous video/audio/networking
// experiments into this to formulate an experimental program utilzing the STUN concept
// to achieve P2P connection and stream a wave audio file. Multiple libraries were utilized
// in this experimental project: PortAudio, oRTP, Simple STUN implementation from the internet,
// and GTK+ for the GUI.

#include <math.h>

// Speex is written in C and the library is decorated as C functions
extern "C" {
#include "globals.h"
}

// Catch interrupts
#include <signal.h>

// Third party STUN implementation
#include "StunClientHelper.h"

using namespace std;

AppInfo appinfo;

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

void cpu_uint16(uint16_t *uval, int val_endian)
{
	if(cpu_endian() == val_endian)
		return;

	uint16_t val = *uval;

	val = (val << 8) | ((val >> 8) & 0x00FF);

	*uval = val;
}

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

int read_bytes(uint8_t *dest_data, uint32_t num_bytes, FILE *file) {
	return fread(dest_data, 1, num_bytes, file);;
}

int read_bytes_short(short *dest_data, uint32_t num_bytes, FILE *file) {
	return fread(dest_data, 2, num_bytes, file);;
}

int read_uint8(uint8_t *uint8_val, FILE *file) {
	return fread(uint8_val, 1, 1, file);;
}

int read_uint16(uint16_t *uint16_val, FILE *file) {
	return fread(uint16_val, 2, 1, file);
}

int read_uint32(uint32_t *uint32_val, FILE *file) {
	return fread(uint32_val, 4, 1, file);
}

int skip_bytes(uint32_t num_bytes, FILE *file)
{
	uint8_t *buf;
	buf = (uint8_t *)malloc(num_bytes);

	if(buf == NULL)
		return -1;

	return fread(buf, 1, num_bytes, file);
}

void changeSampleRate(WaveInfo *wInfo, uint32_t new_rate)
{
	wInfo->dwSamplesPerSec = new_rate;
	wInfo->dwAvgBytesPerSec = wInfo->dwSamplesPerSec*wInfo->wChannels*wInfo->wBitsPerSample/8;
}

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

#define NUM_SECONDS   (250)
#define SAMPLE_RATE   (320)
#define FRAMES_PER_BUFFER  (44100)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

static bool changeDeviceRequested = FALSE;
static bool listenerThreadRunning = FALSE;

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
static int playing = 0;
static int start_playback = 0;

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
			if(data->que->count > 0) {
				*out++ = dequeue(data->que);
			} else {
				*out++ = 0;
			}
	//printf("played\n");

	playLock = 0;
	return paContinue;
}

int runcond=1;

void stophandler(int signum)
{
	runcond=0;
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
	changeDeviceRequested = FALSE;
	listenerThreadRunning = TRUE;
	DData *tdata = (DData *) userData;
	PaStreamParameters outputParameters;
	PaStream *stream;
	PaError err;
	paTestData *data;
	data = (paTestData *)malloc(sizeof(paTestData));
	int index;

	data->sine = tdata->wInfo.data;
	data->num_channels = tdata->wInfo.wChannels;
	data->que = tdata->que;
	data->consumed = 0;
	data->sample_rate = tdata->wInfo.dwSamplesPerSec;
	data->size = tdata->wInfo.data_chunk_size;

	printf("sr = %d chan = %d\n", data->sample_rate, data->num_channels);

	err = Pa_Initialize();
	if( err != paNoError ) goto error;

	char tmp_string[50];
	index = gtk_combo_box_get_active(GTK_COMBO_BOX(appinfo.output_device_list));
	index = appinfo.outputDevices[index].device_pa_index;
	append_textview(appinfo.textview, return_string(sprintf(tmp_string,"starting audio device with index = %d\n",index),tmp_string));
	outputParameters.device =  index; //Pa_GetDefaultOutputDevice(); /* default output device */
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

	g_print("Playing Song...\n");
	while(!changeDeviceRequested);
		//WaitForSingleObject(tdata->pThreads[0], INFINITE);

	start_playback = 0;
	err = Pa_StopStream( stream );
	if( err != paNoError ) goto error;

	err = Pa_CloseStream( stream );
	if( err != paNoError ) goto error;

	Pa_Terminate();
	listenerThreadRunning = FALSE;

	append_textview(appinfo.textview, "reloading audio device...\n");

	return err;
	error:
	Pa_Terminate();
	fprintf( stderr, "An error occured while using the portaudio stream\n" );
	fprintf( stderr, "Error number: %d\n", err );
	fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
	listenerThreadRunning = FALSE;
	append_textview(appinfo.textview, "reloading audio device...\n");
	return err;
}

DWORD WINAPI playThread2(void* userData) {
	while(!start_playback);
	DData *tdata = (DData *) userData;
	static uint32_t size = 0;
	static uint32_t chunks = 0;
	uint32_t max_chunks = tdata->wInfo.data_chunk_size/tdata->wInfo.dwAvgBytesPerSec;
	char tmp_string[500];

	// SPEEX
	/*Holds the state of the encoder*/
	void *enc_state, *dec_state;
	/*Holds bits so they can be read and written to by the Speex routines*/
	SpeexBits enc_bits, dec_bits;
	int tmp,i;
	size_t frame_size;

	/*Initialization of the structure that holds the enc/dec_bits*/
	speex_bits_init(&enc_bits);
	speex_bits_init(&dec_bits);

	/*Create a new encoder/decoder state in narrowband mode*/
	enc_state = speex_encoder_init(&speex_wb_mode);
	dec_state = speex_decoder_init(&speex_wb_mode);

	/* Get frame size for encoding/decoding mode */
	speex_encoder_ctl(enc_state,SPEEX_GET_FRAME_SIZE,&frame_size);

	printf("frame size = %d\n",frame_size);

	/*Set the encoding quality to 8 (15 kbps)*/
	tmp=8;
	speex_encoder_ctl(enc_state, SPEEX_SET_QUALITY, &tmp);

	/*Set the decoder perceptual enhancement on*/
	tmp=1;
	speex_decoder_ctl(dec_state, SPEEX_SET_ENH, &tmp);

	//Initialise ortp

	RtpSession *session;
	int jittcomp=20;
	uint32_t ts=0,user_ts=0;

	ortp_init();
	ortp_scheduler_init();
	ortp_set_log_level_mask(ORTP_DEBUG|ORTP_WARNING|ORTP_ERROR);
	signal(SIGINT,stophandler);
	session=rtp_session_new(RTP_SESSION_RECVONLY);
	rtp_session_enable_rtcp(session, FALSE);
	rtp_session_set_scheduling_mode(session,1);
	rtp_session_set_blocking_mode(session,0);
	rtp_session_set_local_addr(session,tdata->local_ip,tdata->port, -1);
	rtp_session_set_connected_mode(session,FALSE);
	rtp_session_set_symmetric_rtp(session,TRUE);
	rtp_session_enable_adaptive_jitter_compensation(session,TRUE);
	rtp_session_set_jitter_compensation(session,jittcomp);
	rtp_session_set_payload_type(session,0);
	//rtp_session_signal_connect(session,"ssrc_changed",(RtpCallback)ssrc_cb,0);
	//rtp_session_signal_connect(session,"ssrc_changed",(RtpCallback)rtp_session_reset,0);
	//rtp_session_signal_connect(session,"timestamp_jump",(RtpCallback)tsjump_cb,0);
	//rtp_session_signal_connect(session,"network_error",(RtpCallback)neterror_cb,0);
	rtp_session_set_recv_buf_size(session,320);

	printf("Going to Listen on ip = %s, port = %d\n", tdata->local_ip, tdata->port);
	printf("Listening on ip = %s, port = %d\n", tdata->local_ip, rtp_session_get_local_port(session));
	append_textview(appinfo.textview, return_string(
			sprintf(tmp_string,"Listening on ip = %s, port = %d\n", tdata->local_ip, rtp_session_get_local_port(session))
			,tmp_string));

	playing = 1;
	//keep listening for data
	g_print("Waiting for packets...\n");
	while(runcond)
	{
		mblk_t *packet;
		unsigned char *payload;
		int payload_size;

		short dbits[320];

		packet=rtp_session_recvm_with_ts(session,ts);
		ts+=20;
		if(packet == NULL)
			continue;
		payload_size=rtp_get_payload(packet,&payload);

		/*Flush all the bits in the struct so we can encode/decode a new frame*/
		speex_bits_reset(&dec_bits);

		/*Put encoded bits into decoder*/
		speex_bits_read_from(&dec_bits, (char *)payload, frame_size);

		/*Decode the data*/
		tmp=speex_decode_int(dec_state, &dec_bits, dbits);

		/*Put the frame into the queue*/
		for(i=0; i<frame_size; ++i) {
			//while(tdata->que->count >= QUEUESIZE);
			enqueue(tdata->que, dbits[i]);
			++size;
			if(size % (tdata->wInfo.dwSamplesPerSec) == 0) {
				++chunks;
				ortp_global_stats_display();
				printf("[RECEIVED] max_chunks = %u, chunk = %u\n", max_chunks, chunks);
			}

		}
		rtp_session_resync(session);

		/*if(chunks % 30) {
			mblk_t *packet;
			packet = rtp_session_create_packet_with_data(session,payload,frame_size,NULL);
			rtp_session_sendm_with_ts(session,packet,user_ts);
			user_ts+=20;
		}*/
	}
	rtp_session_destroy(session);
	ortp_exit();

	ortp_global_stats_display();
	playing = 0;

	return 0;
}

DWORD WINAPI playThread3(void* userData) {
	WaveInfo wInfo;
	if(decodeWave("test.wav", &wInfo) == -1) {
		append_textview(appinfo.textview, "Failed to decode input file.\n");
		g_print("Failed to decode input file.\n");
	}

	queue q;
	init_queue(&q);
	DData dd;
	dd.wInfo = wInfo;
	dd.que = &q;
	dd.ip = appinfo.remoteinfo->ip;
	dd.local_ip = appinfo.remoteinfo->local_ip;
	dd.port = appinfo.remoteinfo->port;

	printf("CHANNELS = %d\n",dd.wInfo.wChannels);
	HANDLE pThreads[2];
	dd.pThreads = pThreads;

	appinfo.ddata = &dd;
	pThreads[0] = CreateThread(NULL, 0, playThread2, (LPVOID *)&dd, 0, NULL);
	pThreads[1] = CreateThread(NULL, 0, playThread, (LPVOID *)&dd, 0, NULL);
	WaitForMultipleObjects(2, pThreads, TRUE, INFINITE);
}

void ssrc_cb(RtpSession *session)
{
	printf("hey, the ssrc has changed !\n");
}

void tsjump_cb(RtpSession *session)
{
	printf("The stream went out of sync. Resyncing...\n");
	rtp_session_resync(session);
}

void neterror_cb(RtpSession *session)
{
	printf("A network error has occured.\n");
}

void print_ip(uint32_t ip)
{
	unsigned char bytes[4];
	bytes[0] = ip & 0xFF;
	bytes[1] = (ip >> 8) & 0xFF;
	bytes[2] = (ip >> 16) & 0xFF;
	bytes[3] = (ip >> 24) & 0xFF;
	printf("%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);
}

typedef struct WidgetData {
	GtkWidget *window;
	GtkWidget *textview;
	GtkWidget *textview_scroll;
	GtkWidget *statusbar;
	RtpSession *rtpsession;
};


/* Draws a border around a widget using cairo */
void stream_wave_file_cb(GtkWidget *btn, gpointer userData)
{
	WidgetData *wd = (WidgetData *)userData;
	GtkWidget *file_dialog;
	char *filename;

	char tmp_string[500];

	file_dialog = gtk_file_chooser_dialog_new ("Open File",
			GTK_WINDOW(wd->window),
			GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
			NULL);

	gtk_label_set_label(GTK_LABEL(wd->statusbar), "Please select a wav file.");
	if (gtk_dialog_run (GTK_DIALOG (file_dialog)) == GTK_RESPONSE_ACCEPT)
	{
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (file_dialog));
		append_textview(wd->textview, return_string(sprintf(tmp_string, "selected file: %s\n", filename), tmp_string));
	}
	gtk_widget_destroy (file_dialog);

	WaveInfo wInfo;
	if(decodeWave(filename, &wInfo) == -1) {
		append_textview(wd->textview, "Failed to decode input file.\n");
		g_print("Failed to decode input file.\n");
	}
	g_free (filename);
	gtk_label_set_label(GTK_LABEL(wd->statusbar), "Starting stream...");

	/*CStunClientHelper clientHelper ("stunserver.org");
	NAT_TYPE Nat = clientHelper.GetNatType (textview);

			cout << endl;
			switch (Nat)
			{
			case ERROR_DETECTING_NAT:
				cout << "There was an error detecting NAT." << endl;
				break;

			case FIREWALL_BLOCKS_UDP:
				cout << "There is a firewall that blocks UDP." << endl;
				break;

			case FULL_CONE_NAT:
				cout << "The NAT type is Full Cone NAT." << endl;
				break;

			case OPEN_INTERNET:
				cout << "There is no NAT and directly on Open Internet." << endl;
				break;

			case RESTRICTED_CONE_NAT:
				cout << "The NAT type is Restricted Cone NAT." << endl;
				break;

			case RESTRICTED_PORT_CONE_NAT:
				cout << "The NAT type is Restricted Port Cone NAT." << endl;
				break;

			case SYMMETRIC_NAT:
				cout << "The NAT type is Symmetric NAT." << endl;
				break;

			case SYMMETRIC_UDP_FIREWALL:
				cout << "There is a symmetric UDP firewall." << endl;
				break;
			}

	struct in_addr local_addr[10];
	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		cerr << "Error " << WSAGetLastError() <<
				" when getting local host name." << endl;
		return;
	}
	append_textview(wd->textview, return_string(sprintf(tmp_string, "Your Host name is: %s.\n",ac), tmp_string));
	cout << "Host name is " << ac << "." << endl;

	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) {
		append_textview(wd->textview, "Something went wrong while trying to look up your host name. ");
		cerr << "Yow! Bad host lookup." << endl;
		return;
	}

	append_textview(wd->textview, "Detecting your network card[s] and their location...\n");
	for (int i = 0; phe->h_addr_list[i] != 0 && i < 10; ++i) {
		memcpy(&local_addr[i], phe->h_addr_list[i], sizeof(struct in_addr));
		append_textview(wd->textview, return_string(sprintf(tmp_string, "Network Card #%d, Address: %s\n", i+1, inet_ntoa(local_addr[i])), tmp_string));
		cout << "Network Card #" << (i+1) << ", Address: " << inet_ntoa(local_addr[i]) << endl;
	}

	append_textview(wd->textview, "Attemping to transverse through your network (NAT Detection)...\n");

	SOCKADDR_IN addr;
	clientHelper.GetStunMappedAddress (&addr);
	char *ip = inet_ntoa(addr.sin_addr);
	unsigned short mapped_port = htons(addr.sin_port);

	append_textview(wd->textview, return_string(sprintf (tmp_string, "Mapped Address: %s:%d\n", ip, mapped_port), tmp_string));
	//cout << "\n\nMapped Address: " << ip << ":" << mapped_port <<"\n\n";

	GtkAdjustment *textview_vscroll = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(wd->textview_scroll));
	gtk_adjustment_set_value(textview_vscroll, gtk_adjustment_get_upper(textview_vscroll));

	/*queue q;
	init_queue(&q);
	DData dd;
	dd.wInfo = wInfo;
	dd.que = &q;
	dd.local_ip = inet_ntoa(local_addr[0]);
	dd.port = mapped_port;*/

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

	const char *remote_ip = appinfo.selected_user->ip;
	unsigned short remote_port = atoi(appinfo.selected_user->port);

	printf("Streaming to %s:%d\n", remote_ip, remote_port);


	ortp_init();
	ortp_scheduler_init();
	ortp_set_log_level_mask(ORTP_DEBUG|ORTP_WARNING|ORTP_ERROR);
	session=rtp_session_new(RTP_SESSION_SENDONLY);
	rtp_session_enable_rtcp(session, FALSE);

	rtp_session_set_scheduling_mode(session,1);
	rtp_session_set_blocking_mode(session,0);
	rtp_session_set_connected_mode(session,FALSE);
	rtp_session_set_remote_addr(session,remote_ip,remote_port);
	rtp_session_set_payload_type(session,0);
	rtp_session_set_recv_buf_size(session,8000);

	ssrc=getenv("SSRC");
	if (ssrc!=NULL) {
		printf("using SSRC=%i.\n",atoi(ssrc));
		rtp_session_set_ssrc(session,atoi(ssrc));
	}

	//start communication
	append_textview(wd->textview, "Starting Music...\n");
	uint32_t size = 0;
	char out[320];
	uint32_t max_chunks = wInfo.data_chunk_size/wInfo.dwAvgBytesPerSec;
	uint32_t chunks = 0;
	short *data_ptr = wInfo.data;
	int p = 0;
	while(size<wInfo.data_chunk_size && chunks < max_chunks) {

		/*Flush all the bits in the struct so we can encode a new frame*/
		speex_bits_reset(&bits);

		/*Encode the frame*/
		speex_encode_int(state, data_ptr, &bits);
		data_ptr += frame_size;

		/*Copy the bits to an array of char that can be written*/
		nbBytes = speex_bits_write(&bits, out, frame_size);

		//send the message
		mblk_t *packet;
		packet = rtp_session_create_packet_with_data(session,(unsigned char*)out,nbBytes,NULL);
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
			//append_textview(wd->textview, return_string(sprintf(tmp_string, "max_chunks = %u, chunk = %u\n", max_chunks, chunks), tmp_string));
			printf("[SENT]     max_chunks = %u, chunk = %u\n", max_chunks, chunks);
			//gtk_label_set_label(GTK_LABEL(wd->statusbar), return_string(sprintf(tmp_string, "max_chunks = %u, chunk = %u", max_chunks, chunks), tmp_string));
			while(gtk_events_pending()) gtk_main_iteration();
		}
	}
	append_textview(wd->textview, "Ending Music...\n");
	rtp_session_destroy(session);
	ortp_exit();
	ortp_global_stats_display();

	//WaitForMultipleObjects(2, pThreads, TRUE, INFINITE);

}

gboolean cb_delete_event(GtkWidget *login_box, gpointer user_data)
{
	printf("delete event called\n");
	gtk_widget_hide(login_box);
	return FALSE;
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
	appinfo.user_id = (char *)malloc(sizeof(char)*nmemb);
	memcpy(appinfo.user_id, (char *)buffer, nmemb);
	//appinfo.user_id = (char *)buffer;
	printf("got user id = %s\n", appinfo.user_id);
	return nmemb;
}
size_t registered_user_id(void *buffer, size_t size, size_t nmemb, void *userp) {
	const char *data = (const char *)buffer;
	printf("got json data = %s\n", data);

	appinfo.users_list_size = 0;
	json_t *root;
	json_error_t error;

	root = json_loads(data, 0, &error);

	if(!json_is_array(root))  {
		printf("error: root is not an array\n");
		json_decref(root);
		return 0;
	}

	const size_t json_data_len = json_array_size(root);
	size_t i;
	appinfo.user_data_list = (UserListData *)realloc(appinfo.user_data_list, sizeof(UserListData)*json_data_len);
	for(i=0; i<json_data_len; ++i)
	{
		json_t *data, *tmp;

		data = json_array_get(root, i);
		if(!json_is_object(data))
		{
			printf("error: user data %d is not an object\n", i + 1);
			json_decref(root);
			return 0;
		}

		tmp = json_object_get(data, "id");
		if(!json_is_string(tmp))
		{
			printf("error: user %d: id is not a string\n", i + 1);
			return 0;
		}
		appinfo.user_data_list[i].id = strdup(json_string_value(tmp));

		tmp = json_object_get(data, "firstname");
		if(!json_is_string(tmp))
		{
			fprintf(stderr, "error: user %d: firstname is not a string\n", i + 1);
			json_decref(root);
			return 0;
		}
		appinfo.user_data_list[i].firstname = strdup(json_string_value(tmp));

		tmp = json_object_get(data, "lastname");
		if(!json_is_string(tmp))
		{
			fprintf(stderr, "error: user %d: lastname is not a string\n", i + 1);
			json_decref(root);
			return 0;
		}
		appinfo.user_data_list[i].lastname = strdup(json_string_value(tmp));

		tmp = json_object_get(data, "fullname");
		if(!json_is_string(tmp))
		{
			fprintf(stderr, "error: user %d: fullname is not a string\n", i + 1);
			json_decref(root);
			return 0;
		}
		appinfo.user_data_list[i].fullname = strdup(json_string_value(tmp));

		tmp = json_object_get(data, "mapped_ip");
		if(!json_is_string(tmp))
		{
			fprintf(stderr, "error: user %d: ip is not a string\n", i + 1);
			json_decref(root);
			return 0;
		}
		appinfo.user_data_list[i].ip = strdup(json_string_value(tmp));

		tmp = json_object_get(data, "mapped_port");
		if(!json_is_string(tmp))
		{
			fprintf(stderr, "error: user %d: port is not a string\n", i + 1);
			return 0;
		}
		appinfo.user_data_list[i].port = strdup(json_string_value(tmp));
		gtk_combo_box_append_text(GTK_COMBO_BOX(appinfo.user_list), appinfo.user_data_list[i].fullname);

		if(appinfo.selected_user != NULL && appinfo.selected_user->id == appinfo.user_data_list[i].id)
			gtk_combo_box_set_active(GTK_COMBO_BOX(appinfo.user_list), i);

		++appinfo.users_list_size;
	}

	json_decref(root);

	char tmp_string[500];
	for(i=0; i<json_data_len; ++i) {
		append_textview(appinfo.textview, return_string(sprintf(tmp_string,"user[%u] = %s, %s, %s, %s, %s, %s\n",
				i,
				appinfo.user_data_list[i].id, appinfo.user_data_list[i].firstname,
				appinfo.user_data_list[i].lastname, appinfo.user_data_list[i].fullname,
				appinfo.user_data_list[i].ip, appinfo.user_data_list[i].port),tmp_string));
		printf("user[%u] = %s, %s, %s, %s, %s, %s\n",
						i,
						appinfo.user_data_list[i].id, appinfo.user_data_list[i].firstname,
						appinfo.user_data_list[i].lastname, appinfo.user_data_list[i].fullname,
						appinfo.user_data_list[i].ip, appinfo.user_data_list[i].port);
	}

	if(appinfo.selected_user == NULL) {
		appinfo.selected_user = appinfo.user_data_list;
		gtk_combo_box_set_active(GTK_COMBO_BOX(appinfo.user_list), 0);
	}

	CreateThread(NULL, 0, playThread3, (LPVOID *)NULL, 0, NULL);

	return nmemb;
}

size_t refresh_user_id(void *buffer, size_t size, size_t nmemb, void *userp) {
	const char *data = (const char *)buffer;
	printf("got json data = %s\n", data);
	;
	json_t *root;
	json_error_t error;

	root = json_loads(data, 0, &error);

	if(!json_is_array(root))  {
		printf("error: root is not an array\n");
		json_decref(root);
		return 0;
	}

	const size_t json_data_len = json_array_size(root);
	size_t i;
	appinfo.user_data_list = (UserListData *)realloc(appinfo.user_data_list, sizeof(UserListData)*json_data_len);
	for(i=0; i<json_data_len; ++i)
	{
		json_t *data, *tmp;

		data = json_array_get(root, i);
		if(!json_is_object(data))
		{
			printf("error: user data %d is not an object\n", i + 1);
			json_decref(root);
			return 0;
		}

		tmp = json_object_get(data, "id");
		if(!json_is_string(tmp))
		{
			printf("error: user %d: id is not a string\n", i + 1);
			return 0;
		}
		appinfo.user_data_list[i].id = strdup(json_string_value(tmp));

		tmp = json_object_get(data, "firstname");
		if(!json_is_string(tmp))
		{
			fprintf(stderr, "error: user %d: firstname is not a string\n", i + 1);
			json_decref(root);
			return 0;
		}
		appinfo.user_data_list[i].firstname = strdup(json_string_value(tmp));

		tmp = json_object_get(data, "lastname");
		if(!json_is_string(tmp))
		{
			fprintf(stderr, "error: user %d: lastname is not a string\n", i + 1);
			json_decref(root);
			return 0;
		}
		appinfo.user_data_list[i].lastname = strdup(json_string_value(tmp));

		tmp = json_object_get(data, "fullname");
		if(!json_is_string(tmp))
		{
			fprintf(stderr, "error: user %d: fullname is not a string\n", i + 1);
			json_decref(root);
			return 0;
		}
		appinfo.user_data_list[i].fullname = strdup(json_string_value(tmp));

		tmp = json_object_get(data, "mapped_ip");
		if(!json_is_string(tmp))
		{
			fprintf(stderr, "error: user %d: ip is not a string\n", i + 1);
			json_decref(root);
			return 0;
		}
		appinfo.user_data_list[i].ip = strdup(json_string_value(tmp));

		tmp = json_object_get(data, "mapped_port");
		if(!json_is_string(tmp))
		{
			fprintf(stderr, "error: user %d: port is not a string\n", i + 1);
			return 0;
		}
		appinfo.user_data_list[i].port = strdup(json_string_value(tmp));

	}

	json_decref(root);

	char tmp_string[500];
	for(i=0; i<json_data_len; ++i) {
		append_textview(appinfo.textview, return_string(sprintf(tmp_string,"user[%u] = %s, %s, %s, %s, %s, %s\n",
				i,
				appinfo.user_data_list[i].id, appinfo.user_data_list[i].firstname,
				appinfo.user_data_list[i].lastname, appinfo.user_data_list[i].fullname,
				appinfo.user_data_list[i].ip, appinfo.user_data_list[i].port),tmp_string));
		printf("user[%u] = %s, %s, %s, %s, %s, %s\n",
						i,
						appinfo.user_data_list[i].id, appinfo.user_data_list[i].firstname,
						appinfo.user_data_list[i].lastname, appinfo.user_data_list[i].fullname,
						appinfo.user_data_list[i].ip, appinfo.user_data_list[i].port);
	}

	return nmemb;
}

void refresh_user_list_cb(GtkWidget *button, gpointer user_data)
{
	char postData[300];
	sprintf(postData, "user_id=%d&mapped_ip=%s&mapped_port=%d", atoi(appinfo.user_id), appinfo.remoteinfo->ip, appinfo.remoteinfo->port);

	CURL *curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, "http://dev.chineseforall.org/liveClassroom/register_refresh_data");
	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, postData);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, refresh_user_id);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &appinfo);

	CURLcode success = curl_easy_perform(curl_handle);

	if(success == CURLE_OK)
		append_textview(appinfo.textview, "[REFRESH] curl successfully executed:\n");
	else
		append_textview(appinfo.textview, "[REFRESH] curl did not successfully execute.\n");
}


void user_list_changed_cb(GtkComboBox *w, gpointer user_data) {
	int index = gtk_combo_box_get_active(GTK_COMBO_BOX(w));
	appinfo.selected_user = &appinfo.user_data_list[index];
	char tmp_string[500];
	append_textview(appinfo.textview, return_string(sprintf(tmp_string,"selected user = %s\n",
			appinfo.selected_user->fullname), tmp_string));
}

void input_list_changed_cb(GtkComboBox *w, gpointer user_data) {
	int index = gtk_combo_box_get_active(GTK_COMBO_BOX(w));
	char tmp_string[500];
	append_textview(appinfo.textview, return_string(sprintf(tmp_string,"selected input = %s\n",
			gtk_combo_box_get_active_text(GTK_COMBO_BOX(w))), tmp_string));

	changeDeviceRequested = TRUE;
	while(listenerThreadRunning == TRUE);
	CreateThread(NULL, 0, playThread3, (LPVOID *)NULL, 0, NULL);
}

void output_list_changed_cb(GtkComboBox *w, gpointer user_data) {
	int index = gtk_combo_box_get_active(GTK_COMBO_BOX(w));
	char tmp_string[500];
	//append_textview(appinfo.textview, return_string(sprintf(tmp_string,"selected input = %s, pa_index = %d\n",
			//gtk_combo_box_get_active_text(GTK_COMBO_BOX(w)), appinfo.outputDevices[index].device_pa_index), tmp_string));

	printf("selected input = %s, pa_index = %d\n", gtk_combo_box_get_active_text(GTK_COMBO_BOX(w)),
			appinfo.outputDevices[index].device_pa_index);
	changeDeviceRequested = TRUE;
	while(listenerThreadRunning == TRUE);

	CreateThread(NULL, 0, playThread, (LPVOID *)appinfo.ddata, 0, NULL);
}

gboolean cb_login_event(GtkWidget *signin_btn, GdkEventButton *event, gpointer user_data)
{
	LoginData *login_data = (LoginData *)user_data;

	const char *email = gtk_entry_get_text(GTK_ENTRY(login_data->email));
	const char *password = gtk_entry_get_text(GTK_ENTRY(login_data->password));

	printf("Login called --> email: %s password: %s\n", email, password);

	CURL *curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, "http://dev.chineseforall.org/liveClassroom/remote_login");
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, user_data);

	char postData[300];
	sprintf(postData, "email=%s&password=%s", email, password);

	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, postData);

	CURLcode success = curl_easy_perform(curl_handle);

	if(success == CURLE_OK)
		gtk_widget_hide(login_data->login_box);
	else
		gtk_entry_set_text(GTK_ENTRY(login_data->email), "Curl login fail");

	return FALSE;
}
/* Draws a border around a widget using cairo */
gboolean widget_border_expose_cb(GtkWidget *wi, GdkEventExpose *ev, gpointer user_data)
{
	cairo_t *cr;

	/* Create a cairo context... */
	cr = gdk_cairo_create(wi->window);

	/* ...make it blue... */
	cairo_set_source_rgb(cr, 0.6f, 0.6f, 0.6f);
	/* ...set the stroke width... */
	cairo_set_line_width(cr, 1);
	/* ...draw a rectangle covering the bounds of the widget... */
	cairo_rectangle(cr, wi->allocation.x, wi->allocation.y, wi->allocation.width, wi->allocation.height);
	/* ...clip our drawing to prevent errors... */
	cairo_clip_preserve(cr);
	/* ...draw the line... */
	cairo_stroke(cr);

	/* ...and destroy the context */
	cairo_destroy(cr);

	return FALSE;
}

/*******************************************************************/
int main(int argc, char **argv)
{
	GtkWidget *window, *scrolled_win, *textview;
	GtkTextBuffer *buffer;
	PangoFontDescription *font;
	char tmp_string[128];

	gtk_rc_parse("themes/Clearlooks-Quicksilver/gtk-2.0/gtkrc");
	gtk_init (&argc, &argv);

	GtkWidget *statusbar = gtk_label_new("Ready");

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "Live Classroom Audio Test");
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_widget_set_size_request (window, 346, 550);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	// Login Popup
	GtkWidget *login_box = gtk_dialog_new();
	gtk_window_set_position(GTK_WINDOW(login_box), GTK_WIN_POS_CENTER);
	GtkWidget *login_box_content = gtk_dialog_get_action_area(GTK_DIALOG(login_box));

	GtkWidget *frm_halign = gtk_alignment_new(0,0,0,0);
	GtkWidget *frm_vbox = gtk_vbox_new(TRUE, 10);

	gtk_container_add(GTK_CONTAINER(login_box_content), frm_halign);
	gtk_container_add(GTK_CONTAINER(frm_halign), frm_vbox);

	gtk_container_set_border_width(GTK_CONTAINER(frm_vbox), 5);

	// E-mail field
	GtkWidget *Email_label = gtk_label_new("E-mail  ");
	GtkWidget *email_textfield = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(email_textfield),"husman85@gmail.com");
	GtkWidget *frm_field_halign = gtk_alignment_new(0,0,0,0);
	GtkWidget *frm_field_hbox = gtk_hbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(frm_vbox), frm_field_halign);
	gtk_container_add(GTK_CONTAINER(frm_field_halign), frm_field_hbox);
	gtk_container_add(GTK_CONTAINER(frm_field_hbox), Email_label);
	gtk_container_add(GTK_CONTAINER(frm_field_hbox), email_textfield);

	// Password field
	GtkWidget *password_label = gtk_label_new("Password");
	GtkWidget *password_textfield = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(password_textfield), FALSE);
	frm_field_halign = gtk_alignment_new(0,0,0,0);
	frm_field_hbox = gtk_hbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(frm_vbox), frm_field_halign);
	gtk_container_add(GTK_CONTAINER(frm_field_halign), frm_field_hbox);
	gtk_container_add(GTK_CONTAINER(frm_field_hbox), password_label);
	gtk_container_add(GTK_CONTAINER(frm_field_hbox), password_textfield);

	// Buttons
	GtkWidget *frm_sign_in_button = gtk_event_box_new();
	GtkWidget *frm_sign_in_image = gtk_image_new_from_file_utf8("images/login.jpg");

	frm_field_halign = gtk_alignment_new(1,0,0,0);
	frm_field_hbox = gtk_hbox_new(FALSE, 10);

	gtk_container_add(GTK_CONTAINER(frm_vbox), frm_field_halign);
	gtk_container_add(GTK_CONTAINER(frm_field_halign), frm_field_hbox);
	gtk_container_add(GTK_CONTAINER(frm_field_hbox), frm_sign_in_button);
	gtk_container_add(GTK_CONTAINER(frm_sign_in_button), frm_sign_in_image);

	//gtk_button_set_relief (GTK_BUTTON (frm_sign_in_button), GTK_RELIEF_NONE);
	//gtk_button_set_relief (GTK_BUTTON (frm_cancel_button), GTK_RELIEF_NONE);

	//gtk_widget_modify_font(frm_sign_in_label, pango_font_description_from_string ("Monospace Bold 17"));
	//gtk_widget_modify_font(frm_cancel_label, pango_font_description_from_string ("Monospace Bold 17"));

	g_signal_connect( G_OBJECT( login_box ), "delete-event", G_CALLBACK( cb_delete_event ), NULL );
	g_signal_connect( G_OBJECT( login_box ), "destroy", G_CALLBACK( gtk_main_quit ), NULL );


	LoginData login_data;
	login_data.login_box = login_box;
	login_data.email = email_textfield;
	login_data.password = password_textfield;
	login_data.textview = textview;
	login_data.statusbar = statusbar;

	g_signal_connect(G_OBJECT(frm_sign_in_button), "button_press_event", G_CALLBACK(cb_login_event), &login_data);

	gtk_widget_show_all(login_box);
	gtk_dialog_run(GTK_DIALOG(login_box));



	font = pango_font_description_from_string ("Monospace Bold 10");
	textview = gtk_text_view_new ();

	gtk_widget_modify_font (textview, font);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (textview), GTK_WRAP_WORD);
	//gtk_text_view_set_justification (GTK_TEXT_VIEW (textview), GTK_JUSTIFY_RIGHT);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (textview), TRUE);
	gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (textview), TRUE);
	gtk_text_view_set_pixels_above_lines (GTK_TEXT_VIEW (textview), 5);
	gtk_text_view_set_pixels_below_lines (GTK_TEXT_VIEW (textview), 5);
	gtk_text_view_set_pixels_inside_wrap (GTK_TEXT_VIEW (textview), 5);
	gtk_text_view_set_left_margin (GTK_TEXT_VIEW (textview), 10);
	gtk_text_view_set_right_margin (GTK_TEXT_VIEW (textview), 10);

	gtk_widget_set_size_request(textview, 346, 150);


	GtkWidget *vbox = gtk_vbox_new(FALSE, 15);
	GtkWidget *valign = gtk_alignment_new(0,0,1,0);

	gtk_container_add(GTK_CONTAINER(window), valign);
	gtk_container_add(GTK_CONTAINER(valign), vbox);

	/*GtkWidget *btn1 = gtk_button_new_with_label("Open File");

	GdkColor red = {0, 0xffff, 0x0000, 0x0000};
	GdkColor green = {0, 0x0000, 0xffff, 0x0000};
	GdkColor blue = {0, 0x0000, 0x0000, 0xffff};
	gtk_widget_modify_bg(btn1, GTK_STATE_NORMAL, &red);
	gtk_widget_modify_bg(btn1, GTK_STATE_PRELIGHT, &green);
	gtk_widget_modify_bg(btn1, GTK_STATE_ACTIVE, &blue);

	gtk_container_add(GTK_CONTAINER(vbox), btn1);*/


	/* input/output device combo lists */
	GtkWidget *input_device_list = gtk_combo_box_new_text();
	GtkWidget *output_device_list = gtk_combo_box_new_text();

	int err = Pa_Initialize();
	if(err != paNoError)
		append_textview(textview, "Could not initialize the audio system.\n");

	int numDevices;
	numDevices = Pa_GetDeviceCount();
	if(numDevices < 0)
		append_textview(textview, "No devices input devices found on your system.\n");

	const PaDeviceInfo *deviceInfo;
	int defaultInputIndex = Pa_GetDefaultInputDevice();
	int defaultOutputIndex = Pa_GetDefaultOutputDevice();
	int inputIndex = 0, outputIndex = 0;

	printf("default input device index = %d\n", defaultInputIndex);
	printf("default output device index = %d\n", defaultOutputIndex);;

	for(int i=0; i<numDevices; ++i) {
		deviceInfo = Pa_GetDeviceInfo(i);
		printf("device = %s, inputs = %d, outputs = %d, index = %d\n", deviceInfo->name,
				deviceInfo->maxInputChannels, deviceInfo->maxOutputChannels, i);
		if(deviceInfo->maxInputChannels > 0) {
			gtk_combo_box_append_text(GTK_COMBO_BOX(input_device_list), deviceInfo->name);
			if(i == defaultInputIndex)
				gtk_combo_box_set_active(GTK_COMBO_BOX(input_device_list), inputIndex);
			appinfo.inputDevices[inputIndex].device_list_index = inputIndex;
			appinfo.inputDevices[inputIndex].device_pa_index = i;
			++inputIndex;
		} else if(deviceInfo->maxOutputChannels > 0) {
			gtk_combo_box_append_text(GTK_COMBO_BOX(output_device_list), deviceInfo->name);
			if(i == defaultOutputIndex)
				gtk_combo_box_set_active(GTK_COMBO_BOX(output_device_list), outputIndex);
			appinfo.outputDevices[outputIndex].device_list_index = outputIndex;
			appinfo.outputDevices[outputIndex].device_pa_index = i;
			++outputIndex;
		}
	}

	printf("active input device index = %d\n", gtk_combo_box_get_active(GTK_COMBO_BOX(input_device_list)));
	printf("active output device index = %d\n", gtk_combo_box_get_active(GTK_COMBO_BOX(output_device_list)));


	GtkWidget *vbox2 = gtk_vbox_new(FALSE, 3);
	GtkWidget *valign2 = gtk_alignment_new(0,0,1,0);

	gtk_container_add(GTK_CONTAINER(vbox), valign2);
	gtk_container_add(GTK_CONTAINER(valign2), vbox2);

	GtkWidget *input_devices_label = gtk_label_new("Microphone (Input Device)");
	gtk_misc_set_alignment(GTK_MISC(input_devices_label), 0.015f, 0.0f);
	gtk_container_add (GTK_CONTAINER (vbox2), input_devices_label);
	gtk_container_add (GTK_CONTAINER (vbox2), input_device_list);

	GtkWidget *vbox3 = gtk_vbox_new(FALSE, 3);
	GtkWidget *valign3 = gtk_alignment_new(0,0,1,0);

	gtk_container_add(GTK_CONTAINER(vbox), valign3);
	gtk_container_add(GTK_CONTAINER(valign3), vbox3);

	GtkWidget *output_devices_label = gtk_label_new("Speaker (Output Device)");
	gtk_container_add (GTK_CONTAINER (vbox3), output_devices_label);
	gtk_misc_set_alignment(GTK_MISC(output_devices_label), 0.015f, 0.0f);
	gtk_container_add (GTK_CONTAINER (vbox3), output_device_list);

	GtkWidget *contentTabs = gtk_notebook_new();
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(contentTabs), GTK_POS_TOP);

	// Tab #1
	GtkWidget *tab1Label = gtk_label_new("Stream File");

	GdkColor normal_color, active_color;
	gdk_color_parse ("#ff0000", &normal_color);
	gdk_color_parse ("#333333", &active_color);

	GtkStyle *s = gtk_style_new();
	s->font_desc = pango_font_description_from_string ("Monospace Bold 9");
	s->fg[GTK_STATE_NORMAL] = normal_color;
	s->fg[GTK_STATE_ACTIVE] = active_color;

	//gtk_widget_set_style(tab1Label, s);

	GtkWidget *vboxTab1 = gtk_vbox_new(FALSE, 3);
	GtkWidget *valignTab1 = gtk_alignment_new(0,0,1,0);
	gtk_container_add(GTK_CONTAINER(valignTab1), vboxTab1);

	GtkWidget *tab1Test = gtk_label_new("\nYou can stream '.wav' audio file to \n"
			"another peer. Select the user to\nstream to and audio file to begin.\n");
	gtk_container_add(GTK_CONTAINER(vboxTab1), tab1Test);

	gtk_container_set_border_width(GTK_CONTAINER(vboxTab1), 5);

	GtkWidget *file_browse =  gtk_button_new_with_label("Browse");
	GtkWidget *refresh_user_list_btn =  gtk_button_new_with_label("Refresh Users List");

	GtkWidget *user_list = gtk_combo_box_new_text();

	g_signal_connect(user_list, "changed", G_CALLBACK(user_list_changed_cb), NULL);

	g_signal_connect(output_device_list, "changed", G_CALLBACK(output_list_changed_cb), NULL);

	gtk_container_add(GTK_CONTAINER(vboxTab1), refresh_user_list_btn);
	gtk_container_add(GTK_CONTAINER(vboxTab1), user_list);
	gtk_container_add(GTK_CONTAINER(vboxTab1), file_browse);

	gtk_notebook_append_page(GTK_NOTEBOOK(contentTabs), valignTab1, tab1Label);
	// Tab #2
	GtkWidget *tab1Label2 = gtk_label_new("Stream Desktop");
	//gtk_widget_set_style(tab1Label2, s);

	GtkWidget *vboxTab2 = gtk_vbox_new(FALSE, 3);
	GtkWidget *valignTab2 = gtk_alignment_new(0,0,1,0);
	gtk_container_add(GTK_CONTAINER(valignTab2), vboxTab2);

	GtkWidget *tab1Test2 = gtk_label_new("Some content here for tab #2....");
	gtk_container_add(GTK_CONTAINER(vboxTab2), tab1Test2);

	gtk_notebook_append_page(GTK_NOTEBOOK(contentTabs), valignTab2, tab1Label2);

	gtk_container_add(GTK_CONTAINER(vbox), contentTabs);



	//buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));
	//gtk_text_buffer_set_text (buffer, "This is some text!\nChange me!\nPlease!", -1);
	scrolled_win = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_container_add (GTK_CONTAINER (scrolled_win), textview);
	gtk_container_add (GTK_CONTAINER (vbox), scrolled_win);

	gtk_container_set_border_width(GTK_CONTAINER(scrolled_win), 3);
	g_signal_connect(scrolled_win, "expose-event", G_CALLBACK(widget_border_expose_cb), NULL);

	g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	GtkAdjustment *textview_vscroll = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolled_win));
	gtk_adjustment_set_value(textview_vscroll, gtk_adjustment_get_upper(textview_vscroll));

	GtkWidget *statusbar_container = gtk_event_box_new();
	GtkWidget *statusbar_container2 = gtk_event_box_new();
	GtkWidget *statusbar_container3 = gtk_event_box_new();

	GdkColor sb_bg_color, window_bg_color, sb_text_color;
	gdk_color_parse ("#777777", &sb_bg_color);
	gdk_color_parse ("#eeeeee", &window_bg_color);
	//gdk_color_parse ("#181818", &sb_text_color);

	gtk_container_add (GTK_CONTAINER (vbox), statusbar_container);
	gtk_container_add (GTK_CONTAINER (statusbar_container), statusbar_container2);
	gtk_container_add (GTK_CONTAINER (statusbar_container2), statusbar_container3);
	gtk_widget_modify_bg(statusbar_container, GTK_STATE_NORMAL, &sb_bg_color);
	gtk_widget_modify_bg(statusbar_container2, GTK_STATE_NORMAL, &window_bg_color);
	gtk_widget_modify_bg(statusbar_container3, GTK_STATE_NORMAL, &window_bg_color);
	//gtk_widget_modify_fg(statusbar, GTK_STATE_NORMAL, &sb_text_color);
	gtk_widget_modify_font(statusbar, pango_font_description_from_string ("Monospace Bold 9"));

	//gtk_widget_set_size_request(statusbar, 346, 30);

	gtk_container_set_border_width(GTK_CONTAINER(statusbar_container2), 1);
	gtk_container_set_border_width(GTK_CONTAINER(statusbar_container3), 5);

	gtk_label_set_ellipsize(GTK_LABEL(statusbar), PANGO_ELLIPSIZE_END);

	gtk_container_add (GTK_CONTAINER (statusbar_container3), statusbar);



	CStunClientHelper clientHelper ("stunserver.org");
	NAT_TYPE Nat = clientHelper.GetNatType (appinfo.textview);

	cout << endl;
	switch (Nat)
	{
	case ERROR_DETECTING_NAT:
		cout << "There was an error detecting NAT." << endl;
		break;

	case FIREWALL_BLOCKS_UDP:
		cout << "There is a firewall that blocks UDP." << endl;
		break;

	case FULL_CONE_NAT:
		cout << "The NAT type is Full Cone NAT." << endl;
		break;

	case OPEN_INTERNET:
		cout << "There is no NAT and directly on Open Internet." << endl;
		break;

	case RESTRICTED_CONE_NAT:
		cout << "The NAT type is Restricted Cone NAT." << endl;
		break;

	case RESTRICTED_PORT_CONE_NAT:
		cout << "The NAT type is Restricted Port Cone NAT." << endl;
		break;

	case SYMMETRIC_NAT:
		cout << "The NAT type is Symmetric NAT." << endl;
		break;

	case SYMMETRIC_UDP_FIREWALL:
		cout << "There is a symmetric UDP firewall." << endl;
		break;
	}

	struct in_addr local_addr[10];
	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		cerr << "Error " << WSAGetLastError() <<
				" when getting local host name." << endl;
		return 0;
	}
	append_textview(textview, return_string(sprintf(tmp_string, "Your Host name is: %s.\n",ac), tmp_string));
	cout << "Host name is " << ac << "." << endl;

	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) {
		append_textview(textview, "Something went wrong while trying to look up your host name. ");
		cerr << "Yow! Bad host lookup." << endl;
		return 0;
	}

	append_textview(textview, "Detecting your network card[s] and their location...\n");
	for (int i = 0; phe->h_addr_list[i] != 0 && i < 10; ++i) {
		memcpy(&local_addr[i], phe->h_addr_list[i], sizeof(struct in_addr));
		append_textview(textview, return_string(sprintf(tmp_string, "Network Card #%d, Address: %s\n", i+1, inet_ntoa(local_addr[i])), tmp_string));
		cout << "Network Card #" << (i+1) << ", Address: " << inet_ntoa(local_addr[i]) << endl;
	}

	append_textview(textview, "Attemping to transverse through your network (NAT Detection)...\n");

	SOCKADDR_IN addr;
	clientHelper.GetStunMappedAddress (&addr);
	char *ip = inet_ntoa(addr.sin_addr);
	unsigned short mapped_port = htons(addr.sin_port);

	append_textview(textview, return_string(sprintf (tmp_string, "Mapped Address: %s:%d\n", ip, mapped_port), tmp_string));
	char postData[300];
	sprintf(postData, "user_id=%d&mapped_ip=%s&mapped_port=%d", atoi(appinfo.user_id), ip, mapped_port);
	//cout << "\n\nMapped Address: " << ip << ":" << mapped_port <<"\n\n";*/

	RemoteInfo remoteinfo;
	remoteinfo.ip = ip;
	remoteinfo.local_ip = inet_ntoa(local_addr[0]);
	remoteinfo.port = mapped_port;
	appinfo.remoteinfo = &remoteinfo;
	appinfo.window = window;
	appinfo.textview = textview;
	appinfo.statusbar = statusbar;
	appinfo.user_list = user_list;
	appinfo.input_device_list = input_device_list;
	appinfo.output_device_list = output_device_list;

	CURL *curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, "http://dev.chineseforall.org/liveClassroom/register_load_data");
	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, postData);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, registered_user_id);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &appinfo);

	CURLcode success = curl_easy_perform(curl_handle);

	if(success == CURLE_OK)
		append_textview(textview, "curl successfully executed:\n");
	else
		append_textview(textview, "curl did not successfully execute.\n");

	g_signal_connect(refresh_user_list_btn, "clicked", G_CALLBACK(refresh_user_list_cb), NULL);

	WidgetData wd;
	wd.textview = textview;
	wd.window = window;
	wd.textview_scroll = scrolled_win;
	wd.rtpsession = NULL;
	wd.statusbar = statusbar;

	//quick_message("testing 123...", window);

	g_signal_connect(file_browse, "clicked", G_CALLBACK(stream_wave_file_cb), &wd);

	append_textview(textview, return_string(sprintf(tmp_string, "Module = %s\n", gtk_rc_get_module_dir()), tmp_string));

	append_textview(textview, return_string(sprintf(tmp_string, "CURRENT_USER_ID = %s\n", appinfo.user_id), tmp_string));

	gtk_widget_show_all (window);

	gtk_main();

	return 0;
}
