#include <memory>
#include <jni.h>
#include "../../../../../../../../src/open_gl/OpenGl.h"
#include "../../../../../../../../src/application/Application.h"
#include "../../../../../../../../src/Constants.h"
//
// Created by mohammad.fakhreddin on 3/7/20.
//

std::unique_ptr<Application> application;

extern "C" {
    JNIEXPORT void JNICALL Java_co_fakhreddin_cube_Fa3dCube_init(
        JNIEnv * env, 
        jobject obj,  
        jint phyiscalScreenWidth, 
        jint phyiscalScreenHeight
    );
    JNIEXPORT void JNICALL Java_co_fakhreddin_cube_Fa3dCube_step(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_co_fakhreddin_cube_Fa3dCube_init(
    JNIEnv * env, 
    jobject obj,  
    jint phyiscalScreenWidth, 
    jint phyiscalScreenHeight
)
{
    float screenRatio = phyiscalScreenWidth/phyiscalScreenHeight;
    Constants::Window::screenHeight = screenRatio * Constants::Window::screenWidth;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glViewport(0,0,Constants::Window::screenWidth,Constants::Window::screenHeight);
    // glOrtho(-0.5f, Constants::Window::screenWidth-0.5f, -0.5f, Constants::Window::screenHeight-0.5f, -1.0, 1.0);
    application = std::unique_ptr<Application>(new Application(
        Application::Platform::Mac,
        phyiscalScreenWidth,
        phyiscalScreenHeight
    ));
}

JNIEXPORT void JNICALL Java_co_fakhreddin_cube_Fa3dCube_step(JNIEnv * env, jobject obj)
{
    application->mainLoop();
}