#include <iostream>

#include <QCoreApplication>
#include <QFile>

#include "../SRC/cfilewav.h"


//#include "/home/alexandr/WRK/BAUER/wavreader/wav.h"
#include "math.h"


using namespace std;

/*
void print_header (WAVHEADER&  header)
{

    cout << "format:         " <<  make_cstring<4>(header.format) << endl;
    cout << "audioFormat:    " <<  i2a4AF(header.audioFormat)  <<  endl;
    cout << "bitsPerSample:  " <<  header.bitsPerSample  <<  endl;
    cout << "numChannels:    " <<  header.numChannels  <<  endl;
    cout << "blockAlign:     " <<  header.blockAlign  <<  endl;
    cout << "sampleRate:     " <<  header.sampleRate  <<  endl;
    cout << "byteRate:       " <<  header.byteRate << endl;


    cout << endl;


    cout << "chunkId:        " <<  make_cstring<4>(header.chunkId) << endl;
    cout << "chunkSize:      " <<  header.chunkSize << endl;
    cout << "subchunk1Id:    " <<  make_cstring<4>(header.subchunk1Id) << endl;
    cout << "subchunk1Siz:   " <<  header.subchunk1Size << endl;
    cout << "subchunk2Id:    " <<  make_cstring<4>(header.subchunk2Id) << endl;
    cout << "subchunk2Size:  " <<  header.subchunk2Size << endl;
}

*/


/*

void write_header(QFile& file,  WAVHEADER&  header)
{
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug()<<file.errorString();
        return;
    }


    if (file.write(reinterpret_cast<char *>(&header),sizeof(WAVHEADER))==-1)
      return;
file.close();
}

*/



//void read_header(QFile& file,  WAVHEADER&  header)
//{

//if (!file.open(QIODevice::ReadOnly))
//{
//    qDebug()<<file.errorString();
//    return;
//}


//if (file.read(reinterpret_cast<char *>(&header),sizeof(WAVHEADER))==-1)
//  return;

//file.close();
//return;
//}





//char * read_data(QFile& file,  WAVHEADER&  header)
//{
//const size_t size = header.subchunk2Size;
//char* data = new char[size];

//if (!file.open(QIODevice::ReadOnly))
//{
//    qDebug()<<file.errorString();
//    return nullptr;
//}

//file.seek(sizeof(WAVHEADER));


//if (file.read(data,size)==-1)
//  return nullptr;

//file.close();

//cout << "DATA: " ;

//if (size > 8)
//    for (int i =0; i<8; i++)
//       cout << hex << (unsigned) (unsigned char) data[i] << " " << dec;

//cout << endl;

//return data;
//}


/*
void write_data(QFile& file,WAVHEADER&  header,char * data)
{
const size_t size = header.subchunk2Size;


if (!file.open(QIODevice::Append))
{
    qDebug()<<file.errorString();
    return;
}


if (file.write(data,size)==-1)
{
  qDebug()<<file.errorString();
  return;
}

file.close();
return;
}
*/

//char * generate_date(WAVHEADER&  header /*OUT*/,int duration /*sec*/, int frec_rate )
//{
//    const int sampleRate = 44100;
//    const int numSamples = sampleRate  * duration;
//    const int numChannels = 2;
//    const int bitsPerSample = 16;
//    const int blockAlign = numChannels * (bitsPerSample/8);
//    const int subchunk2Size =  numSamples * numChannels * bitsPerSample/8;
//    const int byteRate = (sampleRate * numChannels * bitsPerSample)/8;

//    memcpy(header.format,"WAVE",4);
//    header.audioFormat = 1;
//    header.bitsPerSample = bitsPerSample;
//    header.numChannels = numChannels;
//    header.blockAlign = blockAlign;
//    header.sampleRate = sampleRate;
//    header.byteRate = byteRate;

//    memcpy(header.chunkId,"RIFF",4);
//    header.chunkSize = 36 + subchunk2Size;
//    memcpy(header.subchunk1Id,"fmt ",4);
//    header.subchunk1Size = 16;
//    memcpy(header.subchunk2Id,"data",4);
//    header.subchunk2Size = subchunk2Size;


//double periods = ((double)numSamples/duration/frec_rate)/(2*3.141592);
//double increm = (double)(3.141592 * 2)/periods;

//cout << "numSamples / 2П = " <<  numSamples << " / 2П  = " << periods << endl;
//cout << "increm2 = " <<  increm << endl;


//S_FRAME_16S* sf16s = new S_FRAME_16S[numSamples];


//double d=0.0;
//for (int i = 0; i<numSamples; i++)
//{
//sf16s[i].sample1 = (int16_t)(int)(sin(d)*INT16_MAX);
//sf16s[i].sample2 = sf16s[i].sample1;
//d += increm;
//}


//return reinterpret_cast<char*>(sf16s);

//}


/*
int main(int argc, char *argv[])
{

//QFile file1 ("/home/alexandr/sample-3s.wav");
QFile file2 ("/home/alexandr/sample-3s+.wav");


WAVHEADER header;

//read_header(file1,header);
//char * data = read_data(file1,header);
//print_header(header);

cout << "----------------" << endl << endl;

char * data2 = generate_date(header,10,100);

write_header(file2,header);

write_data(file2,header,data2);

print_header(header);

//read_header(file2,header);
//char * data2 = read_data(file2,header);
//print_header(header);

return 0;

//delete[] data;
delete[] data2;

    QCoreApplication a(argc, argv);

    return a.exec();
}
*/




int main(int argc, char *argv[])
{


CFileWav fw("/home/alexandr/sample-3s.wav");

auto t = fw.read();


char* data = get<0>(t);
int size  = get<1>(t);
CFileWav::FMT__SUBCHUNK fmt = get<2>(t);

fw.print_fmt_info();




//read_header(file1,header);
//char * data = read_data(file1,header);
//print_header(header);

cout << "----------------" << endl << endl;

//char * data2 = generate_date(header,10,100);

//write_header(file2,header);

//write_data(file2,header,data2);

//print_header(header);

//read_header(file2,header);
//char * data2 = read_data(file2,header);
//print_header(header);

return 0;

//delete[] data;
//delete[] data2;

    QCoreApplication a(argc, argv);

    return a.exec();
}

