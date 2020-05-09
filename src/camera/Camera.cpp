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
  startX(0.0f),
  startY(0.0f),
  endX(float(paramAppScreenWidth)),
  endY(float(paramAppScreenHeight)),
  appScreenWidth(paramAppScreenWidth),
  appScreenHeight(paramAppScreenHeight)
{

  assert(paramAppScreenWidth > 0);
  assert(paramAppScreenHeight > 0);

  this->rotateXYZ(
    paramRotation.getX(),
    paramRotation.getY(),
    paramRotation.getZ()
  );

  this->transformXYZ(
    paramTransform.getX(),
    paramTransform.getY(),
    paramTransform.getZ()
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

  // In current camera projection 
  // 1- X is between -1 and 1
  // 2- Y is between -1 and 1
  // 3- Z is between 0 and 1
  // So camera position must be in oposite direction
  /*
  Projection applied x and y and z in openGL defined world is following values
  position.setX(0);
  position.setY(0);
  position.setZ(-1.0f);
  */
  position.setX((endX - startX) / 2.0f);
  position.setY((endY - startY) / 2.0f);
  position.setZ(-1.0f * endZ);

  screenCenter.setX(position.getX());
  screenCenter.setY(position.getY());
  screenCenter.setZ(0.0);
  screenCenter.setW(0.0);

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

  double xPixelStep = 2.0 / double(endX - startX);
  double yPixelStep = 2.0 / double(endY - startY);

  pixelMap = new DrawPixel[pixelMapSize];
  unsigned int rowValue = 0;
  for(unsigned int i=0;i<appScreenWidth;i++){
    rowValue = i * appScreenHeight;
    for(unsigned int j=0;j<appScreenHeight;j++){
      auto currentPixel = &pixelMap[rowValue + j];
      currentPixel->zValue = endZ;
      currentPixel->blue = 0;
      currentPixel->green = 0;
      currentPixel->red = 0;
      currentPixel->x = float(-1.0 + xPixelStep * i);
      currentPixel->y = float(-1.0 + yPixelStep * j);
    }
  }

}
//TODO Start from here//Put pixel in map must be between -1 and 1 for both x and y
void Camera::putPixelInMap(
  const int& x,
  const int& y,
  const float& zValue,
  const float& red,
  const float& green,
  const float& blue
) {

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
  auto& currentPixel = pixelMap[index];
  if (currentPixel.zValue > zValue) {
    //TODO Maybe impelemting some antialising
    currentPixel.blue = blue;
    currentPixel.green = green;
    currentPixel.red = red;
    currentPixel.zValue = zValue;
  }
}

void Camera::render(const double& deltaTime){
  {//Drawing screen
    gl.beginDrawingPoints();
    for (unsigned int i = 0; i < pixelMapSize; i++) {
      auto currentPixel = &pixelMap[i];
      if (currentPixel->zValue != endZ) {
        gl.drawPixel(
          currentPixel->x,
          currentPixel->y,
          currentPixel->red,
          currentPixel->green,
          currentPixel->blue
        );
        currentPixel->zValue = endZ;
        currentPixel->red = 0.0f;
        currentPixel->green = 0.0f;
        currentPixel->blue = 0.0f;
      }
    }
    gl.resetProgram();
  }
}

float Camera::scaleBasedOnZDistance(const float& zLocation) {
  assert(zLocation != 0.0f);
  return zDistance / zLocation;
}

const unsigned int& Camera::getAppScreenWidth() const {
  return appScreenWidth;
}

const unsigned int& Camera::getAppScreenHeight() const {
  return appScreenHeight;
}

//TODO Check this code again
//It must transform based on theta
void Camera::transformXYZ(const float& transformX, const float& transformY, const float& transformZ) {
  
  newTransform.setX(-1.0f * transformX);
  newTransform.setY(-1.0f * transformY);
  newTransform.setZ(1.0f * transformZ);
  newTransform.setW(1.0f);
  
  newTransform.multiply(rotationXMatrix);

  newTransform.multiply(rotationYMatrix);

  newTransform.multiply(rotationZMatrix);
  
  transformInverseValue.setX(transformInverseValue.getX() + newTransform.getX());
  transformInverseValue.setY(transformInverseValue.getY() + newTransform.getY()); 
  transformInverseValue.setZ(transformInverseValue.getZ() - newTransform.getZ());

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

  rotationDegree.set(0, 0, rotationDegree.get(0, 0) + x);
  rotationDegree.set(1, 0, rotationDegree.get(1, 0) + y);
  rotationDegree.set(2, 0, rotationDegree.get(2, 0) + z);

  Matrix4X4Float::assignRotationX(rotationXMatrix,rotationDegree.getX());

  Matrix4X4Float::assignRotationY(rotationYMatrix,rotationDegree.getY());

  Matrix4X4Float::assignRotationZ(rotationZMatrix,rotationDegree.getZ());

}

void Camera::generateCameraToPointVector(
  const Matrix4X1Float& worldPoint,
  Matrix4X1Float& output
) const {
  //TODO check for vector correctness
  output.assign(worldPoint);
  output.minus(position);
  output.setW(1.0f);
}

bool Camera::isVisibleToCamera(
  std::vector<Matrix4X1Float>& worldPoints,
  std::vector<Matrix4X1Float>& normals,
  const unsigned long edgeIndices[3],
  const unsigned long normalVectorIndices[3],
  Matrix4X1Float& cameraVectorPlaceholder
) {
  
  bool isShapeCompletelyOutOfCamera = true;
  //Need to clip when shape is out of camera
  for (unsigned short i = 0; i < 3; i++) {

    const auto& currentWorldPoint = worldPoints.at(edgeIndices[i]);

    if (
      currentWorldPoint.getX() >= startX &&
      currentWorldPoint.getX() < endX &&
      currentWorldPoint.getY() >= startY &&
      currentWorldPoint.getY() < endY &&
      currentWorldPoint.getZ() > startZ &&
      currentWorldPoint.getZ() < endZ
      ) {

      isShapeCompletelyOutOfCamera = false;
      break;

    }

  }

  if (isShapeCompletelyOutOfCamera == true) {
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