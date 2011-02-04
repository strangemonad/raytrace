/*
 * stats.hpp
 * A5
 *
 * Basic stats counter singleton. Call init when program starts and exit before
 * execution ends to print out comprehensive statistics about the render.
 *
 * Created by Shawn Morel on 7/14/06.
 * smorel - 20090740
 * Copyright 2006.
 *
 */


#ifndef STATS_HPP
#define STATS_HPP

#include <iostream>
#include <time.h>

#include <unistd.h>

class Stats 
{
public:
   static Stats & sharedStats()
   {
      static Stats s;
      return s;
   };
   
   ~Stats(){};
   
   
   void init();
   void reset();
   void nextRender();
   void exit();
   
   // Public members for simplicity
   
   // Intersection tests
   uint32_t primitiveIntersections;
   uint32_t rapidIntersections;
   uint32_t boundingBoxIntersections;
   
   uint32_t imageWidth;
   uint32_t imageHeight;
   
   uint32_t generatedSamples;
   uint32_t raysCast;
   
   time_t startTime;
   
private:
   Stats(){};
   
   uint8_t numRenders;
};

inline void
printTime(std::ostream &out, const time_t &time)
{
   tm *t = gmtime(&time);
   
   (t->tm_hour < 10) ? out << "0" << t->tm_hour : out << t->tm_hour;
   out << ":";
   
   (t->tm_min < 10) ? out << "0" << t->tm_min : out << t->tm_min;
   out << ":";
   (t->tm_sec < 10) ? out << "0" << t->tm_sec : out << t->tm_sec;
}


inline std::ostream &
operator<<(std::ostream &out, const Stats &s)
{
   out << std::endl 
      << "::Statistics::" << std::endl << std::endl
      << "Image " << s.imageWidth << " x " << s.imageHeight 
      << " => " << s.imageWidth * s.imageHeight << " pixels" << std::endl
      << "Generated " << s.raysCast << " samples" << std::endl
      << "Cast " << s.raysCast << " rays" << std::endl << std::endl;
      
   double totalIntersections = s.primitiveIntersections + s.rapidIntersections
                               + s.boundingBoxIntersections;
   double avgIntersectionsPerRay = totalIntersections / s.raysCast;
   out << "Intersection tests" << std::endl 
      << "Primitive: "<< s.primitiveIntersections
      << "  Rapid: " << s.rapidIntersections
      << "  Bounding Box: " << s.boundingBoxIntersections << std::endl
      << "Total: " << totalIntersections << std::endl 
      << "Average per ray: " << avgIntersectionsPerRay << std::endl << std::endl;
   
   out << "Time: ";
   printTime(out, time(NULL) - s.startTime);
   out << std::endl << std::endl;

   return out;
};

#endif // STATS_HPP
