#include "./DataAccessPoint.h"

#include "../open_gl/OpenGl.h"

std::unique_ptr<DataAccessPoint> DataAccessPoint::instance = nullptr;

std::unique_ptr<DataAccessPoint>& DataAccessPoint::getInstance() {

  return instance;

}

void DataAccessPoint::createInstance() {
  if (instance == nullptr) {
    instance = std::make_unique<DataAccessPoint>();
  }
}

DataAccessPoint::DataAccessPoint() {

#ifdef __DESKTOP__
  {//Filling keyMap
    keyMap['a'] = Constants::KeyboardButtons::keyA;
    keyMap['b'] = Constants::KeyboardButtons::keyB;
    keyMap['b'] = Constants::KeyboardButtons::keyB;
    keyMap['c'] = Constants::KeyboardButtons::keyC;
    keyMap['d'] = Constants::KeyboardButtons::keyD;
    keyMap['e'] = Constants::KeyboardButtons::keyE;
    keyMap['f'] = Constants::KeyboardButtons::keyF;
    keyMap['g'] = Constants::KeyboardButtons::keyG;
    keyMap['h'] = Constants::KeyboardButtons::keyH;
    keyMap['i'] = Constants::KeyboardButtons::keyI;
    keyMap['j'] = Constants::KeyboardButtons::keyJ;
    keyMap['k'] = Constants::KeyboardButtons::keyK;
    keyMap['l'] = Constants::KeyboardButtons::keyL;
    keyMap['m'] = Constants::KeyboardButtons::keyM;
    keyMap['n'] = Constants::KeyboardButtons::keyN;
    keyMap['o'] = Constants::KeyboardButtons::keyO;
    keyMap['p'] = Constants::KeyboardButtons::keyP;
    keyMap['q'] = Constants::KeyboardButtons::keyQ;
    keyMap['r'] = Constants::KeyboardButtons::keyR;
    keyMap['s'] = Constants::KeyboardButtons::keyS;
    keyMap['t'] = Constants::KeyboardButtons::keyT;
    keyMap['u'] = Constants::KeyboardButtons::keyU;
    keyMap['v'] = Constants::KeyboardButtons::keyV;
    keyMap['w'] = Constants::KeyboardButtons::keyW;
    keyMap['x'] = Constants::KeyboardButtons::keyX;
    keyMap['y'] = Constants::KeyboardButtons::keyY;
    keyMap['z'] = Constants::KeyboardButtons::keyZ;
    keyMap[0x09] = Constants::KeyboardButtons::tab;
    keyMap[0x25] = Constants::KeyboardButtons::arrowLeft;
    keyMap[0x27] = Constants::KeyboardButtons::arrowRight;
    keyMap[0x26] = Constants::KeyboardButtons::arrowUp;
    keyMap[0x28] = Constants::KeyboardButtons::arrowDown;
  }
#endif // __DESKTOP__

}

#ifdef __DESKTOP__
Constants::KeyboardButtons DataAccessPoint::getKeyCode(const unsigned char& key) {
  return keyMap[std::tolower(key)];
}
#endif // __DESKTOP__


void DataAccessPoint::setAppScreenWidth(const unsigned int appScreenWidth) {
  this->appScreenWidth = appScreenWidth;
}

void DataAccessPoint::setAppScreenHeight(const unsigned int appScreenHeight) {
  this->appScreenHeight = appScreenHeight;
}

void DataAccessPoint::setPhysicalScreenWidth(const unsigned int physicalScreenWidth) {
  this->physicalScreenWidth = physicalScreenWidth;
}

void DataAccessPoint::setPhysicalScreenHeight(const unsigned int physicalScreenHeight) {
  this->physicalScreenHeight = physicalScreenHeight;
}

void DataAccessPoint::setPlatform(Constants::Platform platform) {
  this->platform = platform;
}

const unsigned int& DataAccessPoint::getAppScreenWidth() {
  return this->appScreenWidth;
}

const unsigned int& DataAccessPoint::getAppScreenHeight() {
  return this->appScreenHeight;
}

const unsigned int& DataAccessPoint::getPhysicalScreenWidth() {
  return this->physicalScreenWidth;
}

const unsigned int& DataAccessPoint::getPhysicalScreenHeight() {
  return this->physicalScreenHeight;
}


Constants::Platform DataAccessPoint::getPlatform() {
  return this->platform;
}

EventHandler& DataAccessPoint::getEventHandler() {
  return this->eventHandler;
}

ThreadPool& DataAccessPoint::getThreadPool() {
  return threadPool;
}

void DataAccessPoint::setMousePosition(const int& x,const int& y){
  mousePosition.setX(x);
  mousePosition.setY(y);
}

const Matrix2X1Int& DataAccessPoint::getMousePosition(){
  return mousePosition;
}

const FreeType& DataAccessPoint::getFreeType()
{
  return freeType;
}

Constants::MouseButtonName DataAccessPoint::getMouseButtonName(const int& button){
  switch (button)
  {
  case 0 :
    return Constants::MouseButtonName::left;
  case 1 :
    return Constants::MouseButtonName::right;
  case 2 :
    return Constants::MouseButtonName::middle;
  default:
    //TODO Handle all mouse inputs
    // Logger::exception("DataAccessPoint::getMouseButtonName is unknown");
    return Constants::MouseButtonName::unknown;
  }
}