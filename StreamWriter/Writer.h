#pragma once

//#include <stdlib.h>
#include <string>

extern "C" {
#include "WAV.h"
}
constexpr auto MAXCHANNELS = 8;

enum class DataFormat
{
    RAW,
    WAV,
};

struct fileConfig
{
    DataFormat format;
    int bytesPerSample;
    int sampleRate;
    std::string inFile;
    int channels;
    int threshhold;
};

class Writer
{
    public:
        Writer(fileConfig cfg);
        ~Writer();
        void readFile(DataFormat format, std::string file_in);
        void writeToFile(std::string file_out);
        
        
};