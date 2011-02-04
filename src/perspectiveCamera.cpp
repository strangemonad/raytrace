/*
 *  perspectiveCamera.cpp
 *  A4
 *
 *  Basic perspective projection transformation camera.
 *
 *  Created by Shawn Morel on 6/24/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */


#include "defs.hpp"
#include "perspectiveCamera.hpp"

PerspectiveCamera::PerspectiveCamera(const Point &position,
                                     const Point &lookAt,
                                     const Vector &up,
                                     double fov,
                                     double near,
                                     double far,
                                     double xResolution,
                                     double yResolution,
                                     double lensRadius,
                                     double focalDistance)
   : mNear(near),
     mFar(far),
     mXRes(xResolution),
     mYRes(yResolution),
     mLensRadius(lensRadius),
     mFocalDistance(focalDistance)
{
   // ie: view to world
   mCameraToWorld = LookAt(position, 
                           lookAt, 
                           up).getInverse();
         
   /*
    * The non-normalized screen to raster transform is taken dirrectly from 
    * PBRT. But really there are only so many ways to do this transformation.
    */
   Transform screenToRaster = 
      Scale(xResolution, yResolution, 1.0) * 
      Scale(0.5, -0.5, 1.0) *
      Translate(Vector(1.0, -1.0, 0.0));
   
   // ie: Raster to View.
   mRasterToCamera = Perspective(fov, near, far).getInverse() * 
                     screenToRaster.getInverse();
}


double 
PerspectiveCamera::rayForSample(const Sample &sample, 
                                Ray *ray) const
{
   ASSERT(sample.rasterPoint[0] <= mXRes);
   ASSERT(sample.rasterPoint[1] <= mYRes);
   ASSERT(sample.rasterPoint[2] == 0.0);
   
   if (!ray) {
      ray = new Ray();
   }
   
   /*
    * Samplers provide a point in the raster coordinate space. View want that
    * point in the Camera (or View) coordinate space.
    */
   Point pCamera = mRasterToCamera(sample.rasterPoint);
      
   /*
    * We now have a point on the near plane our ray should originate from that 
    * point (ie ray(0) = pCamera). The direction of the ray should be from the 
    * origin (ie: the camera (0,0,0)) to the point on the plane and should be 
    * parameterized to start on the near plane and stop on the far plane.
    */
   ray->o = pCamera;
   ray->d = Vector(pCamera); // Make vector using point components.

   if (mLensRadius > 0.0) {
      double lensX, lensY;
		lensCoords(sample.lensU, sample.lensV,
                 &lensX, &lensY);
      
      float ft = (mFocalDistance - mNear) / ray->d.z;
		Point pFocus = (*ray)(ft);
      
		/*
       * Update the generated ray. The ray now originates from the sampled point
       * and the ray direction is from the sample point to the point in focus 
       * on the focal plane.
       */
		ray->o.x += lensX;
		ray->o.y += lensY;
		ray->d = pFocus - ray->o;
   }
   
   
   ray->mint = 0.0; // Yes we actually want 0 here not RAY_EPSILON;
   if (finite(mFar)) {
      ray->maxt = (mFar - mNear) / ray->d.z;
   } else {
      ray->maxt = INFINITY;
   }
   
   /*
    * Finally we are ready to convert the ray into world coordinates. Remember
    * since this is a camera we constructed the world to view transform to not
    * have any scaling (ie: the ray will still be normalized).
    */
   mCameraToWorld(*ray, ray);
   
   return 1.0;
}


/*
 * If the lens has a non-zero radius a depth of field effect will come into play.
 * The ray tracer will automatically sample many lens points for each pixel.
 * To iolate the sampler from the details of the Camera all that the sampler 
 * provides is a lens parameterization from [0, 1] in 2 variables. We must
 * convert from this parameterization to actual x and y coordinates on the disk
 * cross section in the center of the lens.
 *
 * This essentially maps a square to a circle and pays close attention to 
 * retaining a uniform distribution assuming multiple lesU, lensV sampled 
 * parameterizations are also uniform. This code is adapted from one of the 
 * sampling methods presented in PBRT.
 */
void 
PerspectiveCamera::lensCoords(double lensU, double lensV,
                              double *lensDX, double *lensDY) const
{
   // To reduce error handle the origin before remapping to [-1, 1].
	if (lensU == 0.5 && lensV == 0.5) {
      /*
       * Normally should do a compare with epsilon but we are sampling and 
       * jittering anyway so we don't care.
       */
		*lensDX = 0.0;
		*lensDY = 0.0;
		return;
	}
   
   
   double r;
   double theta;
   
	/*
    * First convert the parameterization into numbers from [-1, 1]
    * (0,0) being the center of the lens.
    */
	double sx = 2 * lensU - 1;
	double sy = 2 * lensV - 1;
   
	/*
    * Must map the square parameterization to a parameterization that makes 
    * sense for a disk. (radius, theta).
    */
   
	if (sx >= -sy) {
		if (sx > sy) {
			// Handle first region of disk
			r = sx;
			if (sy > 0.0)
				theta = sy/r;
			else
				theta = 8.0 + sy/r;
		}
		else {
			// Handle second region of disk
			r = sy;
			theta = 2.0 - sx/r;
		}
	}
	else {
		if (sx <= sy) {
			// Handle third region of disk
			r = -sx;
			theta = 4.0 - sy/r;
		}
		else {
			// Handle fourth region of disk
			r = -sy;
			theta = 6.0 + sx/r;
		}
	}
   
   /*
    * Map to actual x,y values on the camera lens. So far the calculations 
    * were done on the unit disk.
    */
	theta *= M_PI / 4.0;
	*lensDX = r * cosf(theta) * mLensRadius;
	*lensDY = r * sinf(theta) * mLensRadius;
}
