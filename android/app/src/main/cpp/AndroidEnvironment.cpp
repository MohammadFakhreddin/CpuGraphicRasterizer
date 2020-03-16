#include "./AndroidEnvironment.h"

#include <string>
#include <jni.h>
#include <cassert>

AndroidEnvironment* AndroidEnvironment::instance = nullptr;

AndroidEnvironment::AndroidEnvironment(JNIEnv * env)
  :
    env(env)
{
    init();
    instance = this;
};

void AndroidEnvironment::replaceEnv(JNIEnv *env) {
    this->env = env;
    init();
}

void AndroidEnvironment::init(){
    ndkClass = env->FindClass("co/fakhreddin/cube/NDKHelper");
    assert(ndkClass);
    loadImageMethodId = env->GetStaticMethodID(ndkClass, "loadImage", "(Ljava/lang/String;)Ljava/lang/Object;");
    assert(loadImageMethodId);
    logMethodId = env->GetStaticMethodID(ndkClass,"log", "(Ljava/lang/String;)V");
    assert(logMethodId);
}

unsigned char * AndroidEnvironment::loadImage(
  std::string textureAddress,
  int* width,
  int* height,
  int* numberOfChannels
){
  auto jTextureAddress = env->NewStringUTF(textureAddress.c_str());
  auto imageInformation = env->CallStaticObjectMethod(ndkClass,loadImageMethodId,jTextureAddress);
  
  auto imageInformationClass = env->GetObjectClass(imageInformation);

  auto successFieldId = env->GetFieldID(imageInformationClass,"success","Z");
  auto widthFieldId = env->GetFieldID(imageInformationClass,"originalWidth","I");
  auto heightFieldId = env->GetFieldID(imageInformationClass,"originalHeight","I");
  auto hasAlphaChannelFieldId = env->GetFieldID(imageInformationClass,"alphaChannel","Z");
  auto imageDataFieldId = env->GetFieldID(imageInformationClass,"image", "Ljava/lang/Object;");
  
  auto success = env->GetBooleanField(imageInformation,successFieldId);
  assert(success);

  *width = (int)env->GetIntField(imageInformation,widthFieldId);
  *height = (int)env->GetIntField(imageInformation,heightFieldId);
  //TODO Remove this field from data
  //Unused field because all of android bitmaps have 4 channels
  auto hasAlphaChannel = (bool)env->GetBooleanField(imageInformation,hasAlphaChannelFieldId);
  *numberOfChannels = 4;
  auto rawImageObject = env->GetObjectField(imageInformation,imageDataFieldId);
  //TODO This is a memory leak do something about it
  unsigned char* data = (unsigned char *)env->GetDirectBufferAddress(rawImageObject);
  
  return data;
}