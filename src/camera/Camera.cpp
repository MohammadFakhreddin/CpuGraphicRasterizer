#include "./Camera.h"

#include <cassert>
#include <math.h>
#include <functional>

#include "./../utils/log/Logger.h"
#include "./../utils/math/Math.h"
#include "../event_handler/EventHandler.h"
#include "../data_access_point/DataAccessPoint.h"

//TODO Add constructor without transformation and rotation

Camera::Camera(
  OpenGL& paramGl,
  Matrix3X1Float paramTransform,
  Matrix3X1Float paramRotation,
  unsigned int paramAppScreenWidth,
  unsigned int paramAppScreenHeight,
  std::string paramCameraName
)
  :
  gl(paramGl),
  pixelMapSize(paramAppScreenWidth * paramAppScreenHeight),
  startX(0),
  startY(0),
  endX(paramAppScreenWidth),
  endY(paramAppScreenHeight),
  appScreenWidth(paramAppScreenWidth),
  appScreenHeight(paramAppScreenHeight)
{

  assert(paramAppScreenWidth > 0);
  assert(paramAppScreenHeight > 0);
  
  this->transform(
    paramTransform.getX(),
    paramTransform.getY(),
    paramTransform.getZ()
  );

  this->rotateXYZ(
    paramRotation.getX(),
    paramRotation.getY(),
    paramRotation.getZ()
  );

  Matrix4X4Float::assignProjection(
    projection,
    startX,
    endX,
    startY,
    endY,
    startZ,
    endZ
  );


  position.setX((endX - startX) / 2.0f);
  position.setY((endY - startY) / 2.0f);
  //Usually camera distance to zero is equal to far
  position.setZ(-1 * (endZ - startZ));

  DataAccessPoint::getInstance()->getEventHandler().subscribeToEvent<bool>(
    EventHandler::EventName::screenSurfaceChanged,
    cameraName,
    std::bind(&Camera::notifyScreenSurfaceIsChanged, this, std::placeholders::_1)
  );

  initPixelMap();

}

Camera::~Camera() {
  DataAccessPoint::getInstance()->getEventHandler().unSubscribeFromEvents(cameraName);
  delete[] pixelMap;
}

void Camera::notifyScreenSurfaceIsChanged(
  bool usingNewAppScreenWidthAndHeightIsForced
){

  if (
    usingNewAppScreenWidthAndHeightIsForced==false || 
    (
      this->appScreenWidth == DataAccessPoint::getInstance()->getAppScreenWidth() && 
      this->appScreenHeight == DataAccessPoint::getInstance()->getAppScreenHeight()
    )
  ) {
    return;
  }

  this->appScreenWidth = DataAccessPoint::getInstance()->getAppScreenWidth();
  this->appScreenHeight = DataAccessPoint::getInstance()->getAppScreenHeight();

  assert(appScreenWidth>0);
  assert(appScreenHeight>0);

  delete[] pixelMap;

  initPixelMap();
}

void Camera::initPixelMap(){

  pixelMap = new DrawPixel[pixelMapSize];
  unsigned int rowValue = 0;
  for(unsigned int i=0;i<appScreenWidth;i++){
    rowValue = i * appScreenHeight;
    for(unsigned int j=0;j<appScreenHeight;j++){
      auto currentPixel = &pixelMap[rowValue + j];
      currentPixel->zValue = zDefaultValue;
      currentPixel->blue = 0;
      currentPixel->green = 0;
      currentPixel->red = 0;
      currentPixel->x = i;
      currentPixel->y = j;
    }
  }

}

void Camera::putPixelInMap(
  const int& x,
  const int& y,
  const float& zValue,
  const float& red,
  const float& green,
  const float& blue
) {

  assert(red>=0 && red<=1.0f);
  assert(green>=0 && green<=1.0f);
  assert(blue>=0 && blue<=1.0f);
  
  if(
    zValue <= startZ ||
    zValue >= endZ ||
    x < startX ||
    x >= endX ||
    y < startY ||
    y >= endY
  ){
    return;
  }  

  assignToPixel(x * appScreenHeight + y, zValue, red, green, blue);

}

void Camera::assignToPixel(
  const unsigned int& index,
  const float& zValue,
  const float& red,
  const float& green,
  const float& blue
) {
  if (pixelMap[index].zValue < zValue) {
    pixelMap[index].blue = blue;
    pixelMap[index].green = green;
    pixelMap[index].red = red;
    pixelMap[index].zValue = zValue;
  }
}

void Camera::render(const double& deltaTime){
  {//Drawing screen
    gl.beginDrawingPoints();
    for (unsigned int i = 0; i < pixelMapSize; i++) {
      auto currentPixel = &pixelMap[i];
      if (currentPixel->zValue != zDefaultValue) {
        gl.drawPixel(
          currentPixel->x,
          currentPixel->y,
          currentPixel->red,
          currentPixel->green,
          currentPixel->blue
        );
        currentPixel->zValue = zDefaultValue;
        currentPixel->red = 0.0f;
        currentPixel->green = 0.0f;
        currentPixel->blue = 0.0f;
      }
    }
    gl.resetProgram();
  }
}

float Camera::scaleBasedOnZDistance(const float& zLocation) {
  return 1 / zLocation;
}

const unsigned int& Camera::getAppScreenWidth() const {
  return appScreenWidth;
}

const unsigned int& Camera::getAppScreenHeight() const {
  return appScreenHeight;
}

//TODO Check this code again
//It must transform based on theta
void Camera::transform(float transformX, float transformY, float transformZ) {
  
  transformInverseValue.setX(transformInverseValue.getX() - transformX);
  transformInverseValue.setY(transformInverseValue.getY() - transformY); 
  transformInverseValue.setZ(transformInverseValue.getZ() - transformZ);
  
  Matrix4X4Float::assignTransformation(
    transformInverseMatrix, 
    transformInverseValue.getX(), 
    transformInverseValue.getY(), 
    transformInverseValue.getZ()
  );

}

//TODO We can use other ways instead of sin and cos for new matrix calculation
void Camera::rotateXYZ(const float& x,const float& y,const float& z) {
  //For camera we reverse the rotation to apply to pipeline shapes
  rotationInverseDegree.set(0, 0, rotationInverseDegree.get(0, 0) - x);
  rotationInverseDegree.set(1, 0, rotationInverseDegree.get(1, 0) - y);
  rotationInverseDegree.set(2, 0, rotationInverseDegree.get(2, 0) - z);

  Matrix4X4Float::assignRotationXYZ(
    rotationInverseMatrix,
    rotationInverseDegree.getX(),
    rotationInverseDegree.getY(),
    rotationInverseDegree.getZ()
  );

}

void Camera::generateCameraToPointVector(
  const Matrix4X1Float& worldPoint,
  Matrix4X1Float& output
) const {
  //TODO check for vector correctness
  output.assign(position);
  output.minus(worldPoint);
  output.setW(1.0f);
}

bool Camera::isVisibleToCamera(
  std::vector<Matrix4X1Float>& worldPoints,
  std::vector<Matrix4X1Float>& normals,
  const unsigned long edgeIndices[3],
  const unsigned long normalVectorIndices[3],
  Matrix4X1Float& cameraVectorPlaceholder
) {
  
  bool isShapeCompletlyOutOfCamera = true;
  //Need to clip when shape is out of camera
  for (unsigned short i = 0; i < 3; i++) {

    const auto& currentWorldPoint = worldPoints.at(edgeIndices[i]);

    if (
      currentWorldPoint.get(0, 0) >= startX &&
      currentWorldPoint.get(0, 0) < endX &&
      currentWorldPoint.get(1, 0) >= startY &&
      currentWorldPoint.get(1, 0) < endY
      ) {

      isShapeCompletlyOutOfCamera = false;
      break;

    }

  }

  if (isShapeCompletlyOutOfCamera == true) {
    return false;
  }

  for (unsigned int i = 0; i < 3; i++) {

    generateCameraToPointVector(
      worldPoints.at(edgeIndices[i]),
      cameraVectorPlaceholder
    );

    double dotProductValue = normals.at(
      normalVectorIndices[i]
    ).dotProduct(
      cameraVectorPlaceholder
    );

    if (dotProductValue <= 0.0f) {
      return true;
    }

  }
  return false;
}

void Camera::calculateStepCount(
  const float& difference,
  unsigned int* totalStepCount
) const {
  assert(difference != 0 && "Difference must be above 0 in BaseSurface::calculateStepCountAndStepValue");
  assert(drawStepValue != 0 && "Draw step value must be above 0 in BaseSurface::calculateStepCountAndStepValue");
  *totalStepCount = (unsigned int)abs(difference / drawStepValue) + 1;
  if (*totalStepCount > appScreenWidth) {
    *totalStepCount = appScreenWidth;
  }
}

void Camera::calculateStepValueBasedOnStepCount(
  const float& difference,
  const unsigned int& stepCount,
  float* stepValue
) const {
  assert(stepCount > 0);
  *stepValue = difference / float(stepCount);
}

void Camera::calculateStepValueBasedOnStepCount(
  const float& differenceX,
  const float& differenceY,
  const unsigned int& stepCount,
  Matrix2X1Float& stepValueMatrix
) const {
  assert(stepCount > 0);
  stepValueMatrix.setX(differenceX / float(stepCount));
  stepValueMatrix.setY(differenceY / float(stepCount));
}

void Camera::calculateStepValueBasedOnStepCount(
  const float& differenceX,
  const float& differenceY,
  const float& differenceZ,
  const unsigned int& stepCount,
  Matrix3X1Float& stepValueMatrix
) const {
  assert(stepCount > 0);
  stepValueMatrix.setX(differenceX / float(stepCount));
  stepValueMatrix.setY(differenceY / float(stepCount));
  stepValueMatrix.setZ(differenceZ / float(stepCount));
}

void Camera::calculateStepValueBasedOnStepCount(
  const float& differenceX,
  const float& differenceY,
  const float& differenceZ,
  const unsigned int& stepCount,
  Matrix4X1Float& stepValueMatrix
) const {
  assert(stepCount > 0);
  stepValueMatrix.setX(differenceX / float(stepCount));
  stepValueMatrix.setY(differenceY / float(stepCount));
  stepValueMatrix.setZ(differenceZ / float(stepCount));
  stepValueMatrix.setW(0);
}

void Camera::calculateStepValueBasedOnStepCount(
  const Matrix3X1Float& differenceMatrix,
  const unsigned int& stepCount, 
  Matrix3X1Float& stepValueMatrix
) const
{
  assert(stepCount > 0);
  for (unsigned short i = 0; i < 3; i++) {
    stepValueMatrix.setDirect(i, differenceMatrix.getDirect(i) / float(stepCount));
  }
  stepValueMatrix.setDirect(3, 0);
}


void Camera::calculateStepValueBasedOnStepCount(
  const Matrix4X1Float& differenceMatrix,
  const unsigned int& stepCount,
  Matrix4X1Float& stepValueMatrix
) const {
  assert(stepCount > 0);
  for (unsigned short i = 0; i < 3; i++) {
    stepValueMatrix.setDirect(i, differenceMatrix.getDirect(i) / float(stepCount));
  }
  stepValueMatrix.setDirect(3, 0);
}