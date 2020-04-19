
#ifndef LineMemoryPool_Class
#define LineMemoryPool_Class

#include "../../data_types/MatrixTemplate.h"
#include "../../texture/Texture.h"

class LineMomoryPool {

public:
  
  LineMomoryPool()
    :
    lineStart(3, 1, 0.0f),
    lineStepValue(3, 1, 0.0f),
    textureStart(2, 1, 0.0f),
    textureStepValue(2, 1, 0.0f),
    colorStart(3, 1, 0.0f),
    colorStepValue(3, 1, 0.0f),
    normalStart(3, 1, 0.0f),
    normalStepValue(3, 1, 0.0f),
    lightReflectionPlaceholder(3, 1, 0.0f),
    lightReflectionHatPlaceholder(3, 1, 0.0f),
    lightVectorPlaceholder(3, 1, 0.0f),
    lightVectorHatPlaceholder(3, 1, 0.0f),
    cameraVectorPlaceholder(3, 1, 0.0f),
    cameraVectorHatPlaceholder(3, 1, 0.0f)
  {};

  MatrixFloat lineStart;

  MatrixFloat lineStepValue;

  MatrixFloat textureStart;

  MatrixFloat textureStepValue;

  float xDifference = 0.0f;

  float yDifference = 0.0f;

  unsigned int totalStepCount = 0;

  MatrixFloat colorStart;

  MatrixFloat colorStepValue;

  MatrixFloat normalStart;

  MatrixFloat normalStepValue;

  float red = 0;
  
  float green = 0;

  float blue = 0;

  MatrixFloat perPixelNormalHat;

  MatrixFloat perPixelColorIntensity;

  //TODO Create memory pool and use that instead of this trash
  MatrixFloat lightVectorPlaceholder;

  MatrixFloat lightVectorHatPlaceholder;

  MatrixFloat lightReflectionPlaceholder;

  MatrixFloat lightReflectionHatPlaceholder;

  MatrixFloat cameraVectorPlaceholder;

  MatrixFloat cameraVectorHatPlaceholder;

  MatrixFloat colorOutputPlaceholder;

};

#endif // !LineAssembler_Class
