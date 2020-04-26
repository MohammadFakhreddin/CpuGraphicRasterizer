#ifndef TriangleMemoryPool_Class
#define TriangleMemoryPool_Class

#include <vector>

#include "../../camera/Camera.h"
#include "../../texture/Texture.h"

class TriangleMemoryPool{

public:

  Matrix4X1Float triangleStart;

  Matrix4X1Float triangleEnd;

  Matrix4X1Float triangleFinal;

  unsigned int totalStepCount = 0;

  Matrix4X1Float triangleStartStepValue;

  Matrix4X1Float triangleEndStepValue;

  Matrix4X1Float normalStart;

  Matrix4X1Float normalEnd;

  Matrix4X1Float normalFinal;

  Matrix4X1Float normalStartStepValue;

  Matrix4X1Float normalEndStepValue;

  Matrix2X1Float textureStart;

  Matrix2X1Float textureEnd;

  Matrix2X1Float textureFinal;

  Matrix2X1Float textureStartStepValue;

  Matrix2X1Float textureEndStepValue;

  Matrix4X1Float colorStart;

  Matrix4X1Float colorEnd;

  Matrix4X1Float colorFinal;

  Matrix4X1Float colorStartStepValue;

  Matrix4X1Float colorEndStepValue;

  float xDifference = 0.0f;

  float yDifference = 0.0f;

  //TODO Create memory pool and use that instead of this trash
  Matrix4X1Float lightVectorPlaceholder;

  Matrix4X1Float lightVectorHatPlaceholder;

  Matrix4X1Float lightReflectionPlaceholder;

  Matrix4X1Float lightReflectionHatPlaceholder;

  Matrix4X1Float cameraVectorPlaceholder;

  Matrix4X1Float cameraVectorHatPlaceholder;

  Matrix4X1Float colorOutputPlaceholder;

};

#endif // !TriangleAssembler
