#include "./ColorTexture.h"

ColorTexture::ColorTexture(
  const MatrixFloat& color
)
  :
  ColorTexture(color.getR(), color.getG(), color.getB())
{}

ColorTexture::ColorTexture(
  const float& red,
  const float& green,
  const float& blue
)
  :
  red(red),
  green(green),
  blue(blue)
{
  assert(red >= 0 && red <= 1);
  assert(green >= 0 && green <= 1);
  assert(blue >= 0 && blue <= 1);
}

void ColorTexture::getPixelForPosition(
  const float& x,
  const float& y,
  float* red,
  float* green,
  float* blue
) const
{
  *red = this->red;
  *green = this->green;
  *blue = this->blue;
}