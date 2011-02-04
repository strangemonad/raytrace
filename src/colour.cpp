/*
 *  colour.cpp
 *  A4
 *
 *  Basic object oriented approach to colour manipulation. All colour components 
 *  are represented as double precision floating point between 0 and 1.
 *
 * This represents a 3 sample abstraction of a spectrum power distribution. 
 * Normalizing each sample allows to easily tone map.
 *
 *  Some future work:
 *  - make alpha composition work properly
 *  - handle more complex colour models
 *  - make this into an interface.
 *
 *  Created by Shawn Morel on 6/24/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */


#include "colour.hpp"
#include "defs.hpp"
#include "algebra.hpp"


/*
 *------------------------------------------------------------------------------
 * 
 * Colour::Colour --
 *     
 *     Constructor using rgb components.
 *
 *------------------------------------------------------------------------------
 */

Colour::Colour(double red,   // IN
               double green, // IN
               double blue)  // IN
   : r(min(max(red, 0.0), 1.0)), 
     g(min(max(green, 0.0), 1.0)), 
     b(min(max(blue, 0.0), 1.0))
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * Colour::Colour --
 *     
 *     Constructor for a grey tone. Internally still using rgb components all
 *     set to the grey value. The value will be clampped to 0.0, 1.0
 *
 *------------------------------------------------------------------------------
 */

Colour::Colour(double grey) // IN
   : r(min(max(grey, 0.0), 1.0)),
     g(min(max(grey, 0.0), 1.0)),
     b(min(max(grey, 0.0), 1.0))
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * Colour::Colour --
 *     
 *     Copy Constructor
 *
 *------------------------------------------------------------------------------
 */

Colour::Colour(const Colour& otherColour) // IN
   : r(otherColour.r), 
     g(otherColour.g), 
     b(otherColour.b)
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * operator = --
 *     
 *     Assignment operator.
 *
 *------------------------------------------------------------------------------
 */

Colour& 
Colour::operator=(const Colour& otherColour) // IN
{
   r = otherColour.r;
   g = otherColour.g;
   b = otherColour.b;
      
   return *this;
}


/*
 *------------------------------------------------------------------------------
 * 
 * Colour::~Colour --
 *     
 *     Destructor
 *
 *------------------------------------------------------------------------------
 */

Colour::~Colour()
{
   // Nothing to do for now.
}


/*
 *------------------------------------------------------------------------------
 * 
 * Colour::isBlack --
 *     
 *     Predicate.
 *
 *------------------------------------------------------------------------------
 */

bool 
Colour::isBlack()
{
   return isZero(r) && isZero(g) && isZero(b);
}


/*
 *------------------------------------------------------------------------------
 * 
 * Colour::red --
 *     
 *     Accessor for the red component.
 *
 *------------------------------------------------------------------------------
 */

double 
Colour::red() const 
{ 
   return r;
}


/*
 *------------------------------------------------------------------------------
 * 
 * Colour::red --
 *     
 *     Accessor for the green component.
 *
 *------------------------------------------------------------------------------
 */

double 
Colour::green() const 
{ 
   return g;
}


/*
 *------------------------------------------------------------------------------
 * 
 * Colour::red --
 *     
 *     Accessor for the blue component.
 *
 *------------------------------------------------------------------------------
 */

double 
Colour::blue() const 
{ 
   return b;
}


Colour& 
Colour::operator+=(const Colour& otherColour) // IN
{
   r = min(r + otherColour.r, 1.0);
   g = min(g + otherColour.g, 1.0);
   b = min(b + otherColour.b, 1.0);

   return *this;
}


Colour& 
Colour::operator*=(double s) // IN
{
   r = min(r * s, 1.0);
   g = min(g * s, 1.0);
   b = min(b * s, 1.0);
   
   return *this;
}


Colour& 
Colour::operator*=(const Colour& otherColour) // IN
{
   r = min(r * otherColour.r, 1.0);
   g = min(g * otherColour.g, 1.0);
   b = min(b * otherColour.b, 1.0);
   
   return *this;
}
