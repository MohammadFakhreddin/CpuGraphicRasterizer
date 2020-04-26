
#ifndef LineMemoryPool_Class
#define LineMemoryPool_Class

#include "../../data_types/MatrixTemplate.h"
#include "../../texture/Texture.h"

class LineMomoryPool {

public:

  Matrix4X1Float lineStart;

  Matrix4X1Float lineStepValue;

  Matrix2X1Float textureStart;

  Matrix2X1Float textureStepValue;

  float xDifference = 0.0f;

  float yDifference = 0.0f;

  unsigned int totalStepCount = 0;

  Matrix4X1Float colorStart;

  Matrix4X1Float colorStepValue;

  Matrix4X1Float normalStart;

  Matrix4X1Float normalStepValue;

  float red = 0;
  
  float green = 0;

  float blue = 0;

  Matrix4X1Float perPixelNormalHat;

  Matrix4X1Float perPixelColorIntensity;

  //TODO Create memory pool and use that instead of this trash
  Matrix4X1Float lightVectorPlaceholder;

  Matrix4X1Float lightVectorHatPlaceholder;

  Matrix4X1Float lightReflectionPlaceholder;

  Matrix4X1Float lightReflectionHatPlaceholder;

  Matrix4X1Float cameraVectorPlaceholder;

  Matrix4X1Float cameraVectorHatPlaceholder;

  Matrix4X1Float colorOutputPlaceholder;

};

#endif // !LineAssembler_Class
