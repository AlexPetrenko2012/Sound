
#include <iostream>

#include "csinewavegenerator.h"
#include "sample_formats.h"
#include "math.h"


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


S_FRAME_16S* sf16s = new S_FRAME_16S[numSamples];  // TODO ( other formats too)


double d=0.0;
for (int i = 0; i<numSamples; i++)
{
sf16s[i].sample1 = (int16_t)(int)(sin(d)*INT16_MAX);
sf16s[i].sample2 = sf16s[i].sample1;
d += increm;
}


return std::make_pair(reinterpret_cast<char*>(sf16s),size);


}
