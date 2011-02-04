/*
 *  colour.hpp
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


#ifndef COLOUR_HPP
#define COLOUR_HPP

#include <iostream>

#include "algebra.hpp"

class Colour
{
public:
   Colour():r(0.0), g(0.0), b(0.0){};
   Colour(double red, double green, double blue);
   Colour(double grey);
   
   Colour(const Colour& colour);   
   Colour &operator=(const Colour& otherColour);
      
   virtual ~Colour();
   
   bool isBlack();

   double red() const;
   double green() const;
   double blue() const;
   
   Colour &operator+=(const Colour &otherColour);
   Colour &operator*=(double s);
   Colour &operator*=(const Colour &otherColour);

private:
   double r;
   double g;
   double b;

};


inline Colour
operator *(double s, 
           const Colour& a)
{
   return Colour(min(s * a.red(), 1.0),
                 min(s * a.green(), 1.0),
                 min(s * a.blue(), 1.0));
};


inline Colour 
operator *(const Colour& a, 
           const Colour& b)
{
   return Colour(min(a.red()* b.red(), 1.0),
                 min(a.green() * b.green(), 1.0),
                 min(a.blue() * b.blue(), 1.0));
};


inline Colour 
operator +(const Colour& a, 
           const Colour& b)
{
   return Colour(min(a.red() + b.red(), 1.0),
                 min(a.green() + b.green(), 1.0),
                 min(a.blue() + b.blue(), 1.0));
};


inline std::ostream& operator <<(std::ostream& out, 
                                 const Colour& c)
{
   return out << "c<" << c.red() << "," << c.green() << "," << c.blue() << ">";
};

#endif
