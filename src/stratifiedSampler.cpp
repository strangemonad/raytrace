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
#include <stdlib.h>
#include <time.h>

#include "defs.hpp"
#include "prefs.hpp"
#include "stratifiedSampler.hpp"
#include "stats.hpp"


StratifiedSampler::StratifiedSampler(uint32_t width, 
                                     uint32_t height,
                                     uint8_t strata)
   : mWidth(width),
     mHeight(height), // Image array index is 0 based
     mCurX(0),
     mCurY(0),
     mNumStrata(strata),
     mNaiveWeight(1.0 / (strata*strata)),
     mCurXStratum(0),
     mCurYStratum(0),
     mHalfStratWidth(1 / (2.0 * strata)),
     mJitterSamples(Prefs::sharedPrefs().jitterSamples)
{
   if (mJitterSamples) {
     srandom((long)time(NULL));
   }
}


void 
StratifiedSampler::reset()
{
   mCurX = 0;
   mCurY = 0;
   
   mCurXStratum = 0;
   mCurYStratum = 0;
}


bool 
StratifiedSampler::nextSample(Sample *nextSample)
{
   Stats::sharedStats().generatedSamples += 1;
   
   ASSERT(nextSample);
   bzero(nextSample, sizeof(Sample));

   if (mCurX >= mWidth || mCurY >= mHeight) {
      return false;
   }
   
   if (mJitterSamples) {
      double rx = random() / (pow(2.0, 31.0) -1.0);
      double ry = random() / (pow(2.0, 31.0) -1.0); // normalized random Nums
      nextSample->rasterPoint = 
         Point(mCurX + (2*(mCurXStratum + rx) * mHalfStratWidth),
            mCurY + (2*(mCurYStratum + ry) * mHalfStratWidth),
            0.0);
      
      // Jitter once again for lens to get better results.
      rx = random() / (pow(2.0, 31.0) -1.0);
      ry = random() / (pow(2.0, 31.0) -1.0); // normalized random Nums
      nextSample->lensU = 2*(mCurXStratum + rx) * mHalfStratWidth;
      nextSample->lensV = 2*(mCurYStratum + rx) * mHalfStratWidth;
   } else {
      nextSample->rasterPoint = 
         Point(mCurX + ((2*mCurXStratum + 1) * mHalfStratWidth),
               mCurY + ((2*mCurYStratum + 1) * mHalfStratWidth),
               0.0);
      
      nextSample->lensU = (2*mCurXStratum + 1) * mHalfStratWidth;
      nextSample->lensV = (2*mCurYStratum + 1) * mHalfStratWidth;
   }

   nextSample->weight = mNaiveWeight;
   
   if (mCurXStratum < mNumStrata - 1){
      mCurXStratum += 1;
   } else {
      mCurXStratum = 0;
      
      if (mCurYStratum < mNumStrata - 1) {
         mCurYStratum += 1;
      } else {
         mCurYStratum = 0;
         
         if (mCurX < mWidth - 1) {
            mCurX += 1; 
         } else {
            mCurX = 0;
            mCurY += 1;
         }
      }
   }
   return true;
}
