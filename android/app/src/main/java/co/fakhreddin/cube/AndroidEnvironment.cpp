#include "./AndroidEnvironment.h"

AndroidEnvironment* AndroidEnvironment::instance = nullptr;

AndroidEnvironment::AndroidEnvironment(JNIEnv * env)
  :
    env(std::unique_ptr<JNIEnv>(env))
{
  instance = this;
};

std::unique_ptr<JNIEnv>& AndroidEnvironment::getEnv(){
  return env;
}

AndroidEnvironment* AndroidEnvironment::getInstance(){
  return instance;
}