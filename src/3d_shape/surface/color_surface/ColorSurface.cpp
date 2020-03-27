#include "./ColorSurface.h"

#include <cassert>
#include <cmath>

#include "./../../../camera/Camera.h"

ColorSurface::ColorSurface(
  unsigned int edge1,
  unsigned int edge2,
  unsigned int edge3,
  float red,
  float green,
  float blue
)
:
  red(red),
  green(green),
  blue(blue)
{
  this->edge1 = edge1;
  this->edge2 = edge2;
  this->edge3 = edge3;
}

void ColorSurface::computePixelMapData(
  Camera& cameraInstance,
  std::vector<MatrixFloat>* worldPoints
){
  //TODO Remove all these temporary variables to boost performance
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

  for(unsigned int i=0;i<totalStepCount;i++){
    
    drawLineBetweenPoints(
      cameraInstance,
      triangleStartX,
      triangleStartY,
      triangleStartZ,
      triangleEndX,
      triangleEndY,
      triangleEndZ,
      red,green,blue
    );

    triangleStartX += triangleStartStepValueX;
    triangleEndX += triangleEndStepValueX;
    triangleStartY += triangleStartStepValueY;
    triangleEndY += triangleEndStepValueY;
    triangleStartZ += triangleStartStepValueZ;
    triangleEndZ += triangleEndStepValueZ;

  }

}

EdgeType ColorSurface::getEdgeType(){
  return EdgeType::color;
}

void ColorSurface::drawLineBetweenPoints(
  Camera& cameraInstance,
	float triangleStartX,
	float triangleStartY,
	float triangleStartZ,
	float triangleEndX,
	float triangleEndY,
	float triangleEndZ,
  float red,
  float green,
  float blue
){
  assert(red>=0&&red<=1.0f);
  assert(green>=0 && green<=1.0f);
  assert(blue>=0 && blue<=1.0f);
  
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
			assert(triangleTotalStepCount!=0);
      triangleXStepValue = ((triangleEndX - triangleStartX)/yDifference) * triangleYStepValue;
			triangleZStepValue = ((triangleEndZ - triangleStartZ)/yDifference) * triangleYStepValue;
		}
	}

	putPixelInMap(
    cameraInstance,
		int(floor(triangleStartX)),
		int(floor(triangleStartY)),
		triangleStartZ,
		red,
		green,
		blue
	);
	for(unsigned int i=0;i<triangleTotalStepCount;i++){
		triangleStartX += triangleXStepValue;
		triangleStartY += triangleYStepValue;
		triangleStartZ += triangleZStepValue;
		putPixelInMap(
      cameraInstance,
			int(floor(triangleStartX)),
			int(floor(triangleStartY)),
			triangleStartZ,
			red,
			green,
			blue
		);
	}
}