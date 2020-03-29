#include "./Camera.h"

#include <cassert>
#include <math.h>
#include <functional>

#include "./../utils/log/Logger.h"
#include "./../utils/math/Math.h"
#include "../event_handler/EventHandler.h"
#include "../data_access_point/DataAccessPoint.h"

Camera::Camera(
  OpenGL& gl,
  float cameraFieldOfView,
  float transformX,
  float transformY,
  float transformZ,
  float rotationDegreeX,
  float rotationDegreeY,
  float rotationDegreeZ,
  unsigned int appScreenWidth,
  unsigned int appScreenHeight,
  std::string cameraName
) :
  cameraName(cameraName),
  gl(gl),
  cameraFieldOfView(cameraFieldOfView),
  zDefaultValue(cameraFieldOfView * -1),
  appScreenWidth(appScreenWidth),
  appScreenHeight(appScreenHeight),
  transformMatrix(3, 1, 0.0f),
  rotationDegreeMatrix(3, 1, 0.0f),
  rotationValueXMatrix(3, 3, std::vector<std::vector<float>>{
  std::vector<float>{cosf(0), -sinf(0), 0},
    std::vector<float>{sinf(0), cosf(0), 0},
    std::vector<float>{0, 0, 1}
  }),
  rotationValueYMatrix(3, 3, std::vector<std::vector<float>>{
    std::vector<float>{cosf(0), 0, sinf(0)},
    std::vector<float>{0, 1, 0},
    std::vector<float>{-sinf(0), 0, cosf(0)}
  }),
  rotationValueZMatrix(3, 3, std::vector<std::vector<float>>{
    std::vector<float>{1, 0, 0},
    std::vector<float>{0, cosf(0), sinf(0)},
    std::vector<float>{0, -sinf(0), cosf(0)}
  })
{

  assert(cameraFieldOfView>0);
  assert(appScreenWidth>0);
  assert(appScreenHeight>0);

  this->transform(transformX,transformY,transformZ);
  this->rotateX(rotationDegreeX);
  this->rotateY(rotationDegreeY);
  this->rotateZ(rotationDegreeZ);

  DataAccessPoint::getInstance()->getEventHandler().subscribeToEvent<bool>(
    EventHandler::EventName::screenSurfaceChanged,
    cameraName,
    std::bind(&Camera::notifyScreenSurfaceIsChanged,this,std::placeholders::_1)
  );

  initPixelMap();

}

Camera::~Camera() {
  delete[] pixelMap;
}

void Camera::notifyScreenSurfaceIsChanged(
  bool usingNewAppScreenWidthAndHeightIsForced
){

  if (
    this->appScreenWidth == DataAccessPoint::getInstance()->getAppScreenWidth() && 
    this->appScreenHeight == DataAccessPoint::getInstance()->getAppScreenHeight()
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
  if(DEBUG_MODE){
    Logger::log("Initiating pixel map:");
  }
  pixelMap = new DrawPixel[appScreenWidth * appScreenHeight];
  unsigned int rowValue = 0;
  for(unsigned int i=0;i<appScreenWidth;i++){
    rowValue = i * appScreenHeight;
    for(unsigned int j=0;j<appScreenHeight;j++){
      currentPixel = &pixelMap[rowValue + j];
      currentPixel->zValue = zDefaultValue;
      currentPixel->blue = 0;
      currentPixel->green = 0;
      currentPixel->red = 0;
      currentPixel->x = i;
      currentPixel->y = j;
    }
  }
  if(DEBUG_MODE){
    Logger::log("Pixel map is ready");
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

  currentPixel = &pixelMap[x * appScreenHeight + y];
  if(currentPixel->zValue < zValue){
    currentPixel->blue = blue;
    currentPixel->green = green;
    currentPixel->red = red;
    currentPixel->zValue = zValue;
  }
}

void Camera::update(double deltaTime){}

void Camera::render(double deltaTime){
  {//Drawing screen
    gl.beginDrawingPoints();
    unsigned int rowValue = 0;
    for(unsigned int i=0;i<appScreenWidth;i++){
      rowValue = i * appScreenHeight;
      for(unsigned int j=0;j<appScreenHeight;j++){
        currentPixel = &pixelMap[rowValue + j];
        if(currentPixel->blue!=0 || currentPixel->green!=0 || currentPixel->red!=0){
          gl.drawPixel(
            i,
            j,
            currentPixel->red,
            currentPixel->green,
            currentPixel->blue
          );
          currentPixel->blue = 0;
          currentPixel->red = 0;
          currentPixel->green = 0;
        }
        currentPixel->zValue = zDefaultValue;
      }
    }
    gl.resetProgram();
  }
}

float Camera::scaleBasedOnZDistance(float zLocation){
  return cameraFieldOfView/(transformMatrix.get(2, 0) - zLocation);
}

unsigned int Camera::getAppScreenWidth(){
  return appScreenWidth;
}

unsigned int Camera::getAppScreenHeight(){
  return appScreenHeight;
}

//TODO Check this code again
//It must transform based on theta
void Camera::transform(float transformX, float transformY, float transformZ) {
  transformationPlaceholder.set(0, 0, transformX);
  transformationPlaceholder.set(1, 0, transformY);
  transformationPlaceholder.set(2, 0, transformZ);
  
  transformationPlaceholder.multiply(rotationValueXMatrix);
  transformationPlaceholder.multiply(rotationValueYMatrix);
  transformationPlaceholder.multiply(rotationValueZMatrix);

  transformMatrix.sum(transformationPlaceholder);
}

//TODO We can use other ways instead of sin and cos for new matrix calculation
void Camera::rotateX(float x) {
  //For camera we reverse the rotation to apply to pipeline shapes
  rotationDegreeMatrix.set(0, 0, rotationDegreeMatrix.get(0, 0) - x);
  rotationValueXMatrix.set(0, 0, cosf(rotationDegreeMatrix.get(0, 0)));
  rotationValueXMatrix.set(0, 1, -sinf(rotationDegreeMatrix.get(0, 0)));
  rotationValueXMatrix.set(1, 0, sinf(rotationDegreeMatrix.get(0, 0)));
  rotationValueXMatrix.set(1, 1, cosf(rotationDegreeMatrix.get(0, 0)));
}

//TODO We can use other ways instead of sin and cos for new matrix calculation
void Camera::rotateY(float y) {
  //For camera we reverse the rotation to apply to pipeline shapes
  rotationDegreeMatrix.set(1, 0, rotationDegreeMatrix.get(1, 0) - y);
  rotationValueYMatrix.set(0, 0, cosf(rotationDegreeMatrix.get(1, 0)));
  rotationValueYMatrix.set(0, 2, sinf(rotationDegreeMatrix.get(1, 0)));
  rotationValueYMatrix.set(2, 0, -sinf(rotationDegreeMatrix.get(1, 0)));
  rotationValueYMatrix.set(2, 2, cosf(rotationDegreeMatrix.get(1, 0)));
}

//TODO We can use other ways instead of sin and cos for new matrix calculation
void Camera::rotateZ(float z) {
  //For camera we reverse the rotation to apply to pipeline shapes
  rotationDegreeMatrix.set(2, 0, rotationDegreeMatrix.get(2, 0) - z);
  rotationValueZMatrix.set(1, 1, cosf(rotationDegreeMatrix.get(2, 0)));
  rotationValueZMatrix.set(1, 2, sinf(rotationDegreeMatrix.get(2, 0)));
  rotationValueZMatrix.set(2, 1, -sinf(rotationDegreeMatrix.get(2, 0)));
  rotationValueZMatrix.set(2, 2, cosf(rotationDegreeMatrix.get(2, 0)));
}

const MatrixFloat& Camera::getTransformMatrix() {
  return transformMatrix;
}

const MatrixFloat& Camera::getRotationX() {
  return rotationValueXMatrix;
}

const MatrixFloat& Camera::getRotationY() {
  return rotationValueYMatrix;
}

const MatrixFloat& Camera::getRotationZ() {
  return rotationValueZMatrix;
}