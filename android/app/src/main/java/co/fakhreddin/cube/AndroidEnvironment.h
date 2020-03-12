#ifndef AndroidEnvironment_class
#define AndroidEnvironment_class

#include <jni.h>
#include <memory>
#include <string>

class AndroidEnvironment{
public:
  static AndroidEnvironment* instance;
private:
  std::unique_ptr<JNIEnv> env;
  jclass ndkClass;
  jmethodID loadImageMethodId;
  jmethodID logMethodId;
public:
  static AndroidEnvironment* getInstance();
  std::unique_ptr<JNIEnv>& getEnv();
  unsigned char * loadImage(
    std::string textureAddress,
    int* width,
    int* height,
    int* numberOfChannels
  );
  void log(
    std::string
  );
  AndroidEnvironment(JNIEnv * env);
};

#endif 
