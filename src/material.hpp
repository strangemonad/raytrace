
#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "colour.hpp"
#include "primitive.hpp"
#include "texture.hpp"

class Material 
{
public:
   virtual ~Material(){};
   
   /*
    * This API to go along with the Phong lighting model.
    */
   virtual void evaluate(const PrimitivePatch &pPatch,
                         Colour *kDiffuse = NULL, 
                         Colour *kSpecular = NULL, 
                         double *specCoeff = NULL,
                         double *reflectivity = NULL,
                         double *opacity = NULL,
                         double *indexOfRefraction = NULL) const = 0;
   
   virtual double getOpacityApprox() const = 0;
   
   void setBumpMap(Texture *bumpTexture);
   void evaluateBumpMap(const PrimitivePatch &pPatch,
                        PrimitivePatch *bumpedPatch) const;
   
protected:
   Material() : mBumpMap(NULL){};
   
   Texture *mBumpMap;
};



/*
 * Reflective and Transmissive material.
 * This is done very naively by specifying a constant reflectivity factor
 * to control how much light the material reflects and opacity factor to control 
 * how much light the material allows through it. In addition an index of 
 * refraction for the material is also speficied to allow ray deviation 
 * calculations to be performed according to Snell's law.
 */
class RTMaterial 
: public Material 
{
   
public:
   RTMaterial(const Texture *tDiffuse,
              const Colour& kSpecular = Colour(0, 0, 0),
              double shininess = 0.0,
              double reflectivity = 0.0,
              double opacity = 1.0,
              double indexOfRefraction = 1.0);
   virtual ~RTMaterial(){};
   
   virtual void evaluate(const PrimitivePatch &pPatch,
                         Colour *kDiffuse = NULL, 
                         Colour *kSpecular = NULL, 
                         double *specCoeff = NULL,
                         double *reflectivity = NULL,
                         double *opacity = NULL,
                         double *indexOfRefraction = NULL) const;
   
   virtual double getOpacityApprox() const;
   
private:
   const Texture &mTDiffuse;
   Colour mKSpecular;
   
   double mShininess;
   
   double mReflectivity;
   double mOpacity;
   double mIndexOfRefraction;
};

#endif // MATERIAL_HPP
