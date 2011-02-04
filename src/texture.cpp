
#include "algebra.hpp"
#include "image.hpp"
#include "texture.hpp"


Colour 
UVTexture::evaluate(const PrimitivePatch &pPatch) const
{
   return Colour(pPatch.u, pPatch.v, 0.0);
}



ConstantTexture::ConstantTexture(const Colour &c)
: mColour(c)
{
}
Colour 
ConstantTexture::evaluate(const PrimitivePatch &) const
{
   return mColour;
}



CheckerTexture::CheckerTexture(Texture *a,
                               Texture *b,
                               uint8_t frequency)
: mTA(a),
mTB(b),
freq(frequency)
{
}
Colour 
CheckerTexture::evaluate(const PrimitivePatch &pPatch) const
{
   double u = (pPatch.u * freq);
   double v = (pPatch.v * freq);

   PrimitivePatch pp = PrimitivePatch(pPatch);
   pp.u = u - floor(u);
   pp.v = v - floor(v);

   if (((int)floor(u) + (int)floor(v)) % 2 == 0) {
      return mTA->evaluate(pp);
   }
   
   return mTB->evaluate(pp);
}



ImageTexture::ImageTexture(Image *image)
: mImage(image),
width(mImage->width()),
height(mImage->height())
{
}
Colour 
ImageTexture::evaluate(const PrimitivePatch &pPatch) const
{
   // must flip the y coord for image lookup.
   return mImage->getColour((int)(pPatch.u * width), (int)((1-pPatch.v) * height));
}