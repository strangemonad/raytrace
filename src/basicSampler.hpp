/*
 * basicSampler.hpp
 * A4
 *
 * A naive approach to sampling, will provide 1 sample per pixel and iterate 
 * over all pixels in the image.
 *
 * Created by Shawn Morel on 6/24/06.
 * smorel - 20090740
 * Copyright 2006.
 *
 */


#ifndef BASIC_SAMPLER_HPP
#define BASIC_SAMPLER_HPP

#include "sampler.hpp"

class BasicSampler
   : public Sampler
{
public:
   BasicSampler(uint32_t width, uint32_t height);

   virtual ~BasicSampler(){};
   
   virtual void reset();
   virtual bool nextSample(Sample *nextSample);
   
private:
   const uint32_t mWidth;
   const uint32_t mHeight;
   
   uint32_t mCurX;
   uint32_t mCurY;
      
};

#endif // BASIC_SAMPLER_HPP
