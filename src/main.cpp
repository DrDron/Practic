#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <iostream>

using namespace std;



void CreatePcmFile(const char* filePath,  float Duration, unsigned short SampleRate, unsigned short Amplitude)
{

    FILE* pcmFile = fopen(filePath, "wb");

    unsigned char c;

    for (double t = 0; t < Duration; t+=1./SampleRate)
    {
        double sample = Amplitude * sin(2 * M_PI * 1000 * t);
        short s16 = (short)sample;
        c = (unsigned)s16 % 256;
        fwrite(&c, 1, 1, pcmFile);
        c = (unsigned)s16/256 % 256;
        fwrite(&c, 1, 1, pcmFile);
    }
    fclose(pcmFile);
}

int main()
{
    CreatePcmFile("test.pcm", 1, 44100, 10000);
    return 0;
}