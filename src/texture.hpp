

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "colour.hpp"
#include "image.hpp"
#include "primitive.hpp"

class Texture 
{
public:
   virtual ~Texture(){};
   
   virtual Colour evaluate(const PrimitivePatch &pPatch) const = 0;
   
protected:
   Texture(){};
};


// Useful for debugging... maps (u,v) -> Colour(u,v,0)
class UVTexture
: public Texture
{
public:
   UVTexture(){};
   ~UVTexture(){};
   
   Colour evaluate(const PrimitivePatch &pPatch) const;
   
private:
};


// Provides uniform colour throughout - simplest implementation.
class ConstantTexture
   : public Texture
{
public:
   ConstantTexture(const Colour &c = Colour(0,0,0));
   ~ConstantTexture(){};
   
   Colour evaluate(const PrimitivePatch &) const;
   
private:
   Colour mColour;
};


// Provides a checkerboard pattern alternating between 
class CheckerTexture
: public Texture
{
public:
   CheckerTexture(Texture *a = new ConstantTexture(Colour(0,0,0)),
                  Texture *b = new ConstantTexture(Colour(1,1,1)),
                  uint8_t frequency = 1);
   ~CheckerTexture(){};
   
   Colour evaluate(const PrimitivePatch &pPatch) const;
   
private:
   Texture *mTA;
   Texture *mTB;
   uint8_t freq;
};


// Evaluates the Colour at (u,v) using an Image for the lookup.
class ImageTexture
: public Texture
{
public:
   ImageTexture(Image *image = NULL);
   ~ImageTexture(){};
   
   Colour evaluate(const PrimitivePatch &pPatch) const;
   
private:
   Image *mImage;
   
   // Cached image values
   int width, height;
};

#endif // TEXTURE_HPP
