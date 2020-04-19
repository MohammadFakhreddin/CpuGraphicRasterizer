#include "./Camera.h"

#include <cassert>
#include <math.h>
#include <functional>

#include "./../utils/log/Logger.h"
#include "./../utils/math/Math.h"
#include "../event_handler/EventHandler.h"
#include "../data_access_point/DataAccessPoint.h"

Camera::Camera(
  OpenGL& paramsGl,
  MatrixFloat paramsTransform,
  MatrixFloat paramsRotation,
  unsigned int paramsAppScreenWidth,
  unsigned int paramsAppScreenHeight,
  std::string paramsCameraName
)
  :
  cameraName(paramsCameraName),
  appScreenWidth(paramsAppScreenWidth),
  appScreenHeight(paramsAppScreenHeight),
  pixelMapSize(paramsAppScreenWidth * paramsAppScreenHeight),
  gl(paramsGl),
  transformMatrix(3, 1, 0.0f),
  rotationInverseDegreeMatrix(3, 1, 0.0f),
  rotationInverseValueXYZMatrix(3, 3, 0.0f),
  transformationPlaceholder(3, 1, 0.0f),
  cameraCenterX(paramsAppScreenWidth / 2),
  cameraCenterY(paramsAppScreenHeight / 2),
  cameraStartX(0),
  cameraStartY(0),
  cameraEndX(paramsAppScreenWidth),
  cameraEndY(paramsAppScreenHeight)
{

  assert(params.cameraFieldOfView > 0);
  assert(params.appScreenWidth > 0);
  assert(params.appScreenHeight > 0);
  assert(params.transform.getWidth() == 3);
  assert(params.transform.getHeight() == 1);
  assert(params.rotation.getWidth() == 3);
  assert(params.transform.getHeight() == 1);

  this->transform(
    paramsTransform.get(0, 0),
    paramsTransform.get(1, 0),
    paramsTransform.get(2, 0)
  );

  this->rotateXYZ(
    paramsRotation.get(0, 0),
    paramsRotation.get(1, 0),
    paramsRotation.get(2, 0)
  );

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

  this->cameraCenterX = this->appScreenWidth / 2;
  this->cameraCenterY = this->appScreenHeight / 2;

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
    zValue >= 0 ||
    zValue <= zDefaultValue ||
    x < 0 ||
    long(x) >= long(appScreenWidth) ||
    y < 0 ||
    long(y) >= long(appScreenHeight)
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

void Camera::render(double deltaTime){
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
  
  transformationPlaceholder.set(0, 0, transformX);
  transformationPlaceholder.set(1, 0, transformY);
  transformationPlaceholder.set(2, 0, transformZ);
  
  transformMatrix.sum(transformationPlaceholder);

}

//TODO We can use other ways instead of sin and cos for new matrix calculation
void Camera::rotateXYZ(const float& x,const float& y,const float& z) {
  //For camera we reverse the rotation to apply to pipeline shapes
  rotationInverseDegreeMatrix.set(0, 0, rotationInverseDegreeMatrix.get(0, 0) - x);
  rotationInverseDegreeMatrix.set(1, 0, rotationInverseDegreeMatrix.get(1, 0) - y);
  rotationInverseDegreeMatrix.set(2, 0, rotationInverseDegreeMatrix.get(2, 0) - z);

  MatrixFloat::assignAsRotationXYZMatrix(
    rotationInverseValueXYZMatrix,
    rotationInverseDegreeMatrix.get(0, 0),
    rotationInverseDegreeMatrix.get(1, 0),
    rotationInverseDegreeMatrix.get(2, 0)
  );

}

const MatrixFloat& Camera::getTransformMatrix() {
  return transformMatrix;
}

const MatrixFloat& Camera::getRotationInverseXYZ() {
  return rotationInverseValueXYZMatrix;
}

const unsigned int& Camera::getCameraCenterX() const {
  return cameraCenterX;
}

const unsigned int& Camera::getCameraCenterY() const {
  return cameraCenterY;
}


void Camera::generateCameraToPointVector(
  const MatrixFloat& worldPoint,
  MatrixFloat& output
) const {
  //TODO It must be camera position instead
  output.set(
    0,
    0,
    Math::clamp(
      worldPoint.get(0, 0),
      cameraStartX,
      cameraEndY
    ) - worldPoint.get(0, 0)
  );
  output.set(
    1,
    0,
    Math::clamp(
      worldPoint.get(1, 0),
      cameraStartY,
      cameraEndY
    ) - worldPoint.get(1, 0)
  );
  output.set(2, 0, worldPoint.get(2, 0));
}

bool Camera::isVisibleToCamera(
  std::vector<MatrixFloat>& worldPoints,
  std::vector<MatrixFloat>& normals,
  const unsigned long edgeIndices[3],
  const unsigned long normalVectorIndices[3],
  MatrixFloat& cameraVectorPlaceholder
) {
  
  bool isShapeCompletlyOutOfCamera = true;
  //Need to clip when shape is out of camera
  for (unsigned short i = 0; i < 3; i++) {

    const auto& currentWorldPoint = worldPoints.at(edgeIndices[i]);

    if (
      currentWorldPoint.get(0, 0) >= cameraStartX &&
      currentWorldPoint.get(0, 0) < cameraEndX &&
      currentWorldPoint.get(1, 0) >= cameraStartY &&
      currentWorldPoint.get(1, 0) < cameraEndY
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
  *totalStepCount = Math::min(
    (unsigned int)abs(difference / drawStepValue) + 1,
    appScreenWidth
  );
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
  MatrixFloat& stepValueMatrix
) const {
  assert(stepCount > 0);
  assert(stepValueMatrix.getWidth() == 2);
  assert(stepValueMatrix.getHeight() == 1);
  stepValueMatrix.setX(differenceX / float(stepCount));
  stepValueMatrix.setY(differenceY / float(stepCount));
}

void Camera::calculateStepValueBasedOnStepCount(
  const float& differenceX,
  const float& differenceY,
  const float& differenceZ,
  const unsigned int& stepCount,
  MatrixFloat& stepValueMatrix
) const {
  assert(stepCount > 0);
  assert(stepValueMatrix.getWidth() == 3);
  assert(stepValueMatrix.getHeight() == 1);
  stepValueMatrix.setX(differenceX / float(stepCount));
  stepValueMatrix.setY(differenceY / float(stepCount));
  stepValueMatrix.setZ(differenceZ / float(stepCount));
}


void Camera::calculateStepValueBasedOnStepCount(
  const MatrixFloat& differenceMatrix,
  const unsigned int& stepCount,
  MatrixFloat& stepValueMatrix
) const {
  assert(stepCount > 0);
  assert(stepValueMatrix.getMatrixArraySize() == differenceMatrix.getMatrixArraySize());
  for (unsigned short i = 0; i < differenceMatrix.getMatrixArraySize(); i++) {
    stepValueMatrix.setDirect(i, differenceMatrix.getDirect(i) / float(stepCount));
  }
}