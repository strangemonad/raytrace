#ifndef CS488_IMAGE_HPP
#define CS488_IMAGE_HPP

#include <string>

#include "colour.hpp"

/*
 * An image, consisting of a rectangle of floating-point elements.
 * This class makes it easy to read PNG data and the like from
 * files.
 * Note that colours in the range [0.0, 1.0] are mapped linearly to the integer
 * range [0, 255] when writing and reading PNG files.
 */
class Image 
{
public:
   Image(); ///< Construct an empty image
   
   // XXX needs to become uint.
   Image(int width, int height, int components); ///< Construct a black
                                                 ///image at the given width/height/depth
   Image(const Image& other); ///< Copy an image
   
   ~Image();
   
   Image& operator=(const Image& other); ///< Copy the data from
                                         ///one image to another
   
   int width() const; ///< Determine the width of the image
   int height() const; ///< Determine the height of the image
   int elements() const; ///< Determine the depth (doubles per pixel) of
                         ///the image
   
   // XXX need to make i an enum.
   double operator()(int x, int y, int i) const; ///< Retrieve a
                                                 ///particular component
                                                 ///from the image.
   double& operator()(int x, int y, int i);  ///< Retrieve a
                                             ///particular component
                                             ///from the image.
   
   Colour getColour(int x, int y);
   void setColour(const Colour &c, int x, int y);
   void addColour(const Colour &c, int x, int y);
   
   
   bool loadPng(const std::string& filename); ///< Load a PNG file into
                                              /// this image.
   
   bool savePng(const std::string& filename) const; ///< Save this image into
                                                    ///  the given PNG file
   
   const double* data() const;
   double* data();
   
private:
   int m_width, m_height;
   int m_elements;
   double* m_data;
};

#endif
