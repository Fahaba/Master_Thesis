#include <stdio.h>
#include "Writer.h"

int main()
{
    fileConfig cfg;
    cfg.channels = 4;
    cfg.format = DataFormat::WAV;
    cfg.inFile = "C:\\Users\\fahas\\Desktop\\Masterarbeit\\4channelKnownFreqTest.wav";
    cfg.threshhold = 25000;
    
    Writer writer = Writer(cfg);
    return 0;
}