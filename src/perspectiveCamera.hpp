/*
 *  perspectiveCamera.hpp
 *  A4
 *
 *  Camera that simulates depth of field effect for a simple spherical glass 
 *  lens. The rays produced are perturbed according to where the ray intersects 
 *  the lens as specified in the sample's u,v parameterization. If the 
 *  lensRadius passed to the constructor is 0.0 then the focal length is 
 *  inconsequential and the camera will behave like a simple pin-hole camera
 *  and no circle of confusion / depth of field will be sampled.
 *
 *  Created by Shawn Morel on 6/24/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */


#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#include <cmath>

#include "camera.hpp"
#include "point.hpp"
#include "ray.hpp"
#include "sampler.hpp"
#include "transform.hpp"
#include "vector.hpp"

class Point;
class Vector;


class PerspectiveCamera
   : public Camera
{   
public:
   /*
    * Position and view direction are in World coordinates. Up is always
    * [0,1,0]. The lookAt parameter has no effect on near, far (or focal distance
    * field of view etc. in subclasses). It's only used to rotate the camera into 
    * the desired orientation.
    */
   PerspectiveCamera(const Point &position = Point(0.0, 0.0, 0.0),
                     const Point &lookAt = Point(0.0, 0.0, 1.0),
                     const Vector &up = Vector(0.0, 1.0, 0.0),
                     double fov = 90,
                     double near = 0.1,
                     double far = INFINITY,
                     double xResolution = 0.0,
                     double yRexolution = 0.0,
                     double lensRadius = 0.0,
                     double focalDistance = 0.0); // XXX do I want INFINITY?
   
   /* 
    * XXX consider making a constructor that behaves even more like a real 
    * camera by allowing the user the specify tilt, pan, roll etc. Both methods
    * are conveniant for different purposes.
    */
   
   virtual double rayForSample(const Sample &sample, Ray *ray) const;
   
protected:
   Transform mRasterToCamera;
   Transform mCameraToWorld;

   double mNear; // Hither in camera terminology
   double mFar;  // Yon in camera terminology
   
   double mXRes;
   double mYRes; // XXX these are only here for debugging.
   
   double mLensRadius;
   double mFocalDistance;
   
private:
   void lensCoords(double lensU, double lensV,
                   double *lensDX, double *lensDY) const;
};
          
      
#endif // PERSPECTIVE_CAMERA_HPP
