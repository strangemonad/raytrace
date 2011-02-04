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


#include <strings.h>

#include "defs.hpp"
#include "basicSampler.hpp"
#include "stats.hpp"


BasicSampler::BasicSampler(uint32_t width, 
                           uint32_t height)
   : mWidth(width),
     mHeight(height), // Image array index is 0 based
     mCurX(0),
     mCurY(0)
{
}


void 
BasicSampler::reset()
{
   mCurX = 0;
   mCurY = 0;
}


bool 
BasicSampler::nextSample(Sample *nextSample)
{
   Stats::sharedStats().generatedSamples += 1;
   
   ASSERT(nextSample);
   bzero(nextSample, sizeof(Sample));

   if (mCurX >= mWidth || mCurY >= mHeight) {
      return false;
   }
   
   nextSample->rasterPoint = Point(mCurX, mCurY, 0.0);
   nextSample->weight = 1.0;
   
   if (mCurX < mWidth - 1) {
     mCurX += 1; 
   } else {
      mCurX = 0;
      mCurY += 1;
   }
   
   return true;
}
