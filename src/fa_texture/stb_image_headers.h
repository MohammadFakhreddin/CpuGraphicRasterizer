#ifndef STB_image_header
#define STB_image_header
#include "./../Constants.h"

#ifdef __ANDROID__
  #include "./../../android/app/src/main/java/co/fakhreddin/cube/AndroidEnvironment.h"
  #include "jni.h"
#endif

#include "./open_gl_stb_image.h"
#include <string>
#include <assert.h>
#include <iostream>

class STBImageHelper{
public:
  static unsigned char * loadTexture(
    std::string textureAddress,
    int* width,
    int* height,
    int* numberOfChannels
  ){
    #if defined(__APPLE__) || defined(__PLATFORM_WIN__)
      std::cout<<"Loading image:"<<textureAddress<<std::endl;
      return stbi_load(textureAddress.c_str(), width, height, numberOfChannels, STBI_rgb);
    #elif defined(__ANDROID__)
      auto& environment = AndroidEnvironment::getInstance()->getEnv();

      auto ndkClass = environment->FindClass("co/fakhreddin/cube/NDKHelper");
      auto methodId = environment->GetStaticMethodID(ndkClass, "loadImage", "(Ljava/lang/String;)Ljava/lang/Object;");
      auto jTextureAddress = environment->NewStringUTF(textureAddress.c_str());
      auto imageInformation = environment->CallStaticObjectMethod(ndkClass,methodId,jTextureAddress);
      
      auto imageInformationClass = environment->GetObjectClass(imageInformation);

      auto successFieldId = environment->GetFieldID(imageInformationClass,"success","Z");
      auto widthFieldId = environment->GetFieldID(imageInformationClass,"originalWidth","I");
      auto heightFieldId = environment->GetFieldID(imageInformationClass,"originalHeight","I");
      auto hasAlphaChannelFieldId = environment->GetFieldID(imageInformationClass,"alphaChannel","Z");
      auto imageDataFieldId = environment->GetFieldID(imageInformationClass,"image", "Ljava/lang/Object;");
      
      auto success = environment->GetBooleanField(imageInformation,successFieldId);
      assert(success);

      *width = (int)environment->GetIntField(imageInformation,widthFieldId);
      *height = (int)environment->GetIntField(imageInformation,heightFieldId);
      auto hasAlphaChannel = (bool)environment->GetBooleanField(imageInformation,hasAlphaChannelFieldId);
      *numberOfChannels = 4;//hasAlphaChannel ? 4 : 3;
      auto rawImageObject = environment->GetObjectField(imageInformation,imageDataFieldId);
      unsigned char* data = (unsigned char *)environment->GetDirectBufferAddress(rawImageObject);
      
      return data;
    #endif
    return NULL;
  };
};

#endif