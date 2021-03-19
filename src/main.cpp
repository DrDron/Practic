#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <argparse/argparse.hpp>
#include <math.h>
#include <iostream>
#include <vector>
using namespace std;

class Sinusoid {
private:
    uint32_t sampleRate;
    uint16_t amplitude;
    vector <double> sampleTable;
public:

    void SetSampleRate(uint32_t SR) {
        sampleRate = SR;
    }
    unsigned short GetSampleRate() {
        return sampleRate;
    };
    void SetAmplitude(uint16_t Am) {
        amplitude = Am;
    }
    unsigned short GetAmplitude() {
        return amplitude;
    };
    void SetSampleTable() {
        for (double i = 0; i < sampleRate; i++) {
            double t = i/ sampleRate;
            sampleTable.push_back(short(amplitude * sin(2 * M_PI * 1000 * t)));
        }
    }
    void BuildT(FILE* filename) {
        for (int i = 0; i < sampleRate; i++) {
            uint8_t c;
            c = (unsigned)sampleTable[i] % 256;
            fwrite(&c, 1, 1, filename);
            c = (unsigned)sampleTable[i] / 256 % 256;
            fwrite(&c, 1, 1, filename);
        }

    }

    Sinusoid(uint32_t SR, uint16_t Am)
    {
        SetSampleRate(SR);
        SetAmplitude(Am);
        SetSampleTable();
    };
    
    void BuildSinusoid(int drt, const char* filePath) {
        FILE* pcmFile = fopen(filePath, "wb");
        for (int i = 0; i < drt; i++) {
            BuildT(pcmFile);
        };
        fclose(pcmFile);
    };
    ~Sinusoid() {};


};

int main(int argc, char* argv[])
{
    argparse::ArgumentParser program("sinus_builder");

    program.add_argument("-f","--flnm")
        .help("name of file with sin")
        .default_value(string("test.pcm"));
    program.add_argument("-s","--smplrt")
        .required()
        .help("samplerate of sin")
        .action([](const string& value) { return stoi(value); });
    program.add_argument("-a","--amp")
        .required()
        .help("amplitude of sin")
        .action([](const string& value) { return stoi(value); });
    program.add_argument("-d","--drt")
        .required()
        .help("duration of pcm")
        .action([](const string& value) { return stoi(value); });
    try {
        program.parse_args(argc, argv);   // Example: ./main --flnm test.pcm --smplrt 44100 --amp 15000 --drt 4
    }
    catch (const std::runtime_error& err) {
        cout << err.what() << endl;
        cout << program;
        exit(0);
    }

    auto inputSR = program.get<int>("-s");
    auto inputA = program.get<int>("-a");
    auto inputD = program.get<int>("-d");
    string inputFN = program.get<string>("-f");

    const char* file = inputFN.c_str();

    Sinusoid sin(inputSR, inputA);
    sin.BuildSinusoid(inputD, file);
	
    return 0;
}