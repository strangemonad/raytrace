/*
 *  camera.hpp
 *  A4
 *
 *  Basic Camer interface. Many cameras can be implemented to acheive different
 *  effects eg: simple pin hole, orthographic, perspective, depth of field, 
 *  fish-eyed, environmental etc.
 *
 *  Created by Shawn Morel on 6/24/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */


#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"
#include "sampler.hpp"


class Camera
{
public: 
   virtual ~Camera(){};
   
   virtual double rayForSample(const Sample &sample, Ray *ray) const = 0;
   
protected:
   Camera(){};
};
          
      
#endif // CAMERA_HPP
