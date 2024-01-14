#ifndef CFILEWAV_H
#define CFILEWAV_H


#include <QFile>
#include <QString>

#include <map>
#include  <utility>

#include <QDebug>
#include <cstdint>

//#include "/home/alexandr/WRK/BAUER/wavreader/wav.h"

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
const int RIFF_ID = 0x46464952;   // 'RIFF'
const int WAVE_ID = 0x45564157;   // 'WAVE'
const int fmt__ID = 0x20746d66;   // 'fmt '
const int data_ID = 0x61746164;   // 'data'
#else
#error
#endif






template <size_t n>
const char * make_cstring(char arr[])
{
    static char cstr[n+1];
    strncpy(cstr,arr,n);
    return cstr;
}



inline const char * i2a4AF(uint16_t  audioFormat)
{
    std::map<uint16_t,const char*> m
    {
        {1,"PCM"}
    };
    auto search = m.find(audioFormat);
    return (search != m.end()) ? search->second : "";
}

const int AF_PCM = 1;



class CFileRiff
{
public:
    CFileRiff() = delete;
    CFileRiff(QString filename);

protected:
    std::map <int32_t /*ckID*/,std::pair<char*, unsigned> /*staff*/> chanks;
    bool _getsubchanks(bool without_data);
    bool _putsubchanks();
    QString _filename;

};



class    CFileWav : public CFileRiff
{
public:
    struct FMT__SUBCHUNK
    {
        int16_t  audioFormat;     // PCM = 1
        uint16_t numChannels;     // Количество каналов. Моно = 1, Стерео = 2 и т.д.
        uint32_t sampleRate;
        uint32_t byteRate;        // ampleRate * numChannels * bitsPerSample/8
        uint16_t blockAlign;
        uint16_t bitsPerSample;
    };

public:
    CFileWav() = delete;
    CFileWav(QString filename);

    void print_fmt_info ();

    std::tuple <char *, int, FMT__SUBCHUNK>  read(bool without_data = false);
    bool write(char *, int, FMT__SUBCHUNK);


};



#endif // CFILEWAV_H
