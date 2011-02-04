/*
 *  light.cpp
 *  A4
 *
 *  Base class representing a simple point light.
 *
 *  Created by Shawn Morel on 6/25/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */

#include "colour.hpp"
#include "light.hpp"


/*
 *------------------------------------------------------------------------------
 * 
 * Light::Light --
 *     
 *     Constructor
 *
 *------------------------------------------------------------------------------
 */

Light::Light()
   : mColour(*(new Colour(0.0, 0.0, 0.0))),
     mPosition(*(new Point(0.0, 0.0, 0.0)))
{   
   mAttenuationCoeffs.constantAttenuation  = 1.0;
   mAttenuationCoeffs.linearAttenuation    = 0.0;
   mAttenuationCoeffs.quadraticAttenuation = 0.0;
}


Light::Light(Colour& colour, 
             Point& position, 
             AttentuationCoefficients attenuation)
   : mColour(colour),
     mPosition(position),
     mAttenuationCoeffs(attenuation)
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * Light::Light --
 *     
 *     Copy Constructor
 *
 *------------------------------------------------------------------------------
 */

Light::Light(const Light& otherLight) // IN
   : mColour(otherLight.mColour),
     mPosition(otherLight.mPosition),
     mAttenuationCoeffs(otherLight.mAttenuationCoeffs)
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

Light& 
Light::operator =(const Light& otherLight) // IN
{
   mColour = otherLight.mColour; 
   mPosition = otherLight.mPosition;
   mAttenuationCoeffs = otherLight.mAttenuationCoeffs;
   
   return *this;
}


/*
 *------------------------------------------------------------------------------
 * 
 * Light::~Light --
 *     
 *     Destructor
 *
 *------------------------------------------------------------------------------
 */

Light::~Light()
{
   // Nothing to do for now.
}


const Colour& 
Light::getColour() const
{
   return mColour;
}


void 
Light::setColour(const Colour& newColour) // IN
{
   mColour = newColour;
}


const Point& 
Light::getPosition() const
{
   return mPosition;
}


void 
Light::setPosition(const Point& newPosition) // IN
{
   mPosition = newPosition;
}


AttentuationCoefficients
Light::getAttenuationCoefficients() const
{
   return mAttenuationCoeffs;
}
