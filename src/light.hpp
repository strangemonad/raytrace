/*
 *  light.hpp
 *  A4
 *
 *  Base class representing a simple point light.
 *
 *  Created by Shawn Morel on 6/25/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <iostream>

#include "colour.hpp"
#include "point.hpp"


typedef struct {
   double constantAttenuation;
   double linearAttenuation;
   double quadraticAttenuation;
} AttentuationCoefficients;

class Colour;

#include "algebra.hpp"
// XXX clean this up.

class Light 
{
public:
   Light();
   Light(Colour& colour, 
         Point& position, 
         AttentuationCoefficients attenuation);
   
   Light(const Light& otherLight);
   Light& operator =(const Light& otherLight);
   
   virtual ~Light();
   
   const Colour& getColour() const;
   void setColour(const Colour& newColour);

   const Point& getPosition() const;
   void setPosition(const Point& newPosition);
   
   AttentuationCoefficients getAttenuationCoefficients() const;
   
private: 
  Colour mColour;
  Point mPosition;
  AttentuationCoefficients mAttenuationCoeffs;
};


/*
 *------------------------------------------------------------------------------
 * 
 * Light::operator << --
 *     
 *     Print the description of the light to the output stream.
 *
 *------------------------------------------------------------------------------
 */

inline std::ostream& 
operator <<(std::ostream& out,  // IN
            const Light& light) // IN
{
   AttentuationCoefficients c = light.getAttenuationCoefficients();
   
   out << "L[" 
      << light.getColour() << ", "
      << light.getPosition() << ", "
      
      << c.constantAttenuation << ", "
      << c.linearAttenuation << ", "
      << c.quadraticAttenuation
      << "]";
   
   return out;
};

#endif // LIGHT_HPP
