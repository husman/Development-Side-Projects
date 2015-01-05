#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/pixfmt.h>

long long getWaitTime(long long start) {
	long long elapsed = (GetTickCount() - start);
	long long wait_time = 1000 - elapsed;

	return wait_time > 0? wait_time : 0;
}

// Define necessary data structures and 
// global variables/constants for this experiment.
#define STREAM_FRAME_RATE 30 /* 30 frames/s */
#define STREAM_PIX_FMT AV_PIX_FMT_YUV420P /* default pix_fmt */

typedef struct StreamInfo {
	int width;
	int height;
	int pix_fmt;
	int frame_rate;
	int bitrate;
} StreamInfo;

int ScreenX = 0;
int ScreenY = 0;
BYTE* ScreenData = 0;

static AVFrame *frame;
static AVPicture src_picture, dst_picture;

/**
 * This function will initialize the necessary video codec.
*/
static void open_video(AVFormatContext *oc, AVCodec *codec, AVStream *st) {
	int ret;
	AVCodecContext *c = st->codec;
	/* open the codec */
	ret = avcodec_open2(c, codec, NULL);
	if (ret < 0) {
		fprintf(stderr, "Could not open video codec: %s\n", av_err2str(ret));
		exit(1);
	}
	/* allocate and init a re-usable frame */
	frame = avcodec_alloc_frame();
	if (!frame) {
		fprintf(stderr, "Could not allocate video frame\n");
		exit(1);
	}
	/* Allocate the encoded raw picture. */
	ret = avpicture_alloc(&dst_picture, c->pix_fmt, c->width, c->height);
	if (ret < 0) {
		fprintf(stderr, "Could not allocate picture: %s\n", av_err2str(ret));
		exit(1);
	}
	/* If the output format is not YUV420P, then a temporary YUV420P
	 * picture is needed too. It is then converted to the required
	 * output format. */
	if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
		ret = avpicture_alloc(&src_picture, AV_PIX_FMT_YUV420P, c->width, c->height);
		if (ret < 0) {
			fprintf(stderr, "Could not allocate temporary picture: %s\n", av_err2str(ret));
			exit(1);
		}
	} //
	/* copy data and linesize picture pointers to frame */
	*((AVPicture *) frame) = dst_picture;
}
static void close_video(AVFormatContext *oc, AVStream *st) {
	avcodec_close(st->codec);
	av_free(src_picture.data[0]);
	av_free(dst_picture.data[0]);
	av_free(frame);
}
/**************************************************************/
/* Add an output stream. */
static AVStream *add_stream(AVFormatContext *oc, AVCodec **codec, enum AVCodecID codec_id, StreamInfo *sInfo) {
	AVCodecContext *c;
	AVStream *st;
	/* find the encoder */
	*codec = avcodec_find_encoder(codec_id);
	if (!(*codec)) {
		fprintf(stderr, "Could not find encoder for '%s'\n", avcodec_get_name(codec_id));
		exit(1);
	}
	st = avformat_new_stream(oc, *codec);
	if (!st) {
		fprintf(stderr, "Could not allocate stream\n");
		exit(1);
	}
	st->id = oc->nb_streams - 1;
	c = st->codec;
	switch ((*codec)->type) {
	case AVMEDIA_TYPE_VIDEO:
		avcodec_get_context_defaults3(c, *codec);
		c->codec_id = codec_id;
		c->bit_rate = sInfo->bitrate;
		
		/* Resolution must be a multiple of two. */
		c->width = sInfo->width;
		c->height = sInfo->height;
		
		/* timebase: This is the fundamental unit of time (in seconds) in terms
		 * of which frame timestamps are represented. For fixed-fps content,
		 * timebase should be 1/framerate and timestamp increments should be
		 * identical to 1. */
		c->time_base.den = sInfo->frame_rate;
		c->time_base.num = 1;
		c->gop_size = 12; /* emit one intra frame every twelve frames at most */
		c->pix_fmt = sInfo->pix_fmt;
		c->rc_min_rate = sInfo->bitrate-50;
		c->rc_max_rate = sInfo->bitrate+50;
		c->rc_buffer_size = sInfo->bitrate+60;

		if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
			/* just for testing, we also add B frames */
			//c->max_b_frames = 2;
		}
		if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
			/* Needed to avoid using macroblocks in which some coeffs overflow.
			 * This does not happen with normal video, it just happens here as
			 * the motion of the chroma plane does not match the luma plane. */
			c->mb_decision = 2;
		}
		break;
	default:
		break;
	}
	/* Some formats want stream headers to be separate. */
	if (oc->oformat->flags & AVFMT_GLOBALHEADER)
		c->flags |= CODEC_FLAG_GLOBAL_HEADER;
	return st;
}

void ScreenCap()
{
    HDC hScreen = GetDC(GetDesktopWindow());
    ScreenX = GetDeviceCaps(hScreen, HORZRES);
    ScreenY = GetDeviceCaps(hScreen, VERTRES);

    HDC hdcMem = CreateCompatibleDC (hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, ScreenX, ScreenY);
    HGDIOBJ hOld = SelectObject(hdcMem, hBitmap);
    BitBlt(hdcMem, 0, 0, ScreenX, ScreenY, hScreen, 0, 0, SRCCOPY);
    SelectObject(hdcMem, hOld);

    BITMAPINFOHEADER bmi = {0};
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biWidth = ScreenX;
    bmi.biHeight = -ScreenY;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = 0;// 3 * ScreenX * ScreenY;
    printf("dim: %dx%d\n",ScreenX,ScreenY);

    if(ScreenData)
        free(ScreenData);
    ScreenData = (BYTE*)malloc(4 * ScreenX * ScreenY);

    GetDIBits(hdcMem, hBitmap, 0, ScreenY, ScreenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

    ReleaseDC(GetDesktopWindow(),hScreen);
    DeleteDC(hdcMem);
}

inline int PosB(int x, int y)
{
    return ScreenData[4*((y*ScreenX)+x)];
}

inline int PosG(int x, int y)
{
    return ScreenData[4*((y*ScreenX)+x)+1];
}

inline int PosR(int x, int y)
{
    return ScreenData[4*((y*ScreenX)+x)+2];
}

int ButtonPress(int Key)
{
    int button_pressed = 0;

    while(GetAsyncKeyState(Key))
        button_pressed = 1;

    return button_pressed;
}

char * getline(void)
{
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}

int main(int argc, char ** argv)
{
	if(argc < 4) {
		printf("\nScrub, you need to specify a bitrate, number of frames, and server."
				"\nLike this: pixieHD 350 1000 rtmp://domain.com/live/matt\n"
				"\nNOTE, it is: progname bitrate frames server\n\n"
				"The bitrate is understood to be kbits/sec.\n"
				"You should enter frames or else you the program will\n"
				"continue to stream until you forcefully close it.\n"
				"THANK YOU: while(1) { /* stream! */ }\n");
		return 0;
	}
	printf("\nYou have set the following options:\n\n%5cbitrate: %s,"
			"\n%5cframes: %s\n%5cserver: %s\n\n",
			' ',argv[1],' ',argv[2],' ',argv[3]);
	
	/*int p;
	printf("Initializing noob options");
	for(p=0; p<3; ++p) {
		printf("%5c",'.');
		Sleep(1500);
	}
	printf("\n\n");

	char *input;
	printf("You hating on my GFX or wat? Please Answer: ");
	input = getline();

	printf("\n\n");
	printf("Your answer: ");

	size_t input_len = strlen(input);
	for(p=0; p<input_len; ++p) {
		Sleep(300);
		printf("%c",input[p]);
	}
	printf("\nkk here we go...");
	Sleep(1000);*/

	printf("\n\nPress the CONTROL key to begin streaming or ESC key to QUIT.\n\n");
    while (1)
    {
	   if (ButtonPress(VK_ESCAPE)) {
          printf("Quit.\n\n");
          break;
       } else if (ButtonPress(VK_CONTROL)) {
    	   // Decoder local variable declaration
    	   	AVFormatContext *pFormatCtx = NULL;
    	   	int i, videoStream;
    	   	AVCodecContext *pCodecCtx = NULL;
    	   	AVCodec *pCodec;
    	   	AVFrame *pFrame;
    	   	AVPacket packet;
    	   	int frameFinished;

    	   	// Encoder local variable declaration
    	   	const char *filename;
    	   	AVOutputFormat *fmt;
    	   	AVFormatContext *oc;
    	   	AVStream *video_st;
    	   	AVCodec *video_codec;
    	   	int ret; unsigned int frame_count, frame_count2;
    	   	StreamInfo sInfo;

    	   	size_t max_frames = strtol(argv[2], NULL, 0);

    	   	// Register all formats, codecs and network
    	   	av_register_all();
    	   	avcodec_register_all();
    	   	avformat_network_init();

    	   	// Setup mux
    	   	//filename = "output_file.flv";
    	   	//filename = "rtmp://chineseforall.org/live/beta";
    	   	filename = argv[3];
    	   	fmt = av_guess_format("flv", filename, NULL);
    	   	if (fmt == NULL) {
    	   		printf("Could not guess format.\n");
    	   		return -1;
    	   	}
    	   	// allocate the output media context
    	   	oc = avformat_alloc_context();
    	   	if (oc == NULL) {
    	   		printf("could not allocate context.\n");
    	   		return -1;
    	   	}


    	   HDC hScreen = GetDC(GetDesktopWindow());
		   ScreenX = GetDeviceCaps(hScreen, HORZRES);
		   ScreenY = GetDeviceCaps(hScreen, VERTRES);

		   // Temp. hard-code the resolution
		   int new_width = 1024, new_height = 576;
		   double v_ratio = 1.7786458333333333333333333333333;

    	   	// Set output format context to the format ffmpeg guessed
    	   	oc->oformat = fmt;

    	   	// Add the video stream using the h.264
    	   	// codec and initialize the codec.
    	   	video_st = NULL;
    	   	sInfo.width = new_width;
    	   	sInfo.height = new_height;
    	   	sInfo.pix_fmt = AV_PIX_FMT_YUV420P;
    	   	sInfo.frame_rate = 10;
    	   	sInfo.bitrate = strtol(argv[1], NULL, 0)*1000;
    	   	video_st = add_stream(oc, &video_codec, AV_CODEC_ID_H264, &sInfo);

    	   	// Now that all the parameters are set, we can open the audio and
    	   	// video codecs and allocate the necessary encode buffers.
    	   	if (video_st)
    	   		open_video(oc, video_codec, video_st);

    	   	/* open the output file, if needed */
    	   	if (!(fmt->flags & AVFMT_NOFILE)) {
    	   		ret = avio_open(&oc->pb, filename, AVIO_FLAG_WRITE);
    	   		if (ret < 0) {
    	   			fprintf(stderr, "Could not open '%s': %s\n", filename, av_err2str(ret));
    	   			return 1;
    	   		}
    	   	}

    	   	// dump output format
    	   	av_dump_format(oc, 0, filename, 1);

    	   	// Write the stream header, if any.
    	   	ret = avformat_write_header(oc, NULL);
    	   	if (ret < 0) {
    	   		fprintf(stderr, "Error occurred when opening output file: %s\n", av_err2str(ret));
    	   		return 1;
    	   	}

    	   	// Read frames, decode, and re-encode
    	   	frame_count = 1;
    	   	frame_count2 = 1;

		   HDC hdcMem = CreateCompatibleDC (hScreen);
		   HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, ScreenX, ScreenY);
		   HGDIOBJ hOld;
		   BITMAPINFOHEADER bmi = {0};
		   bmi.biSize = sizeof(BITMAPINFOHEADER);
		   bmi.biPlanes = 1;
		   bmi.biBitCount = 32;
		   bmi.biWidth = ScreenX;
		   bmi.biHeight = -ScreenY;
		   bmi.biCompression = BI_RGB;
		   bmi.biSizeImage = 0;// 3 * ScreenX * ScreenY;


		   if(ScreenData)
			   free(ScreenData);
		   ScreenData = (BYTE*)malloc(4 * ScreenX * ScreenY);
		   AVPacket pkt;

		   clock_t start_t = GetTickCount();
		   long long wait_time = 0;

		   uint64_t total_size;

    	   while(1) {
			hOld = SelectObject(hdcMem, hBitmap);
			BitBlt(hdcMem, 0, 0, ScreenX, ScreenY, hScreen, 0, 0, SRCCOPY);
			SelectObject(hdcMem, hOld);

			GetDIBits(hdcMem, hBitmap, 0, ScreenY, ScreenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

			//calculate the bytes needed for the output image
			int nbytes = avpicture_get_size(AV_PIX_FMT_YUV420P, new_width, new_height);

			//create buffer for the output image
			uint8_t* outbuffer = (uint8_t*)av_malloc(nbytes);

			//create ffmpeg frame structures.  These do not allocate space for image data,
			//just the pointers and other information about the image.
			AVFrame* inpic = avcodec_alloc_frame();
			AVFrame* outpic = avcodec_alloc_frame();

			//this will set the pointers in the frame structures to the right points in
			//the input and output buffers.
			avpicture_fill((AVPicture*)inpic, ScreenData, AV_PIX_FMT_RGB32, ScreenX, ScreenY);
			avpicture_fill((AVPicture*)outpic, outbuffer, AV_PIX_FMT_YUV420P, new_width, new_height);

			//create the conversion context
			struct SwsContext *fooContext = sws_getContext(ScreenX, ScreenY, AV_PIX_FMT_RGB32, new_width, new_height, AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL);

			//perform the conversion
			sws_scale(fooContext, inpic->data, inpic->linesize, 0, ScreenY, outpic->data, outpic->linesize);
			
			// Initialize a new frame
			AVFrame* newFrame = avcodec_alloc_frame();

			int size = avpicture_get_size(video_st->codec->pix_fmt, video_st->codec->width, video_st->codec->height);
			uint8_t* picture_buf = av_malloc(size);

			avpicture_fill((AVPicture *) newFrame, picture_buf, video_st->codec->pix_fmt, video_st->codec->width, video_st->codec->height);

			// Copy only the frame content without additional fields
			av_picture_copy((AVPicture*) newFrame, (AVPicture*) outpic, video_st->codec->pix_fmt, video_st->codec->width, video_st->codec->height);

			// encode the image
			int got_output;
			av_init_packet(&pkt);
			pkt.data = NULL; // packet data will be allocated by the encoder
			pkt.size = 0;

			// Set the frame's pts (this prevents the warning notice 'non-strictly-monotonic PTS')
			newFrame->pts = frame_count2;

			ret = avcodec_encode_video2(video_st->codec, &pkt, newFrame, &got_output);
			if (ret < 0) {
				fprintf(stderr, "Error encoding video frame: %s\n", av_err2str(ret));
				exit(1);
			}

			if (got_output) {
				if (video_st->codec->coded_frame->key_frame)
					pkt.flags |= AV_PKT_FLAG_KEY;
				pkt.stream_index = video_st->index;

				if (pkt.pts != AV_NOPTS_VALUE)
					pkt.pts = av_rescale_q(pkt.pts, video_st->codec->time_base, video_st->time_base);
				if (pkt.dts != AV_NOPTS_VALUE)
					pkt.dts = av_rescale_q(pkt.dts, video_st->codec->time_base, video_st->time_base);

				// Write the compressed frame to the media file.
				ret = av_interleaved_write_frame(oc, &pkt);

				fprintf(stderr, "encoded frame #%d\n", frame_count);
				frame_count++;
			} else {
				ret = 0;
			}
			if (ret != 0) {
				fprintf(stderr, "Error while writing video frame: %s\n", av_err2str(ret));
				exit(1);
			}

			++frame_count2;

			// Free the YUV picture frame we copied from the
			// decoder to eliminate the additional fields
			// and other packets/frames used
			av_free(picture_buf);
			av_free_packet(&pkt);
			av_free(newFrame);

			//free memory
			av_free(outbuffer);
			av_free(inpic);
			av_free(outpic);


          if(frame_count == max_frames) {
			/* Write the trailer, if any. The trailer must be written before you
			 * close the CodecContexts open when you wrote the header; otherwise
			 * av_write_trailer() may try to use memory that was freed on
			 * av_codec_close().
			 */
			av_write_trailer(oc);

			/* Close the video codec (encoder) */
			if (video_st) {
				close_video(oc, video_st);
			}
			// Free the output streams.
			for (i = 0; i < oc->nb_streams; i++) {
				av_freep(&oc->streams[i]->codec);
				av_freep(&oc->streams[i]);
			}
			if (!(fmt->flags & AVFMT_NOFILE)) {
				/* Close the output file. */
				avio_close(oc->pb);
			}
			/* free the output format context */
			av_free(oc);

			ReleaseDC(GetDesktopWindow(),hScreen);
			DeleteDC(hdcMem);

			printf("\n\nPress the CONTROL key to begin streaming or ESC key to QUIT.\n\n");
			break;
          }
       }
       }
    }
    return 0;
}
