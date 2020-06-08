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
  appScreenWidth(paramAppScreenWidth),
  appScreenHeight(paramAppScreenHeight),
  pixelMapSize(paramAppScreenWidth * paramAppScreenHeight),
  gl(paramGl),
  startX(0.0f),
  endX(float(paramAppScreenWidth)),
  startY(0.0f),
  endY(float(paramAppScreenHeight))
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
  
  delete[] points;
  
  delete[] colors;

  delete[] colorsDefaultValue;

  delete[] pointsDefaultValue;
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

  delete[] points;

  delete[] colors;

  initPixelMap();
}

void Camera::initPixelMap(){

  double xPixelStep = 2.0 / double(endX - startX);
  double yPixelStep = 2.0 / double(endY - startY);

  points = new GLfloat[pixelMapSize * 4];
  pointsDefaultValue = new GLfloat[pixelMapSize * 4];
  sizeOfPoints = sizeof(GLfloat) * pixelMapSize * 4;
  colors = new GLfloat[pixelMapSize * 4];
  colorsDefaultValue = new GLfloat[pixelMapSize * 4];
  sizeOfColors = sizeof(GLfloat) * pixelMapSize * 4;
  unsigned int rowValue = 0;
  unsigned int pointsIndex = 0;
  unsigned int colorsIndex = 0;
  for(unsigned int i=0;i<appScreenWidth;i++){
    for(unsigned int j=0;j<appScreenHeight;j++){
      pointsIndex = rowValue * 4;
      colorsIndex = rowValue * 4;
      points[pointsIndex] = float(-1.0 + xPixelStep * i);
      points[pointsIndex + 1] = float(-1.0 + yPixelStep * j);
      points[pointsIndex + 2] = endZ;
      points[pointsIndex + 3] = 1.0f;
      pointsDefaultValue[pointsIndex] = float(-1.0 + xPixelStep * i);
      pointsDefaultValue[pointsIndex + 1] = float(-1.0 + yPixelStep * j);
      pointsDefaultValue[pointsIndex + 2] = endZ;
      pointsDefaultValue[pointsIndex + 3] = 1.0f;
      colors[colorsIndex] = 0.0f;
      colors[colorsIndex + 1] = 0.0f;
      colors[colorsIndex + 2] = 0.0f;
      colors[colorsIndex + 3] = 1.0f;
      colorsDefaultValue[colorsIndex] = 0.0f;
      colorsDefaultValue[colorsIndex + 1] = 0.0f;
      colorsDefaultValue[colorsIndex + 2] = 0.0f;  
      colorsDefaultValue[colorsIndex + 3] = 1.0f;
      rowValue++;
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
  unsigned int pointIndex = index * 4;
  unsigned int colorIndex = index * 4;
  if (points[pointIndex + 2] > zValue) {
    //TODO Maybe impelemting some antialising
    points[pointIndex + 2] = zValue;
    colors[colorIndex] = red;
    colors[colorIndex + 1] = green;
    colors[colorIndex + 2] = blue;
  }
}

void Camera::render(const double& deltaTime){
  {//Drawing screen
    gl.beginDrawingPoints();
    gl.drawPixel(points,colors,pixelMapSize);
    std::memcpy(colors,colorsDefaultValue,sizeOfColors);
    std::memcpy(points,pointsDefaultValue,sizeOfPoints);
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
