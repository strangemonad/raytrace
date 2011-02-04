/*
 * stats.cpp
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


#include <iostream>

#include "stats.hpp"


void 
Stats::init()
{
   reset();
   numRenders = 0;
}


/*
 *------------------------------------------------------------------------------
 * 
 * Stats::reset --
 *     
 *     Reset all counters
 *
 *------------------------------------------------------------------------------
 */

void
Stats::reset()
{
   primitiveIntersections = 0;
   rapidIntersections = 0;
   boundingBoxIntersections = 0;
   
   imageWidth = 0;
   imageHeight = 0;
   
   generatedSamples = 0;
   raysCast = 0;
   
   startTime = time(NULL);
}



void 
Stats::nextRender()
{
   if (numRenders > 0) {
      std::cout << *this;
      reset();
   }
   
   numRenders += 1;
}


/*
 *------------------------------------------------------------------------------
 * 
 * Stats::exit --
 *     
 *     Prints stats and resets. 
 *
 *------------------------------------------------------------------------------
 */

void
Stats::exit()
{
   std::cout << *this;
   reset();
}
