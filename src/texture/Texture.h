#ifndef FaTexture_class
#define FaTexture_class

#include <string>
#include <memory>

#include "./../data_types/FaObject.h"
#include "./../camera/Camera.h"

class Texture
{

public:

  virtual void getPixelForPosition(
    const float& x,
    const float& y,
    float *red,
    float *green,
    float *blue
  );

  /**
    For debugging texture
  */
  virtual void update(double deltaTime, Camera& cameraInstance);

public:
  
  virtual ~Texture();

};

#endif