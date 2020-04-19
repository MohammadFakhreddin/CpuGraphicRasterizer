#ifndef TriangleMemoryPool_Class
#define TriangleMemoryPool_Class

#include <vector>

#include "../../camera/Camera.h"
#include "../../texture/Texture.h"

class TriangleMemoryPool{

public:

  TriangleMemoryPool() 
    :
    textureStart(2, 1, 0.0f),
    textureEnd(2, 1, 0.0f),
    textureFinal(2, 1, 0.0f),
    textureStartStepValue(2, 1, 0.0f),
    textureEndStepValue(2, 1, 0.0f),
    triangleStart(3, 1, 0.0f),
    triangleEnd(3, 1, 0.0f),
    triangleFinal(3, 1, 0.0f),
    triangleStartStepValue(3, 1, 0.0f),
    triangleEndStepValue(3, 1, 0.0f),
    normalStart(3, 1, 0.0f),
    normalEnd(3, 1, 0.0f),
    normalFinal(3, 1, 0.0f),
    normalStartStepValue(3, 1, 0.0f),
    normalEndStepValue(3, 1, 0.0f),
    colorStart(3, 1, 0.0f),
    colorEnd(3, 1, 0.0f),
    colorFinal(3, 1, 0.0f),
    lightReflectionPlaceholder(3, 1, 0.0f),
    lightReflectionHatPlaceholder(3, 1, 0.0f),
    lightVectorPlaceholder(3, 1, 0.0f),
    lightVectorHatPlaceholder(3, 1, 0.0f),
    cameraVectorPlaceholder(3, 1, 0.0f),
    cameraVectorHatPlaceholder(3, 1, 0.0f)
  {};

  MatrixFloat triangleStart;

  MatrixFloat triangleEnd;

  MatrixFloat triangleFinal;

  unsigned int totalStepCount = 0;

  MatrixFloat triangleStartStepValue;

  MatrixFloat triangleEndStepValue;

  MatrixFloat normalStart;

  MatrixFloat normalEnd;

  MatrixFloat normalFinal;

  MatrixFloat normalStartStepValue;

  MatrixFloat normalEndStepValue;

  MatrixFloat textureStart;

  MatrixFloat textureEnd;

  MatrixFloat textureFinal;

  MatrixFloat textureStartStepValue;

  MatrixFloat textureEndStepValue;

  MatrixFloat colorStart;

  MatrixFloat colorEnd;

  MatrixFloat colorFinal;

  MatrixFloat colorStartStepValue;

  MatrixFloat colorEndStepValue;

  float xDifference = 0.0f;

  float yDifference = 0.0f;

  //TODO Create memory pool and use that instead of this trash
  MatrixFloat lightVectorPlaceholder;

  MatrixFloat lightVectorHatPlaceholder;

  MatrixFloat lightReflectionPlaceholder;

  MatrixFloat lightReflectionHatPlaceholder;

  MatrixFloat cameraVectorPlaceholder;

  MatrixFloat cameraVectorHatPlaceholder;

  MatrixFloat colorOutputPlaceholder;

};

#endif // !TriangleAssembler
