#include "cfilewav.h"

#include <iostream>
#include <utility>

#include <QString>

#include <QDebug>


using namespace std;



CFileRiff::CFileRiff(QString filename)
    :
      _filename(filename){}


CFileWav::CFileWav(QString filename)
    :
      CFileRiff(filename){}


void CFileWav :: print_fmt_info ()
{
    std::map <int32_t,std::pair<char*,unsigned>>::iterator search = chanks.find(fmt__ID);


    FMT__SUBCHUNK fmt =  *reinterpret_cast<FMT__SUBCHUNK*>(search->second.first);

    ;
    if (search == chanks.end())
    {
        qDebug() << "No <fmt > subchank finded";
        return;
    }

    // cout << "format:         " <<  make_cstring<4>(header.format) << endl;
    cout << "audioFormat:    " <<  i2a4AF(fmt.audioFormat)  <<  endl;
    cout << "numChannels:    " <<  fmt.numChannels  <<  endl;
    cout << "sampleRate:     " <<  fmt.sampleRate  <<  endl;
    cout << "blockAlign:     " <<  fmt.blockAlign  <<  endl;
    cout << "byteRate:       " <<  fmt.byteRate << endl;
    cout << "bitsPerSample:  " <<  fmt.bitsPerSample  <<  endl;
    cout << endl;
}
///}


std::tuple <char *, int, CFileWav::FMT__SUBCHUNK> CFileWav::read(bool without_data)
{

    using FMT = FMT__SUBCHUNK;
    using namespace std;;

    if (!_getsubchanks(without_data))
        return make_tuple<char *, int, FMT>(nullptr,0,{0,0,0,0,0,0});

    map <int32_t,pair<char*,unsigned>>::iterator search = chanks.find(fmt__ID);
    FMT fmt =  *reinterpret_cast<FMT*>(search->second.first);

    ;
    if (search == chanks.end())
    {
        qDebug() << "No <fmt > subchank finded";
        return make_tuple<char *, int, FMT>(nullptr,0,{0,0,0,0,0,0});
    }


    search = chanks.find(data_ID);

    if (search == chanks.end())
    {
        qDebug() << "No <data> subchank finded";
        return make_tuple<char *, int, FMT>(nullptr,0,{0,0,0,0,0,0});
    }


    return make_tuple<char *, int, FMT>(move(search->second.first), move(search->second.second), move(fmt));

}

bool CFileRiff::_getsubchanks(bool without_data)
{

    QFile file (_filename);


    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<file.errorString();
        return false;
    }


    struct{
        uint32_t  ckID_riff;
        uint32_t  ckSize;
        uint32_t  ftID_WAVE;
    }riff_chank;

    if (file.read(reinterpret_cast<char *>(&riff_chank),sizeof(riff_chank))==-1)
    {
        qDebug()<<file.errorString();
        return false;
    }

    if (riff_chank.ckID_riff  != RIFF_ID)
    {
        qDebug()<< "Fatal error.Can not find RIFF chank.";
        return false;
    }

    if (riff_chank.ftID_WAVE  != WAVE_ID)
    {
        qDebug()<< "Fatal error.Can not find RIFF: WAVE format fild in RIFF chank.";
        return false;
    }

    struct{
        uint32_t  ckID;
        uint32_t  ckSize;
    }subchank_header;

    int result;



    while (1)
    {

        result = file.read(reinterpret_cast<char *>(&subchank_header),sizeof(subchank_header));

        if (result == -1)
        {
            qDebug() << file.errorString();
            qDebug() << "Fatal error.Can not read subchanks";
            return false;
        }

        if (result == 0)
            break;    // All subchanks are read



        if(!subchank_header.ckSize | !subchank_header.ckID)
            break; //


        char * subchank_staff =  new char[subchank_header.ckSize];

        // Verify the parity
        if (subchank_header.ckSize%2)
            subchank_header.ckSize++;


        result = file.read(reinterpret_cast<char *>(subchank_staff),subchank_header.ckSize);

        if (result == -1)
        {
            qDebug() << file.errorString();
            qDebug() << "Fatal error.Can not read subchanks";
            return false;
        }

        if (result == 0)
            break;    // All subchanks are read

        if (!(subchank_header.ckID == data_ID && without_data))
        {
            auto iresult = chanks.insert(make_pair(subchank_header.ckID,make_pair(subchank_staff,subchank_header.ckSize)));

            if (!iresult.second)
            {
                qDebug() << "error. Such subchunk already present";
                delete[] subchank_staff;
            }
        }
    }

    return true;
}




