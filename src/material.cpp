
#include "defs.hpp"
#include "material.hpp"
#include "texture.hpp"


void 
Material::setBumpMap(Texture *bumpTexture)
{
   mBumpMap = bumpTexture;
}


void 
Material::evaluateBumpMap(const PrimitivePatch &pPatch,
                          PrimitivePatch *bumpedPatch) const
{
   ASSERT(bumpedPatch);
   
   if (!mBumpMap) {
      *bumpedPatch = pPatch;
      return;
   }
   
   /*
    * The final bump mapping calculation results in finding a "new" point of 
    * intersection (and hence a pPatch normal and all if we want to properly  
    * calculate shading). The point is found by taking the original point and  
    * adding a displacement value. The displacement value depends on the orginal 
    * shading normal of the pPatch and some displacement function.
    *
    * The shading normal is determined by the cross product of the 2 partial 
    * derivatives dp/du and dp/dv. This allows any primitive type to be 
    * generically bump mapped. Calculation thus boils down to finding the new
    * partial derivatives at point p' (dp'/du and dp'/dv).
    *
    * most of the information needed we already have but finding the partial 
    * derivative in the u and v directions involves finding the partial 
    * derivative of the displacement function. Our displacement function D is 
    * specified by a Texture that we will treat as greyscale (reading the red 
    * channel only). We obviously can't determine infinitesmally small changes
    * so instead we will approximate dD/du and dD/dv by evaluting the texture
    * at D(u,v) = texture(u,v),  
    * dD/du(u,v) = texture(u + deltaU, v) - texture(u,v), and 
    * dD/dv(u,v) = (u, v + deltaV) - texture(u,v).
    * where deltaU and deltaV are small finite displacements. This is in line 
    * with the limit definition of a derivative.
    */
    
   PrimitivePatch jitteredPatch = pPatch;
   
   double D = mBumpMap->evaluate(pPatch).red();
   
      
   /* .01 is an approximation of a good small displacement. To make this better
    * we should determine the sampling frequency of the ray that generated this
    * intersection patch and adjust the jittering accordingly. This can still
    * be addressed in a brute force way by casting more rays.
    */
	double delta = .01;
   
   /*
    * First jitter the texture in u and evaluate.
    */
	jitteredPatch.p += (delta * pPatch.dpdu);
	jitteredPatch.u += delta;
	jitteredPatch.geomNorm = normalize(pPatch.shadingNorm + 
                                      Normal(delta * pPatch.dndu));
   
   double dDdu = mBumpMap->evaluate(jitteredPatch).red();
   
   /*
    * Rinse and repeat in the v direction.
    */
   jitteredPatch.p += (delta * pPatch.dpdv);
	jitteredPatch.u = pPatch.u;
   jitteredPatch.v += delta;
	jitteredPatch.geomNorm = normalize(pPatch.shadingNorm + 
                                             Normal(delta * pPatch.dndv));
	
   double dDdv = mBumpMap->evaluate(jitteredPatch).red();
   
   
	// We now have all the info we need to compute the displacement.
   Vector dpdu = pPatch.dpdu +
                 (((dDdu - D) / delta) * Vector(pPatch.geomNorm)) + 
                 (D * Vector(pPatch.dndu));
   Vector dpdv = pPatch.dpdv + 
                 (((dDdv - D) / delta) * Vector(pPatch.geomNorm)) + 
                 (D * Vector(pPatch.dndv));
   *bumpedPatch = PrimitivePatch(pPatch.p,
                                 pPatch.u,
                                 pPatch.v,
                                 dpdu,
                                 dpdv,
                                 // dn has actually changed because of the bump
                                 // mapping but it doesn't really matter and 
                                 // would cost a lot to recalculate.
                                 pPatch.dndu,  
                                 pPatch.dndv,
                                 pPatch.primitive,
                                 false);
	
   // Need to keep the shading normals oriented in the same direction.
   // XXX come back and shift so that the displacement is in the right direction.
	if (lessThanZero(dot(pPatch.shadingNorm, bumpedPatch->shadingNorm))) {
      bumpedPatch->shadingNorm = - bumpedPatch->shadingNorm;
   }
}


RTMaterial::RTMaterial(const Texture *tDiffuse, 
                       const Colour &kSpecular, 
                       double shininess,
                       double reflectivity,
                       double opacity,
                       double indexOfRefraction)
  : mTDiffuse(*tDiffuse),
    mKSpecular(kSpecular),
    mShininess(shininess),
    mReflectivity(reflectivity),
    mOpacity(opacity),
    mIndexOfRefraction(indexOfRefraction)
{
}



void
RTMaterial::evaluate(const PrimitivePatch &pPatch,
                     Colour *kDiffuse, 
                     Colour *kSpecular, 
                     double *specCoeff,
                     double *reflectivity,
                     double *opacity,
                     double *indexOfRefraction) const
{
   if (kDiffuse) {
      *kDiffuse = mTDiffuse.evaluate(pPatch);
   }
   
   if (kSpecular) {
      *kSpecular = mKSpecular;
   }
   
   if (specCoeff) {
      *specCoeff = mShininess;
   }
   
   if (reflectivity) {
      *reflectivity = mReflectivity;
   }

   if (opacity) {
      *opacity = mOpacity;
   }
   
   if (reflectivity) {
      *reflectivity = mReflectivity;
   }
   
   if (indexOfRefraction) {
      *indexOfRefraction = mIndexOfRefraction;
   }
}


double 
RTMaterial::getOpacityApprox() const
{
   return mOpacity;
}

