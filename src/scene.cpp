/*
 *  scene.cpp
 *  A4
 *
 *  Contains a view independent representation of a scene. This can be thought
 *  as the data model of a scene to render.
 *
 *  View specifics of a particular render are specified by Camera and the size
 *  requested when triggering the render.
 *
 *
 *  Created by Shawn Morel on 6/24/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */

#include <iostream>

#include "bBox.hpp"
#include "colour.hpp"
#include "defs.hpp"
#include "light.hpp"
#include "scene.hpp"
#include "sceneNode.hpp"
#include "vector.hpp"


/*
 *------------------------------------------------------------------------------
 * 
 * Scene::Scene --
 *     
 *     Constructor.
 *
 *------------------------------------------------------------------------------
 */

Scene::Scene(SceneNode& root,           // IN
             std::list<Light*>& lights, // IN
             Colour ambientColour)     // IN
   : mRoot(root),
     mLights(lights),
     mAmbientColour(ambientColour)
{
   if (mAmbientColour.isBlack()) {
      /*
       * Take 0.1 % of all the lights (weighted by colour) to compose the 
       * ambient light. This is a nasty hack and might have to be changed to 
       * have more accurate lighting in the project. What's nice is that 
       * someone can pass in a more appropriate ambient colour (maybe based on
       * average distance of each light source to all objects or some better
       * physical model).
       */
      
      for(std::list<Light*>::const_iterator lightIter = mLights.begin();
          lightIter != mLights.end();
          lightIter++) {
         
         Light &curLight = **lightIter;
         
         // XXX Colour needs a += opperation for efficiency
         mAmbientColour = mAmbientColour + (0.05 * curLight.getColour());
      }   
   }
}


/*
 *------------------------------------------------------------------------------
 * 
 * Scene::Scene --
 *     
 *     Copy Constructor.
 *
 *------------------------------------------------------------------------------
 */

Scene::Scene(const Scene& otherScene) // IN
   : mRoot(otherScene.mRoot),
     mLights(otherScene.mLights),
     mAmbientColour(otherScene.mAmbientColour)
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * Scene::operator = --
 *     
 *     Assignment operator.
 *
 *------------------------------------------------------------------------------
 */

Scene& 
Scene::operator =(const Scene& otherScene) // IN
{
   mRoot = otherScene.mRoot;
   mLights = otherScene.mLights;
   mAmbientColour = otherScene.mAmbientColour;
   
   return *this;
}


/*
 *------------------------------------------------------------------------------
 * 
 * Scene::~Scene --
 *     
 *     Destructor.
 *
 *------------------------------------------------------------------------------
 */

Scene::~Scene()
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * Scene::prepare -- 
 *     
 *     A scene is considered immutable while rendering. Prepare will be called
 *     immediately before rendering the scene and allow things to be cached.
 *
 *------------------------------------------------------------------------------
 */

void
Scene::prepare()
{
   /*
    * Preparetion of a hierarchical scene must be done as a depth first 
    * (post order) traversal.
    */
   // The identity matrix. Each SceneNode will stack on its relative transform.
   Transform t;
   mRoot.prepare(t);
}


/*
 *------------------------------------------------------------------------------
 * 
 * Scene::getAmbient --
 *     
 *     Get the ambient component.
 *
 *------------------------------------------------------------------------------
 */

Colour
Scene::getAmbient() const
{
   return mAmbientColour;
}


/*
 *------------------------------------------------------------------------------
 * 
 * Scene::intersect --
 *     
 *     Simply delegates the call to the root of the scene.
 *
 *------------------------------------------------------------------------------
 */

bool 
Scene::intersect(const Ray &r, 
                 NodeIntersection *nIntersection) const
{
   return mRoot.intersect(r, nIntersection);
}


/*
 *------------------------------------------------------------------------------
 * 
 * Scene::intersect --
 *     
 *     Simply delegates the call to the root of the scene.
 *
 *------------------------------------------------------------------------------
 */

bool 
Scene::intersects(const Ray &r, double *transmissiveness) const
{
   return mRoot.intersects(r, transmissiveness);
}


/*
 *------------------------------------------------------------------------------
 * 
 * Scene::getColourAtIntersection --
 *     
 *     Calculates the resultant colour for the given intersection applying 
 *     specular highlights, transmission, reflection (if applicable) etc.
 *
 *------------------------------------------------------------------------------
 */

Colour 
Scene::getColourAtIntersection(const Ray &r,
                               const NodeIntersection &ni,
                               const Colour &cEnvironment,
                               uint8_t rayRecursion) const
{   
   /*
    * Do a simple variation on the phong lighting model. This could be improved.
    *
    * In my ray tracer the Colour class is analogous to a spectral distribution.
    * (ie: light flux)
    */
   
   /*
    * Evaluation of material properties at this intersection.
    */
   Colour cDiffuse;
   Colour cSpecular;
   double phongCoeff = 0.0;
   double reflectivity = 0.0;
   double opacity = 1.0;
   double indexOfRefraction = 1.0;
   
   PrimitivePatch bumpedPatch = ni.pPatch;
   if (ni.mat) {
      ni.mat->evaluate(ni.pPatch, 
                       &cDiffuse, 
                       &cSpecular, 
                       &phongCoeff,
                       &reflectivity,
                       &opacity,
                       &indexOfRefraction);
      ni.mat->evaluateBumpMap(ni.pPatch, &bumpedPatch);
   }
   
   
   /*
    * Calculate the lighting.
    */
   
   // XXX need to calculate light attenuation due to distance.
   Colour diffuseLight;
   Colour specularLight;
   for(std::list<Light*>::const_iterator lightIter = mLights.begin();
       lightIter != mLights.end();
       lightIter++) {
      
      Light &curLight = **lightIter;
      double intersectionTransmissiveness = 0.0;
      Ray patchToCurLightRay(bumpedPatch.p, 
                             normalize(curLight.getPosition() - bumpedPatch.p));
      
      /*
       * Check if there are any obstructions.
       */
      if (!intersects(patchToCurLightRay, &intersectionTransmissiveness) || 
          !isZero(intersectionTransmissiveness)) {
         // Rely on short-circuit || to test intersects && intersectionTransmissiveness > 0.0
         
         // Diffuse - cosine weighted angle between normal and light source
         double diffuseWeight = dot(bumpedPatch.shadingNorm, patchToCurLightRay.d);
         if (lessThanZero(diffuseWeight)) {
            diffuseWeight = 0.0;
         }
         diffuseLight += (diffuseWeight 
                          //* intersectionTransmissiveness 
                          * curLight.getColour());
         
         /*
          * Specular - exponential of cosine weighted angle between the incoming
          * ray and the reflected ray from the light source.
          */
         Vector reflected = normalize(computeReflection(-patchToCurLightRay.d, 
                                                        bumpedPatch));
         double specularWeight = dot(reflected, normalize(-r.d));
         if (lessThanZero(specularWeight)) {
            specularWeight = 0.0;
         }
         specularLight += (pow(specularWeight, phongCoeff) * 
                           curLight.getColour());
      }
   }
   
   
   /*
    * Recursive case.
    */
   if (rayRecursion > 0) {
      /*
       * Check to see if the material is reflective or transmissive. If it is
       * this will directly affect the diffuse Colour before it is used in the
       * final colour computation.
       */
      Ray recR;
      NodeIntersection recNI;
      Colour recColour;
         
      if (reflectivity > 0.0) {
         recR = Ray(bumpedPatch.p, computeReflection(r.d, bumpedPatch));
         if (intersect(recR, &recNI)) {
            recColour = getColourAtIntersection(recR, recNI, 
                                                cEnvironment, rayRecursion - 1);
            
            // Do a simple Alpha composite.
            cDiffuse = (cDiffuse * (1-reflectivity)) + (recColour * reflectivity);
         } else {
            // Didn't intersect with anything so take the env colour.
            cDiffuse = (cDiffuse * (1-reflectivity)) + (cEnvironment * reflectivity);
         }
         
      }
      
      if (opacity < 1.0) {
         // XXX need to work out the refraction using Snell's law.
         recR = Ray(bumpedPatch.p, r.d);
         if (intersect(recR, &recNI)) {
            
            recColour = getColourAtIntersection(recR, recNI, 
                                                cEnvironment, rayRecursion - 1);
            // Do a simple Alpha composite.
            cDiffuse = (cDiffuse * opacity) + (recColour * (1 - opacity));
         } else {
            // Didn't intersect with anything so take the env colour.
            cDiffuse = (cDiffuse * opacity) + (cEnvironment * (1 - opacity));
         }
      }
   }
   
   
   /*
    * Base Case.
    */
      
   
   
   /*
    * Apply lighting calculations onto the diffuse and specular lighting 
    * components of the material evaluated at the curent intersection point.
    */
   
   Colour c = getAmbient(); 
   // XXX need to multiply emmissive component if the object is a light source.
   
   if (!cDiffuse.isBlack()) {
      c += cDiffuse * diffuseLight;
   }
   
   // Need to calculate attenuation from distance of the light
   if (!cSpecular.isBlack()) {
      c += cSpecular * specularLight;
   }
   
   return c;
}


/*
 * Computes a perfectly reflected ray starting at pPatch.p given that rIncident
 * is the incident ray that intersected a primitive at pPatch.
 *
 * The angle between rIncident.d and pPatch.shadingNorm will equal the angle between the
 * patch normal and the reflected ray.
 */
 
Vector 
Scene::computeReflection(const Vector &incident,
                         const PrimitivePatch &pPatch) const
{
   Vector i = - incident;
   
   /*
    * Calculate the components of i projected onto (in the direction of) the 
    * pPatch normal and dpdu, dpdv vectors that span the tangent plane and the 
    * point of intersection.
    */
   
   // Need to pay special attention to incident rays close to 90 degrees to N
   double amountN = dot(i, pPatch.shadingNorm);
   /*if (lessThanZero(amountN)) {
      amountN = 0.0;
   }*/
   Vector iProjN = Vector(amountN * pPatch.shadingNorm); // |shsadingNorm| = 1 always
   Vector iProjU = dot(i, pPatch.dpdu) / pPatch.dpdu.lengthSquared() * pPatch.dpdu;
   Vector iProjV = dot(i, pPatch.dpdv) / pPatch.dpdv.lengthSquared() * pPatch.dpdv;
   
   /*
    * Create and return a ray that starts at the pPatch and is directed in the 
    * same N direction but oposite U and V directions.
    */
   return iProjN - iProjU - iProjV;
}
