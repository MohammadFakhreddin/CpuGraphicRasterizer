#ifndef BaseSurface_class
#define BaseSurface_class

#include <vector>

#include "../../../data_types/MatrixTemplate.h"
#include "../../../data_types/VectorTemplate.h"
#include "../../../camera/Camera.h"

enum EdgeType{
  color,
  simple,
  texture,
  base
};

class BaseSurface
{
public:

  void render(
    Camera& cameraInstance,
    std::vector<MatrixFloat>* worldPoints,
    float shapeCenterX,
    float shapeCenterY,
    float shapeCenterZ
  );

  virtual EdgeType getEdgeType();

  int getEdgeByIndex(int index);

  bool areEdgesDataValid(int nodesSize);

protected:

  int edge1;
  int edge2;
  int edge3;

  virtual void computePixelMapData(
    Camera& cameraInstance,
    std::vector<MatrixFloat>* worldPoints
  );

  void putPixelInMap(
    Camera& cameraInstance,
    int x,
    int y,
    float zValue,
    float red,
    float green,
    float blue
  );
  
  void calculateStepCountAndStepValue(
    Camera& cameraInstance,
    float difference,
    unsigned int* totalStepCount,
    float* stepValue
  );

private:
  /**
   * 
   * Making this value to true logs all debug data about edges
   * 
  */
  static constexpr bool DEBUG_MODE = false;
  /**
   * 
   * Jump value in draw steps
   * 
  */
  static constexpr float drawStepValue = 0.5;
  /**
   * 
   * Checks if we need to render shape or not
   * 
   */
  bool isVisibleToCamera(
    Camera& cameraInstance,
    std::vector<MatrixFloat>* worldPoints
  );
  void computeNormalVector(
    std::vector<MatrixFloat>* worldPoints
  );
  void computeEdgeCenter(
    std::vector<MatrixFloat>* worldPoints
  );
  /*
  *
  * Vector from camera boundary to center of shape
  *
  */
  Vec3DFloat cameraVector = Vec3DFloat(0.0f,0.0f,0.0f);
  /**
   * 
   * We use normal vector for diffuse light intensity and check if surface is visible to camera
   *  
   * 
   * */
  Vec3DFloat normalVector = Vec3DFloat(0.0f,0.0f,0.0f);
  /*
   * 
   * Based on world points we create this vector for normal vector computation
   * 
   * */
  Vec3DFloat edge1To2Vector = Vec3DFloat(0.0f,0.0f,0.0f);
  /**
   * 
   * Based on world points we create this vector for normal vector computation
   * 
   * */
  Vec3DFloat edge2To3Vector = Vec3DFloat(0.0f,0.0f,0.0f);
  /**
   * 
   * Center of surface
   * 
  */
  Vec3DFloat edgeCenter = Vec3DFloat(0.0f,0.0f,0.0f);
  /*
  *
  * Vector from camera to center of shape to check if edge is visible in camera
  * 
  */
  Vec3DFloat edgeCenterToPolygonCenterPoint = Vec3DFloat(0.0f,0.0f,0.0f);
  /*
  *
  * Value of dot product stores here to avoid duplicate data storing
  * 
  */
  float dotProductValue = 0;
  /*
  * We use this element to check if we need to reverse our dot products result due to
  * Normal vector being reverse
  */
  Vec3DFloat colorIntensity = Vec3DFloat(0.0f,0.0f,0.0f);  
};

#endif