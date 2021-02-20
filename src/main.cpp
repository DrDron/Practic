#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <tchar.h>
#include <conio.h>
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;

struct WAVHEADER
{
    char chunkId[4]; // RIFF Header Magic header

    unsigned long chunkSize; // RIFF Chunk Size

    char format[4]; // WAVE Header

    char subchunk1Id[4]; // FMT header

    unsigned long subchunk1Size; // Size of the fmt chunk

    unsigned short audioFormat; // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM

    unsigned short numChannels; // Number of channels 1=Mono 2=Sterio

    unsigned long sampleRate; // Sampling Frequency in Hz

    unsigned long byteRate; // bytes per second

    unsigned short blockAlign; // 2=16-bit mono, 4=16-bit stereo

    unsigned short bitsPerSample; // Number of bits per sample

    char subchunk2Id[4]; // "data"  string

    unsigned long subchunk2Size; // Sampled data length

}wav_hdr;

void CreateWavFile(const char* filePath, const double* Amplitude, int VectorSize)
{
    unsigned long Data;
    WAVHEADER wavHeader;
    wavHeader.chunkId[0] = 'R'; wavHeader.chunkId[1] = 'I'; wavHeader.chunkId[2] = 'F'; wavHeader.chunkId[3] = 'F';
    wavHeader.format[0] = 'W'; wavHeader.format[1] = 'A'; wavHeader.format[2] = 'V'; wavHeader.format[3] = 'E';
    wavHeader.subchunk1Id[0] = 'f';  wavHeader.subchunk1Id[1] = 'm';  wavHeader.subchunk1Id[2] = 't';  wavHeader.subchunk1Id[3] = ' ';
    wavHeader.subchunk2Id[0] = 'd'; wavHeader.subchunk2Id[1] = 'a'; wavHeader.subchunk2Id[2] = 't'; wavHeader.subchunk2Id[3] = 'a';
    wavHeader.subchunk1Size = 16;
    wavHeader.audioFormat = 1;
    wavHeader.numChannels = 1;
    wavHeader.sampleRate = 44100;
    wavHeader.byteRate = wavHeader.sampleRate * wavHeader.numChannels * wavHeader.bitsPerSample / 8;
    wavHeader.bitsPerSample = 16;
    wavHeader.blockAlign = wavHeader.numChannels * wavHeader.bitsPerSample / 8;
    wavHeader.subchunk2Size = 2 * VectorSize * wavHeader.blockAlign;

    size_t headerSize = sizeof(wav_hdr);

    FILE* wavFile = fopen(filePath, "wb");
    fwrite(&wavHeader, 1, headerSize, wavFile);

    for (int i = 0; i < VectorSize; i++)
    {
        Data = int32_t(Amplitude[i]);
        fwrite(&Data, 2, 1, wavFile);
    }
    fclose(wavFile);
}

int main()
{
    double data[32000];
    for (int i = 0; i < 32000; i++)
        data[i] = 10000.0 * sin(2 * M_PI * i / 32);

    CreateWavFile("test.pcm", data, 32000);

    system("pause");

    return 0;
}