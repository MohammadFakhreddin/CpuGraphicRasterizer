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
  std::vector<MatrixFloat>& worldPoints
){
  //TODO Remove all these temporary variables to boost performance
  const MatrixFloat& point1 = worldPoints.at(edge1);
  const MatrixFloat& point2 = worldPoints.at(edge2);
  const MatrixFloat& point3 = worldPoints.at(edge3);

  const auto trianglePoint1X = point1.get(0,0);
  const auto trianglePoint1Y = point1.get(1,0);
  const auto trianglePoint1Z = point1.get(2,0);
  const auto trianglePoint2X = point2.get(0,0);
  const auto trianglePoint2Y = point2.get(1,0);
  const auto trianglePoint2Z = point2.get(2,0);
  const auto trianglePoint3X = point3.get(0,0);
  const auto trianglePoint3Y = point3.get(1,0);
  const auto trianglePoint3Z = point3.get(2,0);
  
  auto triangleStartX = trianglePoint1X;
  auto triangleStartY = trianglePoint1Y;
  auto triangleStartZ = trianglePoint1Z;

  unsigned int totalStepCount = 0;
  float triangleStartStepValueX = 0.0f;
  float triangleStartStepValueY = 0.0f;
  float triangleStartStepValueZ = 0.0f;

  auto xDifference = trianglePoint3X - triangleStartX;
  auto yDifference = trianglePoint3Y - triangleStartY;
  if (xDifference == 0 && yDifference == 0) {
    return;
  }

  {
    if(abs(xDifference)>abs(yDifference)){
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
      triangleStartStepValueY = ((trianglePoint3Y - triangleStartY)/xDifference) * triangleStartStepValueX;
      triangleStartStepValueZ = ((trianglePoint3Z - triangleStartZ)/xDifference) * triangleStartStepValueX;
    }else
    {
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

  xDifference = trianglePoint3X - triangleEndX;
  yDifference = trianglePoint3Y - triangleEndY;
  if (xDifference == 0 && yDifference == 0) {
    return;
  }

  {
    if(abs(xDifference)>abs(yDifference)){
      assert(xDifference!=0);
      triangleEndStepValueX = xDifference/totalStepCount;
      triangleEndStepValueY = ((trianglePoint3Y - triangleEndY)/xDifference) * triangleEndStepValueX;
      triangleEndStepValueZ = ((trianglePoint3Z - triangleEndZ)/xDifference) * triangleEndStepValueX;
    }else
    {
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

  auto xDifference = triangleEndX - triangleStartX;
  auto yDifference = triangleEndY - triangleStartY;
  if (xDifference == 0 && yDifference == 0) {
    return;
  }

	{//TriangleStepValue
		if(abs(xDifference) > abs(yDifference)){
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