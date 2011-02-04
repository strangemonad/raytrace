/*
 * prefs.hpp
 * A5
 *
 * Preferences that can be set through LUA commands to control the rendering.
 * Makes it easy to demonstrate certain features.
 *
 * Created by Shawn Morel on 7/16/06.
 * smorel - 20090740
 * Copyright 2006.
 *
 */


#ifndef PREFS_HPP
#define PREFS_HPP

#include <iostream>
#include <time.h>

#include <unistd.h>

class Prefs 
{
public:
   static Prefs &sharedPrefs()
   {
      static Prefs p;
      return p;
   };
   
   ~Prefs(){};

   void reset();
   
   // Public members for simplicity
   bool boundsChecking;
   
   uint8_t numStrata;
   bool jitterSamples;
   
   double lensRadius;
   double focalDistance;
   
   uint8_t rayRecursions;
   
   bool runInteractive;
   
private:
   Prefs(){ reset(); };   
};


inline std::ostream &
operator<<(std::ostream &out, const Prefs &p)
{
   out << std::endl 
      << "::Preferences::" << std::endl << std::endl
      << "Bounds Checking: " << p.boundsChecking << std::endl
      << "Num Sampling Strata: " << (int)p.numStrata << std::endl
      << "Jitter Samples: " << p.jitterSamples << std::endl
      << "Lens Radius: " << p.lensRadius << std::endl
      << "Focal Distance: " << p.focalDistance << std::endl
      << "Ray Recursions: " << (int)p.rayRecursions << std::endl;
      //<< "Run Interactive: " << p.runInteractive << std::endl;

   return out;
};

#endif // PREFS_HPP
