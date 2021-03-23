#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include <argparse/argparse.hpp>
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

class Sinusoid
{
private:
    uint32_t m_sampleRate;
    uint16_t m_amp;
    int16_t m_freq;
    int16_t m_phase;                ///< текущая фаза сигнала
    vector<double> m_sinTable;      ///<  таблица синуса 1Гц для быстрой генерации

    //генерация таблицы синуса (1Гц для данной частоты дискретизации)
    void GenerateSinTable() {
        for (int i = 0; i < m_sampleRate; i++) {
            double t = i / double(m_sampleRate);
            double value = sin(2 * M_PI * t);
            m_sinTable.push_back(value);
        }
    }

public:
    Sinusoid(uint32_t sampleRate=8000, uint16_t amp=1000, int16_t freq=440) {
        m_sampleRate = sampleRate;
        m_phase = 0;
        SetAmplitude(amp);
        SetFreq(freq);
        GenerateSinTable();
    };

    ~Sinusoid(){};

    void SetAmplitude(uint16_t amp) {
        m_amp = amp;
    }

    void SetFreq(int16_t freq) {
        if (freq > m_sampleRate/2) {
            throw std::runtime_error("Freq value error");
        }
        
        m_phase = 0;
        m_freq = freq;
    }

    //генерация фрейма длиной nx отсчетов
    std::vector<int16_t> Generate(int nx) {
        std::vector<int16_t> r(nx);
        for (size_t i = 0; i < nx; i++) {
            r[i] = round(m_amp * m_sinTable[m_phase]);
            m_phase = (m_phase + m_freq) % m_sampleRate;
        }
        return r;
    }
};

//дописать сигнал в файл
static void AddToFile(const std::vector<int16_t>& frame, FILE* file) {
    for (size_t i = 0; i < frame.size(); i++) {
        int16_t value = frame[i];
        uint8_t c = value & 0xFF;
        fwrite(&c, 1, 1, file);
        c = (value >> 8) & 0xFF;
        fwrite(&c, 1, 1, file);
    }
}

int main(int argc, char *argv[])
{
    argparse::ArgumentParser parser("sinus_builder");

    //TODO: использовать более понятные сокращения
    parser.add_argument("-f", "--flnm")
        .help("name of file with sin")
        .default_value(string("test.pcm"));
    parser.add_argument("-s", "--smplrt")
        .required()
        .help("samplerate of sin")
        .action([](const string &value) { return stoi(value); });
    parser.add_argument("-q", "--freq")
        .required()
        .help("frequency of sin")
        .action([](const string &value) { return stoi(value); });
    parser.add_argument("-a", "--amp")
        .required()
        .help("amplitude of sin")
        .action([](const string &value) { return stoi(value); });
    parser.add_argument("-d", "--drt")
        .required()
        .help("duration of pcm")
        .action([](const string &value) { return stoi(value); });

    try {
        parser.parse_args(argc, argv); // Example: ./main --flnm test.pcm --smplrt 44100 --amp 15000 --drt 4
    }
    catch (const std::runtime_error &err) {
        cout << err.what() << endl;
        cout << parser;
        exit(0);
    }

    auto sampleRate = parser.get<int>("-s");
    auto amplutude = parser.get<int>("-a");
    auto duration = parser.get<int>("-d");
    auto freq = parser.get<int>("-q");
    string fname = parser.get<string>("-f");

    Sinusoid sin(sampleRate, amplutude, freq);
    FILE* pcmFile = fopen(fname.c_str(), "wb");

    //генерация duration сек (без разрыва фазы)
    for (size_t i = 0; i < duration; i++) {
        auto frame = sin.Generate(sampleRate);
        AddToFile(frame, pcmFile);
    }

    fclose(pcmFile);
    
    return 0;
}