#define STB_IMAGE_IMPLEMENTATION
#include "../../../../../src/fa_texture/stb_image_headers.h"

#include <memory>
#include <jni.h>

#include "./AndroidEnvironment.h"
#include "../../../../../src/application/Application.h"
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
        jint phyiscalScreenHeight
    );
    JNIEXPORT void JNICALL Java_co_fakhreddin_cube_Fa3dCube_step(JNIEnv * env, jobject obj,jlong deltaTime);
};

JNIEXPORT void JNICALL Java_co_fakhreddin_cube_Fa3dCube_init(
    JNIEnv * env, 
    jobject obj,  
    jint phyiscalScreenWidth, 
    jint phyiscalScreenHeight
)
{

    unsigned int deviceScreenWidth = (unsigned int)phyiscalScreenWidth;
    unsigned int deviceScreenHeight = (unsigned int)phyiscalScreenHeight;
    //TODO Implement dynamic resolution
    unsigned int appScreenWidth = (unsigned int)(float(deviceScreenWidth)/7.0f);
    unsigned int appScreenHeight = (unsigned int)(float(appScreenWidth) * (float(deviceScreenHeight)/float(deviceScreenWidth)));

    if(isAppInitOnce){

        androidEnvironment->replaceEnv(env);

        application->notifyScreenSurfaceChanged(appScreenWidth,
                                               appScreenHeight,
                                               deviceScreenWidth,
                                               deviceScreenHeight,
                                               false);

    } else {

        androidEnvironment = std::unique_ptr<AndroidEnvironment>(
                new AndroidEnvironment(env)
        );

        application = std::unique_ptr<Application>(new Application(
                Application::Platform::Android,
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