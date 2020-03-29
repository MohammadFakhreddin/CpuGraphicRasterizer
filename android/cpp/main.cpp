#define TINYOBJLOADER_IMPLEMENTATION
#include "../../src/libs/tiny_obj_loader/tiny_obj_loader.h"

#include <memory>
#include <jni.h>

#include "./AndroidEnvironment.h"
#include "../../src/application/Application.h"
//
// Created by mohammad.fakhreddin on 3/7/20.
//

std::unique_ptr<Application> application;
std::unique_ptr<AndroidEnvironment> androidEnvironment;
bool isAppInitOnce = false;

extern "C" {
    JNIEXPORT void JNICALL Java_co_fakhreddin_cube_Fa3dCube_init(
        JNIEnv * env, 
        jobject obj,  
        jint phyiscalScreenWidth, 
        jint phyiscalScreenHeight,
        jstring applicationAbsolutePath
    );
    JNIEXPORT void JNICALL Java_co_fakhreddin_cube_Fa3dCube_step(JNIEnv * env, jobject obj,jlong deltaTime);
};

JNIEXPORT void JNICALL Java_co_fakhreddin_cube_Fa3dCube_init(
    JNIEnv * env, 
    jobject obj,  
    jint phyiscalScreenWidth, 
    jint phyiscalScreenHeight,
    jstring rawApplicationAbsolutePath
)
{

    unsigned int deviceScreenWidth = (unsigned int)phyiscalScreenWidth;
    unsigned int deviceScreenHeight = (unsigned int)phyiscalScreenHeight;
    //TODO Implement dynamic resolution
    unsigned int appScreenWidth = (unsigned int)(float(deviceScreenWidth)/4.0f);
    unsigned int appScreenHeight = (unsigned int)(float(appScreenWidth) * (float(deviceScreenHeight)/float(deviceScreenWidth)));

    if(isAppInitOnce){

        androidEnvironment->replaceEnv(env);

        application->notifyScreenSurfaceChanged(appScreenWidth,
                                               appScreenHeight,
                                               deviceScreenWidth,
                                               deviceScreenHeight,
                                               false);

    } else {
        //TODO Refactor this part
        jboolean isCopy;
        const char *convertedValue = (env)->GetStringUTFChars(rawApplicationAbsolutePath, &isCopy);
        std::string applicationAbsolutePath = std::string(convertedValue);
        androidEnvironment = std::unique_ptr<AndroidEnvironment>(
                new AndroidEnvironment(
                        env,
                        applicationAbsolutePath)
        );

        application = std::unique_ptr<Application>(new Application(
                Constants::Platform::Android,
                appScreenWidth,
                appScreenHeight,
                deviceScreenWidth,
                deviceScreenHeight
        ));

    }

    isAppInitOnce = true;

}

JNIEXPORT void JNICALL Java_co_fakhreddin_cube_Fa3dCube_step(JNIEnv * env, jobject obj,jlong deltaTime)
{
    //TODO calculate delta time
    application->mainLoop((int)deltaTime);
}