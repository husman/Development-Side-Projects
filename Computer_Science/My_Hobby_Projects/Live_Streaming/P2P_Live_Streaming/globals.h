#ifndef _GLOBALS_H_
#define _GLOBALS_H_
// To make our codes more cross-platform and without the need to continuously
// rewrite our codes for different platforms, we include this local inttypes
// header to define the standard fix-width data types part of the C99 standard.

// The ISO/IEC 9899:1999 (E), or C99 standard, defines the prototypes for
// 'Format conversion of integer types' on page 197 (page 211 in pdf).
// The implementation for these 'Format conversion of integer types' is
// defined on page 254 (or page 268 of the pdf).

// Microsoft VC compilers, which are based on the C++0x language specification,
// does not support these standard types. These types were later defined. Microsoft
// selectively added support for particular C++11 language specifications, however
// these standard integer types were not the chosen few features Microsoft implemented.
// Therefore, we include this header, which fills in the necessary gap for us to use
// these types by defining them. This header does not make the VC compiler completely
// compatible with the C99 or C++11 standards, however, it will do for this project.

// Reference documents:
// You can find ISO/IEC 9899:1999 (E), aka C99 at:
// http://cs.nyu.edu/courses/spring13/CSCI-GA.2110-001/downloads/C99.pdf

// You can find the ISO/IEC 14882:2003(E), aka C++11 at:
// http://cs.nyu.edu/courses/spring13/CSCI-GA.2110-001/downloads/C++%20Standard%202003.pdf

// Additional references:
// 1. http://msdn.microsoft.com/en-us/library/vstudio/hh567368.aspx
// 2. https://www2.research.att.com/~bs/what-is-2009.pdf
#include <stdint.h>

// We include our standard input/output library. I like 'printf'
// and of course for our file input/output data structures/algorithms
#include <stdio.h>
#include <stdlib.h>
#include <speex/speex.h>
#include <speex/speex_echo.h>
#include <speex/speex_preprocess.h>
#include <string.h>

#include <gtk/gtk.h>

#include <portaudio.h>
#include <ortp/ortp.h>
#include <ortp/rtp.h>
#include <ortp/stun.h>

#include <curl/curl.h>

#include <jansson.h>

#include "queue.h"

#include <windows.h>

typedef struct WaveInfo {
    // The WAVE file format is a subset of the RIFF, which is an extension of IFF.
    // The IFF specification defines the concept and structure of a chunk.
    // This chunk is contains a 4-byte sequence, a 32-bit 'chunk size',
    // and a 'data' section of size 'chunk size'.
    // See http://www.digitalpreservation.gov/formats/fdd/fdd000115.shtml
    // Document link: http://www.martinreddy.net/gfx/2d/IFF.txt

    // However, Wave is a subset of RIFF, which is an extension of IFF.
    // RIFF extends the 'data' section of IFF to, optionally, include
    // sub chunks. Therefore, RIFF documents can contain nested chunks
    // However, Only the RIFF and LIST chunk types can contain other
    // chunks. All other chunks are defined precisely as in IFF That is,
    // their data section does not contain other chunks.
    // Document link: http://www.kk.iij4u.or.jp/~kondo/wave/mpidata.txt

   /* According to RIFF's specification (2-2):
   -------------------------------------------------------
   'We can represent a chunk with the following notation
    (in this example, the ckSize and pad byte are
    implicit):RIFF chunk;notation

     <ckID> ( <ckData> )

    Two types of chunks, the LIST and RIFF chunks, may
    contain nested chunks, or subchunks. These special
    chunk types are discussed later in this document. All
    other chunk types store a single element of binary data
    in <ckData>.'

    The basic building block of a RIFF file is called a
    chunk. Using C syntax, a chunk can be defined as
    follows:RIFF chunk;defined in C syntax

    typedef unsigned long DWORD;
    typedef unsigned char BYTE;

    typedef DWORD FOURCC;   // Four-character code

    typedef FOURCC CKID;    // Four-character-code chunk identifier
    typedef DWORD CKSIZE;   // 32-bit unsigned size value

    ----------------------------------------------------
    Part      Description
    ----------------------------------------------------
    ckID      A [four-character code] that identifies the
              representation of the chunk data data. A
              program reading a RIFF file can skip over
              any chunk whose chunk ID it doesn't
              recognize; it simply skips the number of
              bytes specified by ckSize plus the pad
              byte, if present.

    ckSize    A 32-bit unsigned value identifying the
              size of ckData. This size value does not
              include the size of the ckID or ckSize
              fields or the pad byte at the end of
              ckData.
    */

    // Beginning of our RIFF chunk. All other chunks are nested in this one
    uint32_t    riff_chunk_id; // Chunk id is an unsigned 4-bytes character code
    uint32_t    riff_chunk_size; //  Chuck size is an unsigned 32-bit integer

    /* Data section of RIFF Chunk. First we define the format of our RIFF.
       Then we define all the necessary chunks for our RIFF format. */
    uint32_t    riff_format; // This identifies what sort of RIFF file we have.

    // Wave is defined in the RIFF specifications has consisting
    // of two sub chunks. The two sub chunks are 'fmt' and 'data'
    // The 'fmt' chunk describes the properties of our audio/data.
    uint32_t    fmt_chunk_id;
    uint32_t    fmt_chunk_size;

    /*
        *NOTE on BNF definitions/convention*
        ------------------------------------
        Elements on a new line do not represent 'alternatives'. If
        'alternatives' is meant, then a '|' (OR) operator will
        separate the alterative fields. Therefore:

        <item>: <ckID> (
                            <item_A>
                            <item_B>
                        }

        is the same as:

        <item>: <ckID> ( item_A> <item_B> )

        Therefore, a new line does not represent 'alternatives'.
        Separating our items in new lines represents concatenation
        with a single space between the items. To express
        'alternatives', the following conventions are adopted:

        <item>: <ckID> (
                            <item_A> |
                            <item_B>
                         }

        This can also be represented in a single line as:

        <item>: <ckID> ( <item_A> | <item_B> )

    Remember from above, the items between the opening '(' and
    closing ')' represent the chunk data. The 'chunk size' field is
    always implicitly implied with this notation, as stated by RIFF.

    RIFF defines the 'fmt', or format, chunk as the following (BNF definition):
    <fmt-ck>: fmt (
                    <common-fields>
                    <format-specific-fields>
                  )

    <common-fields>:
          struct
            {
               WORD     wFormatTag;         // Format category (format of audio)
               WORD     wChannels;          // Number of channels
               DWORD    dwSamplesPerSec;    // Sampling rate
               DWORD    dwAvgBytesPerSec;   // For buffer estimation
               WORD     wBlockAlign;        // Data block size
            }

    *IMPORTANT NOTE I WILL ADD*
    ------------------------------------------------
    WORD    = unsigned 2-bytes --> unsigned 16-bits;
    DWORD   = unsigned 4-bytes --> unsigned 32-bits;
    ------------------------------------------------

    the <format-specific-fields> depends on the wFormatTag we are using.
    According to the specificiation document:
    ------------------------------------------------------------------
    'The format category of a WAVE file is specified by the
     value of the wFormatTag field of the fmt chunk. The
     representation of data in <wave-data>, and the content
     of the <format-specific-fields> of the fmt chunk,
     depend on the format category.Waveform files;format
     categories

     The currently defined open non-proprietary WAVE format
     categories are as follows:


     wFormatTag Value           Format Category
     WAVE_FORMAT_PCM (0x0001)   Microsoft Pulse Code Modulation (PCM) format


     The following are the registered proprietary WAVE format categories:

     wFormatTag Value         Format Category
     IBM_FORMAT_MULAW         IBM mu-law format
     (0x0101)
     IBM_FORMAT_ALAW (0x0102) IBM a-law format
     IBM_FORMAT_ADPCM         IBM AVC Adaptive
     (0x0103)                 Differential Pulse Code
                              Modulation format'
    ------------------------------------------------------------------

    In our case, we will only support WAVE_FORMAT_PCM, or PCM. Therefore,
    we will expect the 32-bit wFormatTag field to take on the value 0x00000001.
    Because 32-bit integers are usually stored in Big Endian, we expect the
    value 0x10000000 for PCM wave in the actual file.

    Also, if our wFormatTag is PCM, then the <format-specific-fields> (see above)
    will have 1 additional field, 'Bits per sample', Therefore, we need to add 1
    field to our <format-specific-fields> section. As the document states:

    ------------------------------------------------------------------
    'The following sections describe the Microsoft
     WAVE_FORMAT_PCM format.


     Pulse Code Modulation (PCM) Format

     If the wFormatTag field of the <fmt-ck> is set to
     WAVE_FORMAT_PCM, then the waveform data consists of
     samples represented in pulse code modulation (PCM)
     format. For PCM waveform data, the <format-specific-
     fields> is defined as follows:

      <PCM-format-specific>:
            struct
            {
               WORD wBitsPerSample;      // Sample size
            }

     The wBitsPerSample field specifies the number of bits
     of data used to represent each sample of each channel.
     If there are multiple channels, the sample size is the
     same for each channel.'
     ------------------------------------------------------------------

     Therefore, our format chunk definition expands to:

     <fmt-ck>: fmt (
                     <common-fields>
                     <format-specific-fields>
                   )

     <common-fields>:
            struct
            {
               WORD     wFormatTag;         // Format category (format of audio)
               WORD     wChannels;          // Number of channels
               DWORD    dwSamplesPerSec;    // Sampling rate
               DWORD    dwAvgBytesPerSec;   // For buffer estimation
               WORD     wBlockAlign;        // Data block size
            }

    <format-specific-fields>:
            struct
            {
               WORD wBitsPerSample;         // Sample size
            }

    *IMPORTANT NOTE I WILL ADD #2*
    ------------------------------------------------
    WORD    = unsigned 2-bytes --> uint16_t;
    DWORD   = unsigned 4-bytes --> uint32_t;
    ------------------------------------------------
    */

    // --------------------------------------------------
    // + Implementation of the Format Chunk's fields    +
    // + Note we prefix our variables with the size     +
    // + of the data type. For example, wFormatTag      +
    // + states that 'FormatTag' is of size 'WORD' and  +
    // + in 'dwSamplesPerSec', 'dw' stands for 'DWORD'. +
    // + The types are defined in the notes above.      +
    // --------------------------------------------------
    // <common-fields>:
    uint16_t wFormatTag;        // Format category (PCM = 1)
    uint16_t wChannels;         // Number of channels
    uint32_t dwSamplesPerSec;   // Sampling rate
    uint32_t dwAvgBytesPerSec;  // For buffer estimation (average bytes/sec)
    uint16_t wBlockAlign;       // Data block size

    // <format-specific-fields>
    uint16_t wBitsPerSample;    // Sample size

    // We may have these two additional fields, particularly if wFormatTag is not PCM.
    uint16_t wExtraParamSize;   // Extra parameter size (used by some non-PCM audio formats)
    uint8_t *extraParams;       // We wont know what the size of this array is until later

    /*
    Our format chunk is defined as:
    ------------------------------------------------------------
    <fmt-ck>: fmt (
                     <common-fields>
                     <format-specific-fields>
                  )
    ------------------------------------------------------------
    We have all the necessary fields, so we move onto the second
    field the WAVE RIFF format defines, the 'data' chunk.

    The 'data' chunk of a WAVE file is defined as:
    ------------------------------------------------------------
    'The <wave-data> contains the waveform data. It is
    defined as follows:

     <wave-data>:   { <data-ck> | <wave-list> }

     // NOTE: The specification replaces '<wave-list>' above
     // with '<data-list>'. However, I believe that is a typo,
     // so I am following what I assume is correct and was meant.

     <data-ck>:     data ( <wave-data> )

     <wave-list>:   LIST (
                            'wavl' { <data-ck> | <silence-ck> ... }
                         )

      <silence-ck>: slnt ( <dwSamples:DWORD> ) // Count of silent samples

     Note:  The slnt chunk represents silence, not
     necessarily a repeated zero volume or baseline sample.
     In 16-bit PCM data, if the last sample value played
     before the silence section is a 10000, then if data is
     still output to the D to A converter, it must maintain
     the 10000 value. If a zero value is used, a click may
     be heard at the start and end of the silence section.
     If play begins at a silence section, then a zero value
     might be used since no other information is available.
     A click might be created if the data following the
     silent section starts with a nonzero value.'
    ------------------------------------------------------------

    According to the definition above, the 'wave data' section is
    a sequence of 'data-ck' or 'wave-list' chunks. As the specification
    stated earlier, the only chunks that contain other chunks are
    the 'RIFF' and 'LIST' chunks.

    The <wave-list> chunk is a 'LIST' chunk whose data section
    contains a sequence of <data-ck> or <silence-ck>. The silence
    tells us that no sound is to be played for '<dwSamples:DWORD>'
    number of audio samples (or data samples). Therefore, if our
    data section is a <wave-list> chunk which contains alternating
    sequence of <data-ck> and <silence-ck>, then we will know that
    whenever we encounter a <silence-ck>, we must send 0 (zero)
    amplitude data to the speaker for the next <dwSamples:DWORD>
    sample counts. Alternatively, we can stop all processing for
    x number of seconds that corresponds to the processing/playback
    of <dwSamples:DWORD> number of sample counts. Therefore, <silence-ck>
    can be used to compress wave files because we would represent
    <dwSamples:DWORD> number of samples as a single 32-bit type, as
    opposed to <wBitsPerSample:WORD> (see above) number of bits.
    However, many decoders skip this optional <silence-ck>, therefore
    we will also skip it. Since we wont support <silence-ck>, there
    wont be a need to add support for <wave-list> chunks.

    The <data-ck>, on the other hand, is not a 'LIST' chunk.
    Therefore, it's data section will not contain other chunks.
    It's data section will contain the data defined by the 'fmt'
    chunk. This is the only part of the <wave-data> definition
    we will support because it is the only chunk type supported
    by most wave codecs.

    Depending on the <wFormatTag:WORD>, <wChannels:WORD>, and
    <wBitsPerSample:WORD>, values defined in the <fmt-ck>, we
    could have various layouts. <wBitsPerSample:WORD> determines
    how many bits we use per channel, and the <wChannels:WORD>
    determines how many channels do we have per audio sample.

    Some examples of layouts we can have for PCM are:
    *NOTE: Base 10, or decimal, is being used for these examples*

    -------------------
    + PCM_DATA_LAYOUT +
    -------------------

    (8-bit Mono):
    --------------------------------------------
    <wFormatTag:WORD>       =   1 (PCM)
    <wBitsPerSample:WORD>   =   8
    <wChannels:WORD>        =   1

    Sample  1   Sample  2   Sample  3   Sample  4
    Channel 0   Channel 0   Channel 0   Channel 0

    Where each 'Channel 0' is 8-bits.


    (8-bit Stereo):
    --------------------------------------------
    <wFormatTag:WORD>       =   1 (PCM)
    <wBitsPerSample:WORD>   =   8
    <wChannels:WORD>        =   2

         Sample  1            Sample  2             Sample  3
    Channel 0 Channel 1   Channel 0 Channel 1   Channel 0 Channel 1

    Where each 'Channel 0' is 8-bits and each 'Channel 1' is 8-bits.


    (16-bit Mono):
    --------------------------------------------
    <wFormatTag:WORD>       =   1 (PCM)
    <wBitsPerSample:WORD>   =   16
    <wChannels:WORD>        =   1

         Sample  1            Sample  2             Sample  3
    Channel 0 Channel 0   Channel 0 Channel 0   Channel 0 Channel 0

    Where each 'Channel 0' is 8-bits, therefore, 'Channel 0' = 16-bits.


    (16-bit Stereo):
    --------------------------------------------
    <wFormatTag:WORD>       =   1 (PCM)
    <wBitsPerSample:WORD>   =   16
    <wChannels:WORD>        =   2

                    Sample  1                               Sample  2
    Channel 0 Channel 0 Channel 1 Channel 1   Channel 0 Channel 0 Channel 1 Channel 1

    Where each 'Channel 0' is 8-bits and each 'Channel 1' is 8-bits.
    Therefore, 'Channel 0' = 16-bits and 'Channel 1' = 16-bits.

    Since we wont support for <wave-list>, the definition
    of what we will implement will change from:

    *Standard definition*
    --------------------------------------------------------------------
    <wave-data>:   { <data-ck> | <wave-list> }

    <data-ck>:     data ( <wave-data> | <wave-list> )

    <wave-list>:   LIST (
                            'wavl' { <data-ck> | <silence-ck> ... }
                         )

    <silence-ck>: slnt ( <dwSamples:DWORD> ) // Count of silent samples
    --------------------------------------------------------------------

    will change to our implementation definition:

    *Our Implementation definition*
    --------------------------------------------------------------------
    <wave-data>:   { <data-ck> }

    <data-ck>:     data ( PCM_DATA_LAYOUT )
    --------------------------------------------------------------------
   */

    // --------------------------------------------------
    // + Implementation of the Data Chunk's fields    +
    // + Note we prefix our variables with the size     +
    // + of the data type. For example, wFormatTag      +
    // + states that 'FormatTag' is of size 'WORD' and  +
    // + in 'dwSamplesPerSec', 'dw' stands for 'DWORD'. +
    // + The types are defined in the note below.      +
    // --------------------------------------------------
    // *IMPORTANT NOTE*
    // ------------------------------------------------
    // WORD    = unsigned 2-bytes --> uint16_t;
    // DWORD   = unsigned 4-bytes --> uint32_t;
    // ------------------------------------------------

    // <data-ck>:
    uint32_t    data_chunk_id;
    uint32_t    data_chunk_size;

    // Because we cannot determine <wFormatTag:WORD>, <wChannels:WORD>,
    // and <wBitsPerSample:WORD> until we read some actual data and
    // these fields can change, we will implement this type as a 2D
    // array. However, we also do not know the array size, therefore,
    // we cannot directly declare the type as an array. We will declare
    // it as a pointer to an unsigned 8-bit integer pointer. Therefore,
    // the data field will be an uint8_t pointer to an unit8_t pointer.
    // We will later allocate the rows and columns of our pointer
    // structure, once we know what the size is. We are using an unsigned
    // 8-bit pointer to a pointer because each channel can be expressed in
    // units of 8 bits. Therefore, if we have a Stereo channel, then that will
    // consume two 8-bit columns in our 2D array. Therefore, the columns of
    // our 2D array will contain the channels of our sample and each row
    // will contain another sample.

    short *data; // 2D array that will contain the WAVE's data

    int endian; // endian of our document (depends on RIFF or RIFX)

} WaveInfo;

#define LITTLE_ENDIAN   0
#define BIG_ENDIAN      1

const int cpu_endian_test = 1;
#define cpu_endian() ((*(char*)&cpu_endian_test) == 0? BIG_ENDIAN : LITTLE_ENDIAN)

enum RIFF_INFO {
    RIFF    = 0x52494646,
    RIFX    = 0x52494658,
    WAVE    = 0x57415645,
    FMT     = 0x666D7420,
    DATA    = 0x64617461,


    MY_WAVE_FORMAT_PCM     = 0x0001,
    //IBM_FORMAT_MULAW    = 0x0101,
    IBM_FORMAT_MULAW    = 0x0007,
    IBM_FORMAT_ALAW     = 0x0102,
    IBM_FORMAT_ADPCM    = 0x0103
};

void playWave(WaveInfo *wInfo);
void changeSampleRate(WaveInfo *wInfo, uint32_t new_rate);

void  alaw_expand(long lseg, short *logbuf, short *linbuf);
void  ulaw_expand(long lseg, short *logbuf, short *linbuf);

typedef struct {
	GtkWidget *textview;
	gchar *text;
	char *ip;
	uint16_t mapped_port;
	char *local_ip;
} Widgets;

void insert_text(Widgets *w);
void insert_text(Widgets *w)
{
	GtkTextBuffer *buffer;
	GtkTextMark *mark;
	GtkTextIter iter;
	const gchar *text;
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (w->textview));
	text = w->text;
	mark = gtk_text_buffer_get_insert (buffer);
	gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);
	gtk_text_buffer_insert (buffer, &iter, text, -1);
	gtk_widget_queue_draw (w->textview);
	while (gtk_events_pending ())
		gtk_main_iteration ();
}

void append_textview(GtkWidget *textview, const gchar *text);
void append_textview(GtkWidget *textview, const gchar *text)
{
	GtkTextBuffer *buffer;
	GtkTextMark *mark;
	GtkTextIter iter;
	buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));
	mark = gtk_text_buffer_get_insert (buffer);
	gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);
	gtk_text_buffer_insert (buffer, &iter, text, -1);
	gtk_widget_queue_draw (textview);
	while (gtk_events_pending ())
		gtk_main_iteration ();
}

static Widgets _textArea;
static bool_t _update_widget = FALSE;
DWORD WINAPI textAreaWidgetUpdate(void* userData) {
	while(1) {
		if(_update_widget == TRUE) {
			_update_widget = FALSE;
			if(_textArea.textview != NULL && _textArea.text != NULL)
				insert_text(&_textArea);
				gtk_widget_queue_draw (_textArea.textview);
				while (gtk_events_pending ())
					gtk_main_iteration ();
		}
	}
	return 0;
}

char *return_string(int length, char *str);
char *return_string(int length, char *str) {
	return str;
}
;
/* Function to open a dialog box displaying the message provided. */
void quick_message (gchar *message, GtkWidget *window);
void quick_message (gchar *message, GtkWidget *window)
{
   GtkWidget *dialog, *label, *content_area;

   /* Create the widgets */
   dialog = gtk_dialog_new_with_buttons ("Message",
		   	   	   	   	   	   	   	   	 GTK_WINDOW(window),
                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                         GTK_STOCK_OK,
                                         GTK_RESPONSE_NONE,
                                         NULL);
   content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
   label = gtk_label_new (message);

   /* Ensure that the dialog box is destroyed when the user responds */
   g_signal_connect_swapped (dialog,
                             "response",
                             G_CALLBACK (gtk_widget_destroy),
                             dialog);

   /* Add the label, and show everything we've added to the dialog */

   gtk_container_add (GTK_CONTAINER (content_area), label);
   gtk_widget_show_all (dialog);
}

typedef struct LoginData {
	GtkWidget *login_box;
	GtkWidget *email;
	GtkWidget *password;
	GtkWidget *textview;
	GtkWidget *statusbar;
};

typedef struct RemoteInfo {
	char *local_ip;
	char *ip;
	uint16_t port;
	RtpSession rtpsession;
};

typedef struct UserListData {
	const char *id;
	const char *firstname;
	const char *lastname;
	const char *fullname;
	const char *ip;
	const char *port;
};

typedef struct IODeviceInfo {
	int device_list_index;
	int device_pa_index;
};

typedef struct {
	WaveInfo wInfo;
	queue *que;
	HANDLE *pThreads;
	char *local_ip;
	char *ip;
	unsigned short port;
} DData;

typedef struct AppInfo {
	char *user_id;
	char *email;
	GtkWidget *window;
	GtkWidget *textview;
	GtkWidget *statusbar;
	GtkWidget *user_list;
	GtkWidget *input_device_list;
	GtkWidget *output_device_list;
	IODeviceInfo inputDevices[10];
	IODeviceInfo outputDevices[10];
	RemoteInfo * remoteinfo;
	UserListData *user_data_list;
	UserListData *selected_user;
	DData *ddata;
	size_t users_list_size;
};

#endif
