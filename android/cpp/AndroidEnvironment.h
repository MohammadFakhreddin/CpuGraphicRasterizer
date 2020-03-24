#ifndef AndroidEnvironment_class
#define AndroidEnvironment_class

#include <jni.h>
#include <memory>
#include <string>

class AndroidEnvironment{
private:
  static AndroidEnvironment* instance;
  void init();
private:
  JNIEnv* env;
  jclass ndkClass;
  jmethodID loadImageMethodId;
  jmethodID logMethodId;
  jmethodID loadTextMethodId;
public:
  static AndroidEnvironment* getInstance(){
    return instance;
  };
  unsigned char * loadImage(
    std::string textureAddress,
    int* width,
    int* height,
    int* numberOfChannels
  );
  unsigned char * loadText(std::string textFileAddress);
  void log(
    std::string text
  ){
    auto jText = env->NewStringUTF(text.c_str());
    env->CallStaticVoidMethod(ndkClass,logMethodId,jText);
  };
  AndroidEnvironment(JNIEnv * env);
  void replaceEnv(JNIEnv * env);
};

#endif 
