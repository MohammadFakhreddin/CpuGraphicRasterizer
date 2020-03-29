
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
    keyMap['a'] = Constants::Buttons::keyA;
    keyMap['b'] = Constants::Buttons::keyB;
    keyMap['b'] = Constants::Buttons::keyB;
    keyMap['c'] = Constants::Buttons::keyC;
    keyMap['d'] = Constants::Buttons::keyD;
    keyMap['e'] = Constants::Buttons::keyE;
    keyMap['f'] = Constants::Buttons::keyF;
    keyMap['g'] = Constants::Buttons::keyG;
    keyMap['h'] = Constants::Buttons::keyH;
    keyMap['i'] = Constants::Buttons::keyI;
    keyMap['j'] = Constants::Buttons::keyJ;
    keyMap['k'] = Constants::Buttons::keyK;
    keyMap['l'] = Constants::Buttons::keyL;
    keyMap['m'] = Constants::Buttons::keyM;
    keyMap['n'] = Constants::Buttons::keyN;
    keyMap['o'] = Constants::Buttons::keyO;
    keyMap['p'] = Constants::Buttons::keyP;
    keyMap['q'] = Constants::Buttons::keyQ;
    keyMap['r'] = Constants::Buttons::keyR;
    keyMap['s'] = Constants::Buttons::keyS;
    keyMap['t'] = Constants::Buttons::keyT;
    keyMap['u'] = Constants::Buttons::keyU;
    keyMap['v'] = Constants::Buttons::keyV;
    keyMap['w'] = Constants::Buttons::keyW;
    keyMap['x'] = Constants::Buttons::keyX;
    keyMap['y'] = Constants::Buttons::keyY;
    keyMap['z'] = Constants::Buttons::keyZ;
    keyMap[VK_TAB] = Constants::Buttons::tab;
    keyMap[VK_LEFT] = Constants::Buttons::arrowLeft;
    keyMap[VK_RIGHT] = Constants::Buttons::arrowRight;
    keyMap[VK_UP] = Constants::Buttons::arrowUp;
    keyMap[VK_DOWN] = Constants::Buttons::arrowDown;
  }
#endif // __DESKTOP__

}

#ifdef __DESKTOP__
Constants::Buttons DataAccessPoint::getKeyCode(const unsigned char& key) {
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