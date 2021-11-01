#include "Writer.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Sender.h"
#include <Windows.h>
#include <array>
#include <sstream>
#include <iomanip>

template <typename Word>
void write_word(FILE* outs, Word value, unsigned size = sizeof(Word))
{
	for (; size; --size, value >>= 8)
	{
		char test = static_cast <char> (value & 0xFF);
		fputc(test, outs);
	}
}

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60
void inline printProgress(double percentage) {
	int val = (int)(percentage * 100);
	int lpad = (int)(percentage * PBWIDTH);
	int rpad = PBWIDTH - lpad;
	printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
	fflush(stdout);
}

Writer::Writer(fileConfig cfg)
{	
	std::string file_in = cfg.inFile;
	DataFormat format = cfg.format;
	
	int** channelArr = (int**)malloc(sizeof(int*) * MAXCHANNELS);

	FILE* fp_out;
	switch (format)
	{
		case DataFormat::RAW:
		{
			// header information is not present
			// must pass them to config, to know what we deal with
			int channelNumber = cfg.channels;
			int sizePerSample = cfg.bytesPerSample;
			int threshhold = cfg.threshhold;
			int sampleRate = cfg.sampleRate;

			// todo: implement this

			break;
		}
		case DataFormat::WAV:
		{
			printf("File: %s opened format %u\n", file_in.c_str(), format);
			WAVLoader* wl = new_WAVLoader((char*)file_in.c_str());
			Sender* sender = new Sender();

			int index = 0;
			std::stringstream ss;
			while (true)
			{
				int upper = index + cfg.threshhold;
				ss.str("");
				int bytes = 0;


				for (;index < upper && index < wl->data.SampleNumber ; index++)
				{
					for (int ch = 0; ch < wl->fmt.NumChannels; ch++)
					{
						uint16_t sample = unsigned(wl->data.channelArr[ch][index]);
						ss << std::setfill('0') << std::setw(4) << std::hex << static_cast<uint32_t>(sample);
						if (ch < wl->fmt.NumChannels - 1)
							ss << " ";
					}
					ss << "\n";
				}

				sender->Send(ss.str());

				if (index >= wl->data.SampleNumber)
					break;
			}
			sender->Shutdown();
			delete wl;
			delete sender;
			break;
		}
	}
	free(channelArr);
}

Writer::~Writer()
{
}