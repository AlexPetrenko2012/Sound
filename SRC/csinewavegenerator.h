#ifndef CSINEWAVEGENERATOR_H
#define CSINEWAVEGENERATOR_H


#include <utility>


class CSineWaveGenerator
{
public:
    CSineWaveGenerator() = delete;
    CSineWaveGenerator(int sampleRate,
                       int numChannels,
                       int bitsPerSample);

            //    const int sampleRate = 44100;
            //    const int numSamples = sampleRate  * duration;
            //    const int numChannels = 2;
            //    const int bitsPerSample = 16;
            //    const int blockAlign = numChannels * (bitsPerSample/8);
            //    const int subchunk2Size =  numSamples * numChannels * bitsPerSample/8;
            //    const int byteRate = (sampleRate * numChannels * bitsPerSample)/8;



   std::pair<char*/*data*/, int /*size*/> generate(int duration /*sec*/, int frec_rate);


 private:
   const int _sampleRate;
   //const int _numSamples;
   const int _numChannels;
   const int _bitsPerSample;
   //const int blockAlign;
   //const int subchunk2Size;
   //const int byteRate;





};

#endif // CSINEWAVEGENERATOR_H
