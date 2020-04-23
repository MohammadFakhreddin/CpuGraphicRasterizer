#ifndef ColorTexture_Class
#define ColorTexture_Class

#include "../Texture.h"

class ColorTexture : public Texture {

public:

  ColorTexture(
    const float& red,
    const float& green,
    const float& blue
  );

  ColorTexture(
    const Matrix3X1Float& color
  );

  void getPixelForPosition(
    const float& x,
    const float& y,
    float* red,
    float* green,
    float* blue
  ) const;

private:

  float red;
  float green;
  float blue;

};

#endif // !ColorTexture_Class
