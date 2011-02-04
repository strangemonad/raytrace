/*
 *  rayTracer.cpp
 *  A4
 *
 *  Created by Shawn Morel on 6/24/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */

#include <iostream>
#include <stdlib.h>

#include "camera.hpp"
#include "colour.hpp"
#include "geometryNode.hpp"
#include "image.hpp"
#include "light.hpp"
#include "material.hpp"
#include "normal.hpp"
#include "point.hpp"
#include "primitive.hpp"
#include "prefs.hpp"
#include "rayTracer.hpp"
#include "basicSampler.hpp"
#include "stratifiedSampler.hpp"
#include "scene.hpp"
#include "stats.hpp"


/*
 *------------------------------------------------------------------------------
 * 
 * RayTracer::RayTracer --
 *     
 *     Constructor.
 *
 *------------------------------------------------------------------------------
 */

RayTracer::RayTracer()
   : mImage(new Image())
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * RayTracer::RayTracer --
 *     
 *     Copy Constructor.
 *
 *------------------------------------------------------------------------------
 */

RayTracer::RayTracer(const RayTracer& otherRayTracer) // IN
   : mImage(new Image(*otherRayTracer.mImage))
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * RayTracer::operator = --
 *     
 *     Assignment operator.
 *
 *------------------------------------------------------------------------------
 */

RayTracer& 
RayTracer::operator =(const RayTracer& otherRayTracer) // IN
{
   if (mImage) {
      delete mImage;
   }
   mImage = new Image(*otherRayTracer.mImage);
   
   return *this;
}


/*
 *------------------------------------------------------------------------------
 * 
 * RayTracer::~RayTracer --
 *     
 *     Destructor.
 *
 *------------------------------------------------------------------------------
 */

RayTracer::~RayTracer()
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * RayTracer::render --
 *     
 *     Entry point for the rendering routine. When this returns the image will 
 *     be fully rendered and can be obtained by calling getImage().
 *
 *     XXX need to make and interactive version of this.
 *
 *------------------------------------------------------------------------------
 */

void 
RayTracer::render(const Scene& scene,   // IN
                  const Camera& camera, // IN
                  int width,            // IN
                  int height)           // IN: Image size
{
   if (mImage) {
      delete mImage;
   }
   mImage = new Image(width, height, 3);
   
   Sampler *sampler;
   if (Prefs::sharedPrefs().jitterSamples || 
              Prefs::sharedPrefs().numStrata > 1) {
      sampler = new StratifiedSampler(width, height,
                                      Prefs::sharedPrefs().numStrata);
   } else {
      sampler = new BasicSampler(width, height); 
   }
      
   Sample sample;
   Ray r;
   NodeIntersection ni;
   
   while (sampler->nextSample(&sample)) {
      // XXX not sure if we wil need ray weight returned by rayForSample().
      camera.rayForSample(sample, &r);
      
      Stats::sharedStats().raysCast += 1;
      
      Colour cResulting; // resulting colour
      
      cResulting = background(sample.rasterPoint, height);
      
      if (scene.intersect(r, &ni)) {
         // Pass the environmental colour for reflective and transmissive calcs.
         cResulting = scene.getColourAtIntersection(r, ni, cResulting);
      }
      
      mImage->addColour(sample.weight * cResulting, 
                        (int)sample.rasterPoint[0], 
                        (int)sample.rasterPoint[1]);
   }
}


/*
 *------------------------------------------------------------------------------
 * 
 * RayTracer::getImage --
 *     
 *     Use this to obtain the rendered image. Possibly to save it or display it
 *     on screen.
 *
 *------------------------------------------------------------------------------
 */

const Image& 
RayTracer::getImage()
{
   return *mImage;
}


/*
 *------------------------------------------------------------------------------
 * 
 * RayTracer::background --
 *     
 *     Generates the background colour at a given point. This is arguably 
 *     somewhat more efficient than generating the entire background and then 
 *     painting the scene on top.
 *
 *------------------------------------------------------------------------------
 */

Colour 
RayTracer::background(const Point &p,
                      double imageHeight) const
{
   // XXX move this to Scene and turn the parameterization into spherical coords.
   
   /*
    * Generate a night sky. 
    * If we are in the top 40% of the image randomaly generate starts
    * scale from mignight blue at the top to azure at the bottom.
    */
   
   //midnightBlue = (0, 0, 0.5);
   //sky = (0.47, 0.8, 1.0);
   
   // % of distance from the top.
   double gradient = (imageHeight - p[1])/imageHeight;  
   if (rand() < 0.9990 * RAND_MAX) {
      /*
       * 10% chance of getting a star in the top 40% of the image. just leave 
       * that one white otherwise linearly interpolate between the 2 colours.
       */
      return Colour(/* Skip 0      */ 0.47 * (1 - gradient),
                 /* component   */ 0.80 * (1 - gradient),
                 0.50 * gradient + 1.00 * (1 - gradient));                     
   } else {
      return Colour(1.0, 1.0, 1.0);
   }
}

