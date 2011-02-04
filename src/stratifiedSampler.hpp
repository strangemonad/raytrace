/*
 * stratifiedSampler.hpp
 * A4
 *
 * A naive approach to stratified sampling, will provide s*s samples per pixel 
 * (where s is the number of strata) and iterate over all pixels in the image.
 *
 * Created by Shawn Morel on 7/21/06.
 * smorel - 20090740
 * Copyright 2006.
 *
 */


#ifndef STRATIFIED_SAMPLER_HPP
#define STRATIFIED_SAMPLER_HPP

#include "sampler.hpp"

class StratifiedSampler
   : public Sampler
{
public:
   StratifiedSampler(uint32_t width, uint32_t height, uint8_t strata = 1);

   virtual ~StratifiedSampler(){};
   
   virtual void reset();
   virtual bool nextSample(Sample *nextSample);
   
private:   
   const uint32_t mWidth;
   const uint32_t mHeight;
   
   uint32_t mCurX;
   uint32_t mCurY;
   
   uint8_t mNumStrata;
   double mNaiveWeight; // simple box filter (just average each pixel)
   
   uint8_t mCurXStratum;
   uint8_t mCurYStratum;
   
   double mHalfStratWidth;
   
   bool mJitterSamples;
};

#endif // STRATIFIED_SAMPLER_HPP
