#ifndef BaseSurface_class
#define BaseSurface_class

#include <vector>
#include <memory>

#include "../../Constants.h"
#include "../../texture/Texture.h"
#include "TriangleMemoryPool.h"
#include "LineMemoryPool.h"

class Surface
{

public:
  
  Surface(
    const Constants::LightPrecision lightPrecision,
    Texture* texture,
    const unsigned long edgeIndex[3],
    const float specularIntensity
  );

  void setNormalIndex(
    const short& edgeNumber, 
    const unsigned long& index
  );

  void setTextureCoordinates(
    const short& edgeNumber, 
    const float& x, 
    const float& y
  );

  const unsigned long& getEdgeByIndex(const unsigned short& index);

  const unsigned long& getNormalIndex(const unsigned short& index);

  bool areEdgeAndNormalsValid(
    const unsigned long& worldPointsSize,
    const unsigned long& normalSize
  );

  virtual ~Surface() = default;
 
  Texture* texture;

  unsigned long edgeIndices[3];

  unsigned long normalIndices[3];

  MatrixFloat colorIntensity[3];

  MatrixFloat textureCoordinate[3];

  float specularIntensity;

  Constants::LightPrecision lightPrecision;

  MatrixFloat cameraVectorPlaceholder;

  TriangleMemoryPool triangleMemoryPool;

  LineMomoryPool lineMemoryPool;

};

#endif

