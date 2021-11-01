
#include "WAV.h"

static int const IN_BITS = 8;

WAVLoader* new_WAVLoader(char* file)
{
    return _WAVLoader(file);
}

WAVLoader* _WAVLoader(char* file)
{
    WAVLoader* self = (WAVLoader*)calloc(1, sizeof(struct WAV_STRUCT));
    self->functions = (WAVLoader_functions*)calloc(1, sizeof(struct WAVLoader_functions_));
    self->functions->print = WAVLoader_print;
    self->functions->convertEndianWithOffset = WAVLoader_convertEndianWithOffset;
    self->functions->convertEndian = WAVLoader_convertEndian;

    char buffer4[4];
    char buffer2[2];

    // read file in ->binary<- mode !!important!!
    fopen_s(&self->fp, file, "rb");
    if (!self->fp) {
        printf("ERROR while opening file\n");
        exit(1);
    }
    
    int read = 0;

    /***********
     ** RIFF  **
     ***********/

    RIFF_t riff;
    read = fread(riff.ChunkID, sizeof(riff.ChunkID), 1, self->fp);
    read = fread(buffer4, sizeof(buffer4), 1, self->fp);
    riff.ChunkSize = self->functions->convertEndian(buffer4);
    read = fread(riff.Format, sizeof(riff.Format), 1, self->fp);

    self->riff = riff;

    /***********
     **  FMT  **
     ***********/

    FMT_t fmt;
    read = fread(fmt.Subchunk1ID, sizeof(fmt.Subchunk1ID), 1, self->fp);
    read = fread(buffer4, sizeof(buffer4), 1, self->fp);
    fmt.Subchunk1Size = self->functions->convertEndian(buffer4);
    read = fread(buffer2, sizeof(buffer2), 1, self->fp);
    fmt.AudioFormat = self->functions->convertEndian(buffer2);
    read = fread(buffer2, sizeof(buffer2), 1, self->fp);
    fmt.NumChannels = self->functions->convertEndian(buffer2);
    read = fread(buffer4, sizeof(buffer4), 1, self->fp);
    fmt.SampleRate = self->functions->convertEndian(buffer4);
    read = fread(buffer4, sizeof(buffer4), 1, self->fp);
    fmt.ByteRate = self->functions->convertEndian(buffer4);
    read = fread(buffer2, sizeof(buffer2), 1, self->fp);
    fmt.BlockAlign = self->functions->convertEndian(buffer2);
    read = fread(buffer2, sizeof(buffer2), 1, self->fp);
    fmt.BitsPerSample = self->functions->convertEndian(buffer2);
    
    self->fmt = fmt;

    /***********
     ** DATA  **
     ***********/

    Data_t data;
    read = fread(data.Subchunk2ID, sizeof(data.Subchunk2ID), 1, self->fp);
    read = fread(buffer4, sizeof(buffer4), 1, self->fp);
    data.Subchunk2Size = self->functions->convertEndian(buffer4);
    
    long sample_num = (data.Subchunk2Size * IN_BITS) / (self->fmt.NumChannels * self->fmt.BitsPerSample);
    long sizePerSample = (self->fmt.NumChannels * self->fmt.BitsPerSample) / IN_BITS;
    long bytesPerChannel = sizePerSample / self->fmt.NumChannels;
    data.SampleNumber = sample_num;
    data.SizePerSample = sizePerSample;
    data.channelArr = (int**)malloc(sizeof(int*) * self->fmt.NumChannels);
    self->data = data;
   
    // allocate memory for data sample and channel array
    char* data_buffer = (char*) malloc(sizeof(char) * self->data.SizePerSample);
    int **channelArr = (int**) malloc(sizeof(int*) * self->fmt.NumChannels);
    // allocate memory for all samples for each channel
    for (int i = 0; i < self->fmt.NumChannels; i++)
        channelArr[i] = (int *)malloc(sizeof(int) * (self->data.SampleNumber) + 1);

    // PCM-format specific
    if (self->fmt.AudioFormat == 1)
    {
        for (long i = 1; i <= self->data.SampleNumber; i++)
        {
            read = fread(data_buffer, self->data.SizePerSample, 1, self->fp);
            if (read)
            {
                int channels = 0;
                int channelData = 0;
                int offset = 0;
                
                for (; channels < self->fmt.NumChannels; channels++)
                {   
                    channelData = self->functions->convertEndianWithOffset(data_buffer,offset,bytesPerChannel);
                    channelArr[channels][i-1] = channelData;
                    offset += bytesPerChannel;
                }
            }
            else
            {
                // redo iteration - should not normally happen
                printf("ERROR READING FILE");
                i--;
            }
        }
    }
    self->data.channelArr = channelArr;
    return self;
}

void WAVLoader_print(WAVLoader* self)
{
    // print RIFF chunk
    printf("ChunkID \t%c%c%c%c\n", self->riff.ChunkID[0], self->riff.ChunkID[1], self->riff.ChunkID[2], self->riff.ChunkID[3]);
    printf("ChunkSize \t%d\n", self->riff.ChunkSize);
    printf("Format \t\t%c%c%c%c\n", self->riff.Format[0], self->riff.Format[1], self->riff.Format[2], self->riff.Format[3]);
    printf("\n");

    // print FMT chunk
    printf("Subchunk1ID \t%c%c%c%c\n", self->fmt.Subchunk1ID[0], self->fmt.Subchunk1ID[1], self->fmt.Subchunk1ID[2], self->fmt.Subchunk1ID[3]);
    printf("Subchunk1Size \t%d\n", self->fmt.Subchunk1Size);
    printf("AudioFormat \t%d\n", self->fmt.AudioFormat);
    printf("NumChannels \t%d\n", self->fmt.NumChannels);
    printf("SampleRate \t%d\n", self->fmt.SampleRate);
    printf("ByteRate \t%d\n", self->fmt.ByteRate);
    printf("BlockAlign \t%d\n", self->fmt.BlockAlign);
    printf("BitsPerSample \t%d\n", self->fmt.BitsPerSample);
    printf("\n");

    // print DATA chunk
    printf("blockID \t%c%c%c%c\n", self->data.Subchunk2ID[0], self->data.Subchunk2ID[1], self->data.Subchunk2ID[2], self->data.Subchunk2ID[3]);
    printf("blockSize \t%d\n", self->data.Subchunk2Size);
    printf("\n");
    printf("duration \t%d\n", self->data.Subchunk2Size / self->fmt.ByteRate);
    printf("\n");
}

uint32_t WAVLoader_convertEndian(unsigned char* buffer)
{  
    // this translates the buffer endian format into systems endian format
    uint32_t num;
    memcpy(&num, buffer, 4);
    return num;
}

int WAVLoader_convertEndianWithOffset(char* buffer, int offset, int bytesPerChannel)
{
    int shift = 8;
    int ret = buffer[offset] & 0xFF;
    for (int i = 1; i < bytesPerChannel; ++i)
    {
        ret |= (buffer[offset + i] & i < (bytesPerChannel - 1) ? 0xFF : buffer[offset + i]) << shift;
        shift += 8;
    }
    return ret;
}