/*
 * sampler.hpp
 * A4
 *
 * Interface for Samplers to implement. A Sampler is nothing more that a fancy 
 * iterator that drives the ray tracing. This has many benifits. First it 
 * simplifies the rendering loop, which now just has to initialize a Sampler
 * and loop while the sampler still has samples. This also allows the rendering 
 * loop to become extremely generic and support many types of rendering/
 * sampling. For example we can do a simple interation over all the pixels,
 * do a basic weighted super sample where each pixel is sampled 2, 4, 8 etc. 
 * times with either a jitter or stratification. We can just as easily make a 
 * Sampler that will drive a multi threaded or distributed ray tracer (it would
 * then act as a sample server to all of the rendering threads). Samplers
 * can also be more involved and do a better job of sampling certain parts of 
 * the image based on the frequency domain. If building an interactive ray 
 * tracer you could also conceivably have a sampler that runs indefinitely 
 * rather than for a fixed number of samples; the longer you are viewing a 
 * certain scene without any changes the more progressively refined it becomes.
 * 
 *
 * Created by Shawn Morel on 6/24/06.
 * smorel - 20090740
 * Copyright 2006.
 *
 */


#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include <iostream>

#include "point.hpp"

typedef struct Sample 
{
   Sample()
   {
      rasterPoint = Point(0.0, 0.0, 0.0);
      weight = 1.0;
      lensU = lensV = 0.5;
   }
   
   // The z component should always be 0;
	Point rasterPoint;
   
   /*
    * Parameterization of the point to sample on the lens from 0 .. 1.
    * Specific camera implementations can then map the parameterization onto
    * a specific type of lens shape.
    */
   double lensU;
   double lensV;
   
   /*
    * The contribution that this sample should make towards the total colour
    * of the pixel. This allows super sampling to be done in very flexible ways
    * each super sampler implementation can create samples how it sees fit and
    * assign it a weight between 0.0 and 1.0.
    */
   double weight;
   
   // More info will need to be added here later to handle lenses.
} Sample;



class Sampler 
{
public:
   virtual ~Sampler(){};
   
   virtual void reset() = 0;
   virtual bool nextSample(Sample *nextSample) = 0;
   
protected:
   Sampler(){};
};


inline std::ostream &
operator<<(std::ostream & out,
           const Sample &s)
{
   return out << "(" << s.rasterPoint[0] << ", " 
                     << s.rasterPoint[1] << ", "
                     << s.rasterPoint[2] << ")";
}

#endif // SAMPLER_HPP
