#ifndef AndroidEnvironment_class
#define AndroidEnvironment_class

#include <jni.h>
#include <memory>

class AndroidEnvironment{
public:
  static AndroidEnvironment* instance;
private:
  std::unique_ptr<JNIEnv> env;
public:
  static AndroidEnvironment* getInstance();
  std::unique_ptr<JNIEnv>& getEnv();
  AndroidEnvironment(JNIEnv * env);
};

#endif 
