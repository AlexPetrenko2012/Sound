
#include <iostream>

#include "csinewavegenerator.h"
#include "sample_formats.h"
#include "math.h"
#include <map>
#include <utility>
#include <functional>
#include <QDebug>

CSineWaveGenerator::CSineWaveGenerator(int sampleRate,
                   int numChannels,
                   int bitsPerSample):
    _sampleRate(sampleRate),_numChannels(numChannels),_bitsPerSample(bitsPerSample)
{}




std::pair<char*/*data*/, int /*size*/> CSineWaveGenerator::generate(int duration /*sec*/, int frec_rate)
{

const int numSamples = _sampleRate  * duration;
const int size =  numSamples * _numChannels * _bitsPerSample/8;


const double periods = ((double)numSamples/duration/frec_rate)/(2*3.141592);
const double increm = (double)(3.141592 * 2)/periods;

std::cout << "numSamples / 2П = " <<  numSamples << " / 2П  = " << periods << std::endl;
std::cout << "increm2 = " <<  increm << std::endl;


std::map<int32_t,std::function <void(int)>> mf;

char* data = new char [size];
double d=0.0;

mf.insert(std::make_pair( // 16bit mono
0x00100001,
        [&](int i){
        S_FRAME_16M* psf16m = reinterpret_cast<S_FRAME_16M*>(data);
       (psf16m+i)->sample = (int16_t)(int)(sin(d)*INT16_MAX);}));

mf.insert(std::make_pair( // 16bit stereo
0x00100002,
        [&](int i){
        S_FRAME_16S* psf16s = reinterpret_cast<S_FRAME_16S*>(data);
        (psf16s+i)->sample1 = (int16_t)(int)(sin(d)*INT16_MAX);
        (psf16s+i)->sample2 = (psf16s+i)->sample1;}));

int32_t format = ( _bitsPerSample << 16) | _numChannels;

auto it = mf.find(format);
if(it == mf.end())
         {
         qDebug() << "Not supported";
         return std::make_pair(nullptr,0);
         }
auto [key,value] = *it;

for (int i = 0; i<numSamples; i++)
{
    value(i);
    d += increm;
}

return std::make_pair(data,size);

}
