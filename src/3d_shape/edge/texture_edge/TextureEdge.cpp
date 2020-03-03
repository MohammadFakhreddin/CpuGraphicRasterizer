#include "./TextureEdge.h"
#include <assert.h>
#include <math.h>
#include "../../../application/Application.h"

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
}

void TextureEdge::render(std::vector<MatrixFloat>* worldPoints)
{
  assert(edge1<worldPoints->size() && edge1>=0);
  assert(edge2<worldPoints->size() && edge2>=0);
  assert(edge3<worldPoints->size() && edge3>=0);

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

  float trianglePoint1ToPoint3YM = (trianglePoint3Y - trianglePoint1Y)/(trianglePoint3X - trianglePoint1X);
  float trianglePoint2ToPoint3YM = (trianglePoint3Y - trianglePoint2Y)/(trianglePoint3X - trianglePoint2X);
  float trianglePoint1ToPoint3ZM = (trianglePoint3Z - trianglePoint1Z)/(trianglePoint3X - trianglePoint1X);
  float trianglePoint2ToPoint3ZM = (trianglePoint3Z - trianglePoint2Z)/(trianglePoint3X - trianglePoint2X);
  
  float texturePoint1X = edge1TexturePoint.getX();
  float texturePoint1Y = edge1TexturePoint.getY();
  float texturePoint2X = edge2TexturePoint.getX();
  float texturePoint2Y = edge2TexturePoint.getY();
  float texturePoint3X = edge3TexturePoint.getX();
  float texturePoint3Y = edge3TexturePoint.getY();

  float texturePoint1To3YM = (texturePoint3Y - texturePoint1Y)/(texturePoint3X - texturePoint1X);
  float texturePoint2To3YM = (texturePoint3Y - texturePoint2Y)/(texturePoint3X - texturePoint2X);
  
  int triangleStartStepValue = trianglePoint3X > trianglePoint1X ? 1 : -1;
  int triangleEndStepValue = trianglePoint3X > trianglePoint2X ? 1 : -1;
  int textureStartStepValue = texturePoint3X > texturePoint1X ? 1 : -1;
  int textureEndStepValue = texturePoint3X > texturePoint2X ? 1 : -1;

  float triangleStartX = trianglePoint1X;
  float triangleStartY = trianglePoint1Y;
  float triangleStartZ = trianglePoint1Z;
  float triangleEndX = trianglePoint2X;
  float triangleEndY = trianglePoint2Y;
  float triangleEndZ = trianglePoint2Z;

  float textureStartX = texturePoint1X;
  float textureStartY = texturePoint1Y;
  float textureEndX = texturePoint2X;
  float textureEndY = texturePoint2Y;
  
  do{
    //TODO Step values are wrong
    //Some of them must be negative
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

    triangleStartX += triangleStartStepValue;
    triangleEndX += triangleEndStepValue;
    triangleStartY += trianglePoint1ToPoint3YM * triangleStartStepValue;
    triangleEndY += trianglePoint2ToPoint3YM * triangleEndStepValue;
    triangleStartZ += trianglePoint1ToPoint3ZM * triangleStartStepValue;
    triangleEndZ += trianglePoint2ToPoint3ZM * triangleEndStepValue;

    textureStartX += textureStartStepValue;
    textureEndX += textureEndStepValue;
    textureStartY += texturePoint1To3YM * textureStartStepValue;
    textureEndY += texturePoint2To3YM * textureEndStepValue;

  }while ((
    triangleStartStepValue==-1 && trianglePoint3X < triangleStartX + triangleStartStepValue
  )||(
    triangleEndStepValue==1 && trianglePoint3X > triangleStartX + triangleEndStepValue
  ));  

}

EdgeType TextureEdge::getEdgeType()
{
  return EdgeType::texture;
}