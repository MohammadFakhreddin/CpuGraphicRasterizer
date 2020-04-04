#include "../Texture.h"

class ColorTexture : public Texture {

public:

  ColorTexture(
    const float& red,
    const float& green,
    const float& blue
    );

  void getPixelForPosition(
    const float& x,
    const float& y,
    float* red,
    float* green,
    float* blue
    ) override;

private:

  float red;
  float green;
  float blue;

};