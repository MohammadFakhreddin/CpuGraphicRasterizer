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

const std::vector<LanguageCharacterSource*>& DataAccessPoint::getSpecialCharacters()
{
  return specialCharactersSource;
}
