#include "globals.h"

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

    val = (val << 8)
        | ((val >> 8) & 0x00FF);

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
            else if(bytes_remaining > 0 && wInfo->wFormatTag == WAVE_FORMAT_PCM) {
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


int main(int argc, char **argv)
{
    WaveInfo wInfo;
    if(decodeWave(argv[1], &wInfo) == -1) {
        printf("Failed to decode input file.\n");
        return 0;
    }

    // Write to FLV
    FILE *output;

    output = fopen("output.flv", "wb");
    if(output == NULL) {
    	printf("Could not open output file.\n");
    	return 0;
    }

    uint8_t bytes[13];

    bytes[0] = 'F';
    bytes[1] = 'L';
    bytes[2] = 'V';
    bytes[3] = 0x01;
    bytes[4] = 0x04;
    bytes[5] = 0x00;
    bytes[6] = 0x00;
    bytes[7] = 0x00;
    bytes[8] = 0x09;
    bytes[9] = 0x00;
	bytes[10] = 0x00;
	bytes[11] = 0x00;
	bytes[12] = 0x00;

    fwrite(bytes,13,1,output);

    // Begin writing data
    int have_data = 1;
    uint32_t timestamp = 1000;
    int i = 0,z=0;

    char cbits[320];
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
    state = speex_encoder_init(&speex_nb_mode);

    speex_encoder_ctl(state,SPEEX_GET_FRAME_SIZE,&frame_size);

    printf("frame size = %d\n",frame_size);

    /*Set the quality to 8 (15 kbps)*/
    tmp=8;
    speex_encoder_ctl(state, SPEEX_SET_QUALITY, &tmp);
    int sr=16000;
    speex_encoder_ctl(state, SPEEX_SET_SAMPLING_RATE, &sr);
    int sr2;
    speex_encoder_ctl(state, SPEEX_GET_SAMPLING_RATE, &sr2);

    printf("sampling rate = %d\n", sr2);

    int offs = 0;
    while(have_data) {
    	// Write 'FLV TAG' fields

    	// type
    	bytes[0] = 0x08;

		/*Flush all the bits in the struct so we can encode a new frame*/
		speex_bits_reset(&bits);

		/*Encode the frame*/
		speex_encode_int(state, &(wInfo.data[offs]), &bits);
		offs += frame_size;

		/*Copy the bits to an array of char that can be written*/
		nbBytes = speex_bits_write(&bits, cbits, frame_size);

		//nbBytes = wInfo.dwAvgBytesPerSec;

    	// data size
    	bytes[1] = (nbBytes+1) >> 16;
    	bytes[2] = (nbBytes+1) >> 8;
    	bytes[3] = (nbBytes+1);

    	//printf("data size = %d\n", nbBytes);

    	// timestamp
    	bytes[4] = timestamp >> 16;
		bytes[5] = timestamp >> 8;
		bytes[6] = timestamp;


		// extended timestamp
		bytes[7] = 0x00;


		// stream id
		bytes[8] = 0x00;
		bytes[9] = 0x00;
		bytes[10] = 0x00;
		bytes[11] = 0xB6; //0x36;

		fwrite(bytes,12,1,output);

		/*Write the compressed data*/
		z=fwrite(cbits, 1, nbBytes, output);

    	// Write 'Byte Rate' number of bytes
		//z=fwrite(wInfo.data,sizeof(short),wInfo.dwAvgBytesPerSec/2,output);
		//wInfo.data += wInfo.dwAvgBytesPerSec/2;
		printf("of %d bytes, Wrote %d bytes [%d]\n",wInfo.dwAvgBytesPerSec,z,(i+1));

		// Write previous tag size
		bytes[0] = (nbBytes+12) >> 16;
		bytes[1] = (nbBytes+12) >> 8;
		bytes[2] = (nbBytes+12);
		bytes[3] = 0x00;
		fwrite(bytes,4,1,output);

		timestamp += 20;
		if(i++ == 241)
			break;

    }

    /*Destroy the encoder state*/
   speex_encoder_destroy(state);
   /*Destroy the bit-packing struct*/
   speex_bits_destroy(&bits);

    fclose(output);

    return 0;
}
