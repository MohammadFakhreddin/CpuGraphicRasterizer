#include "./TextureEdge.h"
#include <cassert>
#include <cmath>
#include "../../../application/Application.h"
#include "./../../../utils/log/Logger.h"
#include <string>

bool DEBUG_MODE = false;

TextureEdge::TextureEdge(
  int edge1,
  int edge2,
  int edge3,
  std::unique_ptr<FaTexture>& textureReference,
  Vec2DFloat edge1TexturePoint,
  Vec2DFloat edge2TexturePoint,
  Vec2DFloat edge3TexturePoint
)
:
textureReference(textureReference),
edge1TexturePoint(edge1TexturePoint),
edge2TexturePoint(edge2TexturePoint),
edge3TexturePoint(edge3TexturePoint)
{
  this->edge1 = edge1;
  this->edge2 = edge2;
  this->edge3 = edge3;
  if(DEBUG_MODE){
    Logger::log("virtualImageAddress:"+textureReference->getAddress());
    Logger::log("virtualImageWidth:"+std::to_string(textureReference->getVirtualImageWidth()));
    Logger::log("virtualImageHeight:"+std::to_string(textureReference->getVirtualImageHeight()));
    Logger::log("edge1TexturePoint.getX:"+std::to_string(edge1TexturePoint.getX()));
    Logger::log("edge1TexturePoint.getY:"+std::to_string(edge1TexturePoint.getY()));
    Logger::log("edge2TexturePoint.getX:"+std::to_string(edge2TexturePoint.getX()));
    Logger::log("edge2TexturePoint.getY:"+std::to_string(edge2TexturePoint.getY()));
    Logger::log("edge3TexturePoint.getX:"+std::to_string(edge3TexturePoint.getX()));
    Logger::log("edge3TexturePoint.getY:"+std::to_string(edge3TexturePoint.getY()));
  }
  assert(textureReference->getVirtualImageWidth()>0);
  assert(textureReference->getVirtualImageHeight()>0);
  assert(textureReference->getRealImageWidth()>0);
  assert(textureReference->getRealImageHeight()>0);
  assert(edge1TexturePoint.getX()>=0 && edge1TexturePoint.getX()<textureReference->getVirtualImageWidth());
  assert(edge1TexturePoint.getY()>=0 && edge1TexturePoint.getY()<textureReference->getVirtualImageHeight());
  assert(edge2TexturePoint.getX()>=0 && edge2TexturePoint.getX()<textureReference->getVirtualImageWidth());
  assert(edge2TexturePoint.getY()>=0 && edge2TexturePoint.getY()<textureReference->getVirtualImageHeight());
  assert(edge3TexturePoint.getX()>=0 && edge3TexturePoint.getX()<textureReference->getVirtualImageWidth());
  assert(edge3TexturePoint.getY()>=0 && edge3TexturePoint.getY()<textureReference->getVirtualImageHeight());
}

void TextureEdge::render(
  std::vector<MatrixFloat>* worldPoints,
  Vec3DFloat& cameraLocation,
  unsigned int appScreenWidth,
  unsigned int appScreenHeight,
  float transformX,
  float transformY,
  float transformZ
)
{
  assert(edge1<worldPoints->size() && edge1>=0);
  assert(edge2<worldPoints->size() && edge2>=0);
  assert(edge3<worldPoints->size() && edge3>=0);

  if(isVisibleToCamera(
    worldPoints,
    cameraLocation,
    appScreenWidth,
    appScreenHeight,
    transformX,
    transformY,
    transformZ
  )==false){
    return;
  }
  
  MatrixFloat* point1 = &worldPoints->at(edge1);
  MatrixFloat* point2 = &worldPoints->at(edge2);
  MatrixFloat* point3 = &worldPoints->at(edge3);

  float trianglePoint1X = point1->get(0,0);
  float trianglePoint1Y = point1->get(1,0);
  float trianglePoint1Z = point1->get(2,0);
  float trianglePoint2X = point2->get(0,0);
  float trianglePoint2Y = point2->get(1,0);
  float trianglePoint2Z = point2->get(2,0);
  float trianglePoint3X = point3->get(0,0);
  float trianglePoint3Y = point3->get(1,0);
  float trianglePoint3Z = point3->get(2,0);
  
  float texturePoint1X = edge1TexturePoint.getX();
  float texturePoint1Y = edge1TexturePoint.getY();
  float texturePoint2X = edge2TexturePoint.getX();
  float texturePoint2Y = edge2TexturePoint.getY();
  float texturePoint3X = edge3TexturePoint.getX();
  float texturePoint3Y = edge3TexturePoint.getY();
  
  float triangleStartX = trianglePoint1X;
  float triangleStartY = trianglePoint1Y;
  float triangleStartZ = trianglePoint1Z;

  float totalStepCount = 0;
  float triangleStartStepValueX = 0;
  float triangleStartStepValueY = 0;
  float triangleStartStepValueZ = 0;
  {
    if(abs(triangleStartX - trianglePoint3X)>abs(triangleStartY - trianglePoint3Y)){
      float xDifference = trianglePoint3X - triangleStartX;
      assert(xDifference!=0);
      triangleStartStepValueX = (xDifference > 0 ? 1 : -1) * Application::drawStepValue;
      totalStepCount = ceil(abs(xDifference/Application::drawStepValue));
      assert(totalStepCount!=0);
      triangleStartStepValueY = ((trianglePoint3Y - triangleStartY)/xDifference) * triangleStartStepValueX;
      triangleStartStepValueZ = ((trianglePoint3Z - triangleStartZ)/xDifference) * triangleStartStepValueX;
    }else
    {
      float yDifference = trianglePoint3Y - triangleStartY;
      assert(yDifference!=0);
      triangleStartStepValueY = (yDifference > 0 ? 1 : -1) * Application::drawStepValue;
      totalStepCount = ceil(abs(yDifference/Application::drawStepValue));
      assert(totalStepCount!=0);
      triangleStartStepValueX = ((trianglePoint3X - triangleStartX)/yDifference) * triangleStartStepValueY;
      triangleStartStepValueZ = ((trianglePoint3Z - triangleStartZ)/yDifference) * triangleStartStepValueY;
    }
  }

  float triangleEndX = trianglePoint2X;
  float triangleEndY = trianglePoint2Y;
  float triangleEndZ = trianglePoint2Z;

  float triangleEndStepValueX = 0;
  float triangleEndStepValueY = 0;
  float triangleEndStepValueZ = 0;
  {
    if(abs(triangleEndX - trianglePoint3X)>abs(triangleEndY - trianglePoint3Y)){
      float xDifference = trianglePoint3X - triangleEndX;
      assert(xDifference!=0);
      triangleEndStepValueX = xDifference/totalStepCount;
      triangleEndStepValueY = ((trianglePoint3Y - triangleEndY)/xDifference) * triangleEndStepValueX;
      triangleEndStepValueZ = ((trianglePoint3Z - triangleEndZ)/xDifference) * triangleEndStepValueX;
    }else
    {
      float yDifference = trianglePoint3Y - triangleEndY;
      assert(yDifference!=0);
      triangleEndStepValueY = yDifference/totalStepCount;
      triangleEndStepValueX = ((trianglePoint3X - triangleEndX)/yDifference) * triangleEndStepValueY;
      triangleEndStepValueZ = ((trianglePoint3Z - triangleEndZ)/yDifference) * triangleEndStepValueY;
    }
  }

  float textureStartX = texturePoint1X;
  float textureStartY = texturePoint1Y;

  float textureStartStepValueX = 0;
  float textureStartStepValueY = 0;
  {
    if(abs(textureStartX - texturePoint3X)>abs(textureStartY - texturePoint3Y)){
      float xDifference = texturePoint3X - textureStartX;
      assert(xDifference!=0);
      textureStartStepValueX = xDifference/totalStepCount;
      textureStartStepValueY = ((texturePoint3Y - textureStartY)/xDifference) * textureStartStepValueX;
    }else
    {
      float yDifference = texturePoint3Y - textureStartY;
      assert(yDifference!=0);
      textureStartStepValueY = yDifference/totalStepCount;
      textureStartStepValueX = ((texturePoint3X - textureStartX)/yDifference) * textureStartStepValueY;
    } 
  }

  float textureEndX = texturePoint2X;
  float textureEndY = texturePoint2Y;

  float textureEndStepValueX = 0;
  float textureEndStepValueY = 0;
  {
    if(abs(textureEndX - texturePoint3X)>abs(textureEndY - texturePoint3Y)){
      float xDifference = texturePoint3X - textureEndX;
      assert(xDifference!=0);
      textureEndStepValueX = xDifference/totalStepCount;
      textureEndStepValueY = ((texturePoint3Y - textureEndY)/xDifference) * textureEndStepValueX;
    }else{
      float yDifference = texturePoint3Y - textureEndY;
      assert(yDifference!=0);
      textureEndStepValueY = yDifference/totalStepCount;
      textureEndStepValueX = ((texturePoint3X - textureEndX)/yDifference) * textureEndStepValueY;
    }
  }
  
  for(int i=0;i<totalStepCount;i++){
    
    Application::getInstance()->drawTextureBetweenPoints(
      textureReference,
      triangleStartX,
      triangleStartY,
      triangleStartZ,
      triangleEndX,
      triangleEndY,
      triangleEndZ,
      textureStartX,
      textureStartY,
      textureEndX,
      textureEndY
    );

    triangleStartX += triangleStartStepValueX;
    triangleEndX += triangleEndStepValueX;
    triangleStartY += triangleStartStepValueY;
    triangleEndY += triangleEndStepValueY;
    triangleStartZ += triangleStartStepValueZ;
    triangleEndZ += triangleEndStepValueZ;

    textureStartX += textureStartStepValueX;
    textureEndX += textureEndStepValueX;
    textureStartY += textureStartStepValueY;
    textureEndY += textureEndStepValueY;
  }

}

EdgeType TextureEdge::getEdgeType()
{
  return EdgeType::texture;
}