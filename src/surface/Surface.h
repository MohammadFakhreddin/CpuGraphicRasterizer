#ifndef BaseSurface_class
#define BaseSurface_class

#include <vector>
#include <memory>

#include "../data_types/MatrixTemplate.h"
#include "../data_types/VectorTemplate.h"
#include "../camera/Camera.h"
#include "../shaders/light/Light.h"
#include "../texture/Texture.h"

class Surface
{

protected:

  static constexpr unsigned long edgeCount = 3;

public:

  enum class LightPrecision {
    perPixel,
    perSurface
  };
  
  Surface(
    LightPrecision lightPrecision,
    std::unique_ptr<Texture>& texture,
    const unsigned long& edge1Index,
    const unsigned long& edge2Index,
    const unsigned long& edge3Index
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

  void update(
    Camera& cameraInstance,
    std::vector<MatrixFloat>& worldPoints,
    std::vector<MatrixFloat>& normals,
    std::vector<std::unique_ptr<Light>>& lightSources
  );

  const unsigned long& getEdgeByIndex(const unsigned short& index);

  const unsigned long& getNormalIndex(const unsigned short& index);

  bool areEdgeAndNormalsValid(
    const unsigned long& worldPointsSize,
    const unsigned long& normalsSize
  );

  virtual ~Surface() = default;

private:

  std::unique_ptr<Texture>& texture;

  unsigned long edgeIndices[edgeCount];

  unsigned long normalVectorIndices[edgeCount];

  MatrixFloat colorIntensity[edgeCount];

  Vec2DFloat textureCoordinate[edgeCount];
 
  void computePixelMapData(
    Camera& cameraInstance,
    std::vector<MatrixFloat>& worldPoints,
    std::vector<MatrixFloat>& normals,
    std::vector<std::unique_ptr<Light>>& lightSources
  );

  void calculateStepCount(
    Camera& cameraInstance,
    float difference,
    unsigned int* totalStepCount
  );

  void calculateStepValueBasedOnStepCount(
    const float& difference,
    const unsigned int& stepCount,
    float* stepValue
  );

  void drawLineBetweenPoints(

    Camera& cameraInstance,

    const float& triangleStartX,
    const float& triangleStartY,
    const float& triangleStartZ,

    const float& triangleEndX,
    const float& triangleEndY,
    const float& triangleEndZ,

    const float& textureStartX,
    const float& textureStartY,

    const float& textureEndX,
    const float& textureEndY,

    const float& lightColorStartR,
    const float& lightColorStartG,
    const float& lightColorStartB,

    const float& lightColorEndR,
    const float& lightColorEndG,
    const float& lightColorEndB,

    const float& normalStartX,
    const float& normalStartY,
    const float& normalStartZ,

    const float& normalEndX,
    const float& normalEndY,
    const float& normalEndZ,

    std::vector<std::unique_ptr<Light>>& lightSources

  );
  //TODO Write unit tests
  /**
   * 
   * Jump value in draw steps
   * 
  */
  //TODO It must be 1, Current value is cpu expensive
  static constexpr float drawStepValue = 0.5;
  /**
   * 
   * Checks if we need to render shape or not
   * 
   */
  bool isVisibleToCamera(
    Camera& cameraInstance,
    std::vector<MatrixFloat>& worldPoints,
    std::vector<MatrixFloat>& normals
  );
  /**
  *
  * Intensity of color based on light
  *
  */
  void computeColorIntensity(
    std::vector<MatrixFloat> & worldPoints,
    std::vector<MatrixFloat>& normals,
    std::vector<std::unique_ptr<Light>>& lightSources  
  );

  void computeColorIntensityForPoint(
    const MatrixFloat& worldPoint,
    const MatrixFloat& worldNormal,
    std::vector<std::unique_ptr<Light>>& lightSources,
    MatrixFloat& output
  );

  /*
  *
  * Vector from camera boundary to center of shape
  *
  */
  MatrixFloat cameraVector;

  /*
  *
  * Trmporary data
  * Because these variables are used in for loop we have to keep them 
  * 
  */
  double dotProductValue = 0;

  LightPrecision lightPercision;
  
  MatrixFloat colorIntensityOutput;

  MatrixFloat* currentWorldPoint = nullptr;

  MatrixFloat* currentNormal = nullptr;

  unsigned int cameraLeft = 0;
  
  unsigned int cameraRight = 0;
  
  unsigned int cameraTop = 0;
  
  unsigned int cameraBottom = 0;

  bool isShapeCompletlyOutOfCamera = false;

  float red = 0;

  float green = 0;
  
  float blue = 0;

  unsigned int lineTriangleTotalStepCount = 0;
  
  float lineTriangleXStepValue = 0.0f;
  
  float lineTriangleYStepValue = 0.0f;
  
  float lineTriangleZStepValue = 0.0f;

  float xDifference = 0.0f;

  float yDifference = 0.0f;

  float lineXDifference = 0.0f;

  float lineYDifference = 0.0f;

  float lineTextureXStepValue = 0.0f;

  float lineTextureYStepValue = 0.0f;

  float lineLightColorRStepValue = 0.0f;

  float lineLightColorGStepValue = 0.0f;

  float lineLightColorBStepValue = 0.0f;

  float lineTriangleStartX = 0.0f;

  float lineTriangleStartY = 0.0f;
  
  float lineTriangleStartZ = 0.0f;

  float lineTextureStartX = 0.0f;
  
  float lineTextureStartY = 0.0f;

  float lineLightColorStartR = 0.0f;
  
  float lineLightColorStartG = 0.0f;
  
  float lineLightColorStartB = 0.0f;

  float textureStartX = 0.0f;
  
  float textureStartY = 0.0f;

  float textureEndX = 0.0f;
  
  float textureEndY = 0.0f;

  float textureFinalX = 0.0f;
  
  float textureFinalY = 0.0f;

  float textureStartStepValueX = 0.0f;
  
  float textureStartStepValueY = 0.0f;

  float textureEndStepValueX = 0.0f;
  
  float textureEndStepValueY = 0.0f;

  float triangleStartX = 0.0f;
  
  float triangleStartY = 0.0f;
  
  float triangleStartZ = 0.0f;

  float triangleEndX = 0.0f;
  
  float triangleEndY = 0.0f;
  
  float triangleEndZ = 0.0f;

  float triangleFinalX = 0.0f;
  
  float triangleFinalY = 0.0f;
  
  float triangleFinalZ = 0.0f;

  unsigned int totalStepCount = 0;

  float triangleStartStepValueX = 0.0f;
  
  float triangleStartStepValueY = 0.0f;
  
  float triangleStartStepValueZ = 0.0f;

  float triangleEndStepValueX = 0.0f;
  
  float triangleEndStepValueY = 0.0f;
  
  float triangleEndStepValueZ = 0.0f;

  float normalStartX = 0.0f;

  float normalStartY = 0.0f;

  float normalStartZ = 0.0f;

  float normalEndX = 0.0f;

  float normalEndY = 0.0f;

  float normalEndZ = 0.0f;

  float normalFinalX = 0.0f;

  float normalFinalY = 0.0f;

  float normalFinalZ = 0.0f;

  float normalStartStepValueX = 0.0f;

  float normalStartStepValueY = 0.0f;

  float normalStartStepValueZ = 0.0f;

  float normalEndStepValueX = 0.0f;

  float normalEndStepValueY = 0.0f;

  float normalEndStepValueZ = 0.0f;

  float lineNormalStepValueX = 0.0f;

  float lineNormalStepValueY = 0.0f;

  float lineNormalStepValueZ = 0.0f;

  MatrixFloat lineNormalStartValue;

  float lightColorStartR = 0.0f;
  
  float lightColorStartG = 0.0f;
  
  float lightColorStartB = 0.0f;

  float lightColorEndR = 0.0f;
  
  float lightColorEndG = 0.0f;
  
  float lightColorEndB = 0.0f;

  float lightColorFinalR = 0.0f;
  
  float lightColorFinalG = 0.0f;
  
  float lightColorFinalB = 0.0f;

  float lightColorStartStepValueR = 0.0f;
  
  float lightColorStartStepValueG = 0.0f;
  
  float lightColorStartStepValueB = 0.0f;

  float lightColorEndStepValueR = 0.0f;
  
  float lightColorEndStepValueG = 0.0f;
  
  float lightColorEndStepValueB = 0.0f;

  MatrixFloat worldPointPlaceholder;

  MatrixFloat worldNormalPlaceholder;

};

#endif

