#include "./TextureSurface.h"

#include <cassert>
#include <cmath>
#include <string>

#include "./../../../utils/log/Logger.h"
#include "../../../camera/Camera.h"

TextureSurface::TextureSurface(
  int edge1,
  int edge2,
  int edge3,
  std::unique_ptr<FaTexture>& textureReference,
  Vec2DFloat edge1TexturePoint,
  Vec2DFloat edge2TexturePoint,
  Vec2DFloat edge3TexturePoint
)
:
edge1TexturePoint(edge1TexturePoint),
edge2TexturePoint(edge2TexturePoint),
edge3TexturePoint(edge3TexturePoint),
textureReference(textureReference)
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

void TextureSurface::computePixelMapData(
  Camera& cameraInstance,
  std::vector<MatrixFloat>& worldPoints
)
{
  //TODO Remove all these temporary variables to boost performance
  const auto& point1 = worldPoints.at(edge1);
  const auto& point2 = worldPoints.at(edge2);
  const auto& point3 = worldPoints.at(edge3);

  float trianglePoint1X = point1.get(0,0);
  float trianglePoint1Y = point1.get(1,0);
  float trianglePoint1Z = point1.get(2,0);
  float trianglePoint2X = point2.get(0,0);
  float trianglePoint2Y = point2.get(1,0);
  float trianglePoint2Z = point2.get(2,0);
  float trianglePoint3X = point3.get(0,0);
  float trianglePoint3Y = point3.get(1,0);
  float trianglePoint3Z = point3.get(2,0);
  
  float texturePoint1X = edge1TexturePoint.getX();
  float texturePoint1Y = edge1TexturePoint.getY();
  float texturePoint2X = edge2TexturePoint.getX();
  float texturePoint2Y = edge2TexturePoint.getY();
  float texturePoint3X = edge3TexturePoint.getX();
  float texturePoint3Y = edge3TexturePoint.getY();
  
  float triangleStartX = trianglePoint1X;
  float triangleStartY = trianglePoint1Y;
  float triangleStartZ = trianglePoint1Z;

  unsigned int totalStepCount = 0;
  float triangleStartStepValueX = 0;
  float triangleStartStepValueY = 0;
  float triangleStartStepValueZ = 0;
  {
    if(abs(triangleStartX - trianglePoint3X)>abs(triangleStartY - trianglePoint3Y)){
      float xDifference = trianglePoint3X - triangleStartX;
      assert(xDifference!=0);
      calculateStepCountAndStepValue(
        cameraInstance,
        xDifference,
        &totalStepCount,
        &triangleStartStepValueX
      );
      if (totalStepCount == 0) {
        return;
      }
      assert(totalStepCount!=0);
      triangleStartStepValueY = ((trianglePoint3Y - triangleStartY)/xDifference) * triangleStartStepValueX;
      triangleStartStepValueZ = ((trianglePoint3Z - triangleStartZ)/xDifference) * triangleStartStepValueX;
    }else
    {
      float yDifference = trianglePoint3Y - triangleStartY;
      assert(yDifference!=0);
      calculateStepCountAndStepValue(
        cameraInstance,
        yDifference,
        &totalStepCount,
        &triangleStartStepValueY
      );
      if (totalStepCount == 0) {
        return;
      }
      assert(totalStepCount!=0);
      triangleStartStepValueX = ((trianglePoint3X - triangleStartX)/yDifference) * triangleStartStepValueY;
      triangleStartStepValueZ = ((trianglePoint3Z - triangleStartZ)/yDifference) * triangleStartStepValueY;
    }
  }

  assert(totalStepCount!=0);

  if(DEBUG_MODE){
    Logger::log("TotalStepCount:"+std::to_string(totalStepCount));
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
  
  for(unsigned int i=0;i<totalStepCount;i++){
    
    drawTextureBetweenPoints(
      cameraInstance,
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

void TextureSurface::drawTextureBetweenPoints(
  Camera& cameraInstance,
	std::unique_ptr<FaTexture>& texture,
	float triangleStartX,
	float triangleStartY,
	float triangleStartZ,
	float triangleEndX,
	float triangleEndY,
	float triangleEndZ,
	float textureStartX,
	float textureStartY,
	float textureEndX,
	float textureEndY
){

	unsigned int triangleTotalStepCount = 0;
	float triangleXStepValue = 0;
	float triangleYStepValue = 0;
	float triangleZStepValue = 0;
	{//TriangleStepValue
		if(abs(triangleEndX - triangleStartX) > abs(triangleEndY - triangleStartY)){
			float xDifference = triangleEndX - triangleStartX;
			assert(xDifference!=0);
      calculateStepCountAndStepValue(
        cameraInstance,
        xDifference,
        &triangleTotalStepCount,
        &triangleXStepValue
      );
      if (triangleTotalStepCount == 0) {
        return;
      }
			assert(triangleTotalStepCount!=0);
      triangleYStepValue = ((triangleEndY - triangleStartY)/xDifference) * triangleXStepValue;
			triangleZStepValue = ((triangleEndZ - triangleStartZ)/xDifference) * triangleXStepValue;
		}else{
			float yDifference = triangleEndY - triangleStartY;
			assert(yDifference!=0);
      calculateStepCountAndStepValue(
        cameraInstance,
        yDifference,
        &triangleTotalStepCount,
        &triangleYStepValue
      );
      if (triangleTotalStepCount == 0) {
        return;
      }
			assert(triangleTotalStepCount!=0);
      triangleXStepValue = ((triangleEndX - triangleStartX)/yDifference) * triangleYStepValue;
			triangleZStepValue = ((triangleEndZ - triangleStartZ)/yDifference) * triangleYStepValue;
		}
	}

	float textureXStepValue = 0;
	float textureYStepValue = 0;
	{//TextureStepValue
		if(abs(textureEndX - textureStartX)>abs(textureEndY - textureStartY)){
			float xDifference = textureEndX - textureStartX;
			assert(xDifference!=0);
			textureXStepValue = xDifference/triangleTotalStepCount;
			textureYStepValue = ((textureEndY - textureStartY)/xDifference) * textureXStepValue;
		}else
		{
			float yDifference = textureEndY - textureStartY;
			assert(yDifference!=0);
			textureYStepValue = yDifference/triangleTotalStepCount;
			textureXStepValue = ((textureEndX - textureStartX)/yDifference) * textureYStepValue;
		}
	}
	
	float red = 0;
	float green = 0;
	float blue = 0;

	texture->getColorForPosition(textureStartX,textureStartY,&red,&green,&blue);
	putPixelInMap(
    cameraInstance,
		int(triangleStartX),
		int(triangleStartY),
		triangleStartZ,
		red,
		green,
		blue
	);

	for(unsigned int i=0;i<triangleTotalStepCount;i++){
		triangleStartX += triangleXStepValue;
		triangleStartY += triangleYStepValue;
		triangleStartZ += triangleZStepValue;
		textureStartX += textureXStepValue;
		textureStartY += textureYStepValue;
		texture->getColorForPosition(textureStartX,textureStartY,&red,&green,&blue);
		putPixelInMap(
      cameraInstance,
			int(triangleStartX),
			int(triangleStartY),
			triangleStartZ,
			red,
			green,
			blue
		);
	}
}
