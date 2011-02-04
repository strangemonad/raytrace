/*
 *  rayTracer.hpp
 *  A4
 *
 *  Created by Shawn Morel on 6/24/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */

#ifndef RAY_TRACER_HPP
#define RAY_TRACER_HPP

#include <string>

class Image;
class Scene;
class Camera;


// XXX this will go soon
class Point;
class Vector;


// XXX make a set of tracer flags to turn features on and off.

class RayTracer
{

public:
   
   RayTracer();

   virtual ~RayTracer();
   
   void render(const Scene& scene,
               const Camera& camera,
               int width, 
               int height);
   
   const Image& getImage();
   
private:
   RayTracer(const RayTracer& otherRayTracer);
   RayTracer& operator =(const RayTracer& otherRayTracer);
   
   Colour background(const Point &p, 
                     double imageHeight) const;
   
   Image *mImage;
};

#endif // RAY_TRACER_HPP
