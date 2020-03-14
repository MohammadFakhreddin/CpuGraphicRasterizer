#ifndef AndroidEnvironment_class
#define AndroidEnvironment_class

#include <jni.h>
#include <memory>
#include <string>

class AndroidEnvironment{
private:
  static AndroidEnvironment* instance;
private:
  std::unique_ptr<JNIEnv> env;
  jclass ndkClass;
  jmethodID loadImageMethodId;
  jmethodID logMethodId;
public:
  static AndroidEnvironment* getInstance(){
    return instance;
  };
  std::unique_ptr<JNIEnv>& getEnv();
  unsigned char * loadImage(
    std::string textureAddress,
    int* width,
    int* height,
    int* numberOfChannels
  );
  void log(
    std::string text
  ){
    auto jText = env->NewStringUTF(text.c_str());
    env->CallStaticVoidMethod(ndkClass,logMethodId,jText);
  };
  AndroidEnvironment(JNIEnv * env);
};

#endif 
