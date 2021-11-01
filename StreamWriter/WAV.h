#ifndef __WAV_H_
#define __WAV_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

    typedef struct WAV_RIFF {
        /* chunk "riff" */
        char ChunkID[4];   /* "RIFF" */
        /* sub-chunk-size */
        uint32_t ChunkSize; /* 36 + Subchunk2Size */
        /* sub-chunk-data */
        char Format[4];    /* "WAVE" */
    } RIFF_t;

    typedef struct WAV_FMT {
        /* sub-chunk "fmt" */
        char Subchunk1ID[4];   /* "fmt " */
        /* sub-chunk-size */
        uint32_t Subchunk1Size; /* 16 for PCM */
        /* sub-chunk-data */
        uint16_t AudioFormat;   /* PCM = 1*/
        uint16_t NumChannels;   /* Mono = 1, Stereo = 2, etc. */
        uint32_t SampleRate;    /* 8000, 44100, etc. */
        uint32_t ByteRate;  /* = SampleRate * NumChannels * BitsPerSample/8 */
        uint16_t BlockAlign;    /* = NumChannels * BitsPerSample/8 */
        uint16_t BitsPerSample; /* 8bits, 16bits, etc. 16-bit samples are stored as 2's-complement signed integers, ranging from -32768 to 32767.*/ 
    } FMT_t;

    typedef struct WAV_data {
        /* sub-chunk "data" */
        char Subchunk2ID[4];   /* "data" */
        /* sub-chunk-size */
        uint32_t Subchunk2Size; /* data size */
        /* sub-chunk-data */
        long SampleNumber;
        long SizePerSample;
        int** channelArr;
    //    Data_block_t block;
    } Data_t;

    typedef struct WAV_fotmat {
        RIFF_t riff;
        FMT_t fmt;
        Data_t data;
    } Wav;

    typedef struct WAVLoader_functions_ WAVLoader_functions;
    __declspec(dllimport) typedef struct WAV_STRUCT
    {
        FILE *fp;

        Wav wav;
        RIFF_t riff;
        FMT_t fmt;
        Data_t data;
        WAVLoader_functions* functions;

    } WAVLoader;

    struct WAVLoader_functions_
    {
        void (*print)(WAVLoader*);
        uint32_t(*convertEndian)(unsigned char*);
        uint32_t(*convertEndianWithOffset)(unsigned char*, int, int);
    };

    WAVLoader* new_WAVLoader(char* file);
    WAVLoader* _WAVLoader(char* file);
    void WAVLoader_print(WAVLoader* loader);
    uint32_t WAVLoader_convertEndian(unsigned char*);
    uint32_t WAVLoader_convertEndianWithOffset(unsigned char*, int offset, int bytesPerChannel);

#endif