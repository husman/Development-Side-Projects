#include "globals.h"
#include <SDL/SDL_mixer.h>

void playWave(WaveInfo *wInfo)
{
    SDL_Surface *screen, *image, *image_loading, *temp;
    Mix_Chunk sound;		//Pointer to our sound, in memory
    int channel;				//Channel on which our sound is played
    Uint16 audio_format; //Format of the audio we're playing
    int quit;
    SDL_Event event;

    int audio_rate_orignal = wInfo->dwSamplesPerSec; // Store original freq. for later
    int audio_rate = wInfo->dwSamplesPerSec; //Frequency of audio playback

    if(wInfo->wBitsPerSample == 8)
        audio_format = AUDIO_S8;
    else
        audio_format = AUDIO_S16SYS;

    int audio_channels = wInfo->wChannels;			//2 channels = stereo
    int audio_buffers = wInfo->data_chunk_size;		//Size of the audio buffers in memory

    //Initialize BOTH SDL video and SDL audio
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return;
    }

    //Initialize SDL_mixer with our chosen audio settings
    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
        printf("Unable to initialize audio: %s\n", Mix_GetError());
        exit(1);
    }

    sound.abuf = (uint8_t *)wInfo->data;
    sound.alen = wInfo->data_chunk_size;
    sound.allocated = 1;
    sound.volume = 128;

    //Set the video mode to anything, just need a window

    screen = SDL_SetVideoMode(531, 431, 16, SDL_DOUBLEBUF | SDL_ANYFORMAT);
    if (screen == NULL) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        return;
    }

    temp = SDL_LoadBMP("Einstein.bmp");
    if (temp == NULL) {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return;
    }
    image = SDL_DisplayFormat(temp);

    temp = SDL_LoadBMP("Loading.bmp");
    if (temp == NULL) {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return;
    }
    image_loading = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    SDL_Rect src, dest;

    src.x = 0;
    src.y = 0;
    src.w = image->w;
    src.h = image->h;

    dest.x = 0;
    dest.y = 0;
    dest.w = image->w;
    dest.h = image->h;

    SDL_BlitSurface(image, &src, screen, &dest);
    SDL_Flip(screen);

    //Play our sound file, and capture the channel on which it is played
    channel = Mix_PlayChannel(-1, &sound, 0);
    if(channel == -1) {
        printf("Unable to play WAV file: %s\n", Mix_GetError());
    }
    printf("Starting audio playback. See the pop-up window...\n");

    /* Enable Unicode translation */
    SDL_EnableUNICODE( 1 );
    quit = 0;
    /* Loop until an SDL_QUIT event is found */
    while( !quit ){

        /* Poll for events */
        while(SDL_PollEvent(&event)){

            switch(event.type){
                /* Keyboard event */
                /* Pass the event data onto PrintKeyInfo() */
            case SDL_KEYUP:
                switch(event.key.keysym.sym) {
                case SDLK_ESCAPE: // Escape
                    SDL_BlitSurface(image_loading, &src, screen, &dest);
                    SDL_Flip(screen);
                    printf("[Command - Esc] Stopping audio playback and exiting...\n");
                    quit = 1;
                    break;
                case SDLK_r: // Restart
                    SDL_BlitSurface(image_loading, &src, screen, &dest);
                    SDL_Flip(screen);
                    printf("[Command - R] Restarting audio playback...\n");
                    channel = Mix_PlayChannel(channel, &sound, 0);
                    SDL_Delay(2000);
                    SDL_BlitSurface(image, &src, screen, &dest);
                    SDL_Flip(screen);
                    break;
                case SDLK_s: // Stop
                    SDL_BlitSurface(image_loading, &src, screen, &dest);
                    SDL_Flip(screen);
                    printf("[Command - S] Pauing audio playback...\n");
                    Mix_Pause(channel);
                    SDL_Delay(2000);
                    SDL_BlitSurface(image, &src, screen, &dest);
                    SDL_Flip(screen);
                    break;
                case SDLK_p: // Play
                    SDL_BlitSurface(image_loading, &src, screen, &dest);
                    SDL_Flip(screen);
                    if(Mix_Playing(channel) != 0) {
                        printf("[Command - P] Resuming audio playback from previous point...\n");
                        Mix_Resume(channel);
                    } else {
                        printf("[Command - P] Resuming audio playback from beginning...\n");
                        channel = Mix_PlayChannel(channel, &sound, 0);
                    }
                    SDL_Delay(2000);
                    SDL_BlitSurface(image, &src, screen, &dest);
                    SDL_Flip(screen);
                    break;
                case SDLK_1: // Original
                    SDL_BlitSurface(image_loading, &src, screen, &dest);
                    SDL_Flip(screen);
                    Mix_CloseAudio();
                    changeSampleRate(wInfo, audio_rate_orignal);

                    //Let's play back at new rate
                    audio_rate = wInfo->dwSamplesPerSec;
                    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                        printf("Unable to re-initialize audio: %s\n", Mix_GetError());
                        exit(1);
                    }

                    channel = Mix_PlayChannel(channel, &sound, 0);
                    SDL_Delay(3000);
                    SDL_BlitSurface(image, &src, screen, &dest);
                    SDL_Flip(screen);
                    printf("[Command - 1] Sample rate changed to %u Hz (Original)...\n", wInfo->dwSamplesPerSec);
                    break;
                case SDLK_2: // 8kHz
                    SDL_BlitSurface(image_loading, &src, screen, &dest);
                    SDL_Flip(screen);
                    Mix_CloseAudio();
                    changeSampleRate(wInfo, 8000);

                    //Let's play back at new rate
                    audio_rate = wInfo->dwSamplesPerSec;
                    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                        printf("Unable to re-initialize audio: %s\n", Mix_GetError());
                        exit(1);
                    }

                    channel = Mix_PlayChannel(channel, &sound, 0);
                    SDL_Delay(3000);
                    SDL_BlitSurface(image, &src, screen, &dest);
                    SDL_Flip(screen);
                    printf("[Command - 2] Sample rate changed to %u Hz...\n", wInfo->dwSamplesPerSec);
                    break;
                case SDLK_3: // 11kHz
                    SDL_BlitSurface(image_loading, &src, screen, &dest);
                    SDL_Flip(screen);
                    Mix_CloseAudio();
                    changeSampleRate(wInfo, 16000);

                    //Let's play back at new rate
                    audio_rate = wInfo->dwSamplesPerSec;
                    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                        printf("Unable to re-initialize audio: %s\n", Mix_GetError());
                        exit(1);
                    }

                    channel = Mix_PlayChannel(channel, &sound, 0);
                    SDL_Delay(3000);
                    SDL_BlitSurface(image, &src, screen, &dest);
                    SDL_Flip(screen);
                    printf("[Command - 3] Sample rate changed to %u Hz...\n", wInfo->dwSamplesPerSec);
                    break;
                case SDLK_4: // 11kHz
                    SDL_BlitSurface(image_loading, &src, screen, &dest);
                    SDL_Flip(screen);
                    Mix_CloseAudio();
                    changeSampleRate(wInfo, 22050);

                    //Let's play back at new rate
                    audio_rate = wInfo->dwSamplesPerSec;
                    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                        printf("Unable to re-initialize audio: %s\n", Mix_GetError());
                        exit(1);
                    }

                    channel = Mix_PlayChannel(channel, &sound, 0);
                    SDL_Delay(3000);
                    SDL_BlitSurface(image, &src, screen, &dest);
                    SDL_Flip(screen);
                    printf("[Command - 4] Sample rate changed to %u Hz...\n", wInfo->dwSamplesPerSec);
                    break;
                case SDLK_5: // 32kHz
                    SDL_BlitSurface(image_loading, &src, screen, &dest);
                    SDL_Flip(screen);
                    Mix_CloseAudio();
                    changeSampleRate(wInfo, 32000);

                    //Let's play back at new rate
                    audio_rate = wInfo->dwSamplesPerSec;
                    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                        printf("Unable to re-initialize audio: %s\n", Mix_GetError());
                        exit(1);
                    }

                    channel = Mix_PlayChannel(channel, &sound, 0);
                    SDL_Delay(3000);
                    SDL_BlitSurface(image, &src, screen, &dest);
                    SDL_Flip(screen);
                    printf("[Command - 5] Sample rate changed to %u Hz...\n", wInfo->dwSamplesPerSec);
                    break;
                case SDLK_6: // 44kHz
                    SDL_BlitSurface(image_loading, &src, screen, &dest);
                    SDL_Flip(screen);
                    Mix_CloseAudio();
                    changeSampleRate(wInfo, 44056);

                    //Let's play back at new rate
                    audio_rate = wInfo->dwSamplesPerSec;
                    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                        printf("Unable to re-initialize audio: %s\n", Mix_GetError());
                        exit(1);
                    }

                    channel = Mix_PlayChannel(channel, &sound, 0);
                    SDL_Delay(3000);
                    SDL_BlitSurface(image, &src, screen, &dest);
                    SDL_Flip(screen);
                    printf("[Command - 6] Sample rate changed to %u Hz...\n", wInfo->dwSamplesPerSec);
                    break;
                case SDLK_7: // 88kHz
                    SDL_BlitSurface(image_loading, &src, screen, &dest);
                    SDL_Flip(screen);
                    Mix_CloseAudio();
                    changeSampleRate(wInfo, 88200);

                    //Let's play back at new rate
                    audio_rate = wInfo->dwSamplesPerSec;
                    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                        printf("Unable to re-initialize audio: %s\n", Mix_GetError());
                        exit(1);
                    }

                    channel = Mix_PlayChannel(channel, &sound, 0);
                    SDL_Delay(3000);
                    SDL_BlitSurface(image, &src, screen, &dest);
                    SDL_Flip(screen);
                    printf("[Command - 7] Sample rate changed to %u Hz...\n", wInfo->dwSamplesPerSec);
                    break;
                case SDLK_8: // 96kHz
                    SDL_BlitSurface(image_loading, &src, screen, &dest);
                    SDL_Flip(screen);
                    Mix_CloseAudio();
                    changeSampleRate(wInfo, 96000);

                    //Let's play back at new rate
                    audio_rate = wInfo->dwSamplesPerSec;
                    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                        printf("Unable to re-initialize audio: %s\n", Mix_GetError());
                        exit(1);
                    }

                    channel = Mix_PlayChannel(channel, &sound, 0);
                    SDL_Delay(3000);
                    SDL_BlitSurface(image, &src, screen, &dest);
                    SDL_Flip(screen);
                    printf("[Command - 8] Sample rate changed to %u Hz...\n", wInfo->dwSamplesPerSec);
                    break;
                case SDLK_9: // 176kHz
                    SDL_BlitSurface(image_loading, &src, screen, &dest);
                    SDL_Flip(screen);
                    Mix_CloseAudio();
                    changeSampleRate(wInfo, 176400);

                    //Let's play back at new rate
                    audio_rate = wInfo->dwSamplesPerSec;
                    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                        printf("Unable to re-initialize audio: %s\n", Mix_GetError());
                        exit(1);
                    }

                    channel = Mix_PlayChannel(channel, &sound, 0);
                    SDL_Delay(3000);
                    SDL_BlitSurface(image, &src, screen, &dest);
                    SDL_Flip(screen);
                    printf("[Command - 9] Sample rate changed to %u Hz...\n", wInfo->dwSamplesPerSec);
                    break;
                case SDLK_0: // 192kHz
                    SDL_BlitSurface(image_loading, &src, screen, &dest);
                    SDL_Flip(screen);
                    Mix_CloseAudio();
                    changeSampleRate(wInfo, 192000);

                    //Let's play back at new rate
                    audio_rate = wInfo->dwSamplesPerSec;
                    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
                        printf("Unable to re-initialize audio: %s\n", Mix_GetError());
                        exit(1);
                    }

                    channel = Mix_PlayChannel(channel, &sound, 0);
                    SDL_Delay(3000);
                    SDL_BlitSurface(image, &src, screen, &dest);
                    SDL_Flip(screen);
                    printf("[Command - 0] Sample rate changed to %u Hz...\n", wInfo->dwSamplesPerSec);
                    break;
                }
                break;
            /* SDL_QUIT event (window close) */
            case SDL_QUIT:
                SDL_BlitSurface(image_loading, &src, screen, &dest);
                SDL_Flip(screen);
                printf("[Command - quit] Stopping audio playback and exiting...\n");
                quit = 1;
                break;
            default:
                break;
            }
        }
    }

    SDL_FreeSurface(image);
    //Need to make sure that SDL_mixer and SDL have a chance to clean up
    Mix_CloseAudio();
    SDL_Quit();

    printf("\naudio playback complete\n\n");

    //Return success!
    return;
}