/*
 * prefs.cpp
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


#include <iostream>

#include "prefs.hpp"


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
Prefs::reset()
{
   boundsChecking = false;
   
   numStrata = 1;
   jitterSamples = false;
   
   lensRadius = 0.0;
   focalDistance = 0.0;
   
   rayRecursions = 1;
   
   runInteractive = false;
}
