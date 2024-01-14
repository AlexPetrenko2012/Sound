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



bool CFileWav::write(char * data, int size, FMT__SUBCHUNK fmt)
{
    using FMT = FMT__SUBCHUNK;
    using namespace std;

    for (const auto &[key,value] : chanks)
    {
        const auto &[data,size] = value;
        delete[] data;
    }
    chanks.clear();


    FMT*pfmt =  new FMT(fmt);


    chanks.insert(make_pair(fmt__ID,make_pair(reinterpret_cast<char*>(pfmt),sizeof(FMT))));
    chanks.insert(make_pair(data_ID,make_pair(data,size)));


    if (!_putsubchanks())
        return false;

    return true;
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

bool CFileRiff::_putsubchanks()
{

    QFile file (_filename);


    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug()<<file.errorString();
        return false;
    }


    int ckSize = 0; // All chanks


    struct{
        uint32_t  ckID;
        uint32_t  ckSize;
    }subchank_header;


    for (const auto &[key,value] : chanks)
    {
       auto [data,size] = value;
       if (size%2) // Verify the parity
                size++;
        ckSize += (size + sizeof(subchank_header));
    }


    struct{
        uint32_t  ckID_riff = RIFF_ID;
        uint32_t  ckSize = sizeof(ftID_WAVE);
        uint32_t  ftID_WAVE = WAVE_ID;
    }riff_chank;



    riff_chank.ckSize += ckSize;



    if (file.write(reinterpret_cast<char *>(&riff_chank),sizeof(riff_chank))==-1)
    {
        qDebug()<<file.errorString();
        return false;
    }




    for (const auto &[key,value] : chanks)
    {
        const auto &[data,size] = value;


        subchank_header.ckID =   key;
        subchank_header.ckSize = size;

        if (file.write(reinterpret_cast<char *>(&subchank_header),sizeof(subchank_header))==-1)
        {
            qDebug()<<file.errorString();
            return false;
        }


        if (file.write(data,size)==-1)
        {
            qDebug()<<file.errorString();
            return false;
        }


        // Verify the parity
        if (size%2)
        {
            char padding = 0x00;

            if (file.write(&padding,1)==-1)
            {
                qDebug()<<file.errorString();
                return false;
            }

        }

    }

    return true;
}





