#include "./AmbientLight.h"

#include <cassert>

AmbientLight::AmbientLight(
  const float& colorR,
  const float& colorG,
  const float& colorB
) 
  :
  color(3,1,0.0f)
{

  assert(colorR >= 0.0f && colorR <= 1.0f);
  assert(colorG >= 0.0f && colorG <= 1.0f);
  assert(colorB >= 0.0f && colorB <= 1.0f);

  color.setR(colorR);
  color.setG(colorG);
  color.setB(colorB);

}

AmbientLight::AmbientLight(
  const MatrixFloat& color
)
  :
  color(3,1,0.0f)
{
  assert(color.getWidth() == 3);
  assert(color.getHeight() == 1);
  assert(color.getR() >= 0.0f && color.getR() <= 1.0f);
  assert(color.getG() >= 0.0f && color.getG() <= 1.0f);
  assert(color.getB() >= 0.0f && color.getB() <= 1.0f);

  this->color.assign(color);

}

void AmbientLight::computeLightIntensity(
  MatrixFloat& output
) const {
  assert(output.getWidth() == 3);
  assert(output.getHeight() == 1);
  output.assign(color);
}
