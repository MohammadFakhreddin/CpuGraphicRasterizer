#include "./Camera.h"

#include <cassert>

#include "./../utils/log/Logger.h"
#include "./../utils/math/Math.h"

Camera::Camera(
    OpenGL& openGLInstance,
	Light& lightInstance,
    float cameraZLocation,
    float cameraFieldOfView,
    float left,
    float right,
    float top,
    float bottom
) :
openGLInstance(openGLInstance),
lightInstance(lightInstance),
cameraZLocation(cameraZLocation),
cameraFieldOfView(cameraFieldOfView),
left(left),
right(right),
top(top),
bottom(bottom),
appScreenWidth( (unsigned int)(right - left) ),
appScreenHeight( (unsigned int)(bottom - top) )
{
	
	if(DEBUG_MODE==true){
		Logger::log("==========================");
		Logger::log("Camera information:");
		Logger::log("Left: "+std::to_string(left));
		Logger::log("Right: "+std::to_string(right));
		Logger::log("Top: "+std::to_string(top));
		Logger::log("Bottom: "+std::to_string(bottom));
		Logger::log("CameraZLocation: "+ std::to_string(cameraZLocation));
		Logger::log("FOV: "+std::to_string(cameraFieldOfView));
		Logger::log("AppScreenWidth: "+std::to_string(appScreenWidth));
		Logger::log("AppScreenHeight: "+std::to_string(appScreenHeight));
		Logger::log("==========================");
	}

	assert(bottom>top);
	assert(right>left);
	assert(appScreenWidth>0);
	assert(appScreenHeight>0); 

    initPixelMap();
}

void Camera::notifyScreenSurfaceIsChanged(
    float left,
    float right,
    float top,
    float bottom
){
    this->left = left;
    this->right = right;
    this->top = top;
    this->bottom = bottom;
    this->appScreenWidth = (unsigned int)(right - left);
    this->appScreenHeight = (unsigned int)(bottom - top);

	assert(right>left);
	assert(bottom>top);
	assert(appScreenWidth>0);
	assert(appScreenHeight>0);

    pixelMap.erase(pixelMap.begin(),pixelMap.end());
    initPixelMap();
}

void Camera::initPixelMap(){
	if(DEBUG_MODE){
   		Logger::log("Initiating pixel map:");
	}
    for(int i=0;i<appScreenWidth;i++){
        std::vector<DrawPixel> innerMap;
        pixelMap.emplace_back(innerMap);
        for(int j=0;j<appScreenHeight;j++){
            DrawPixel drawPixel;
            drawPixel.zValue = cameraZLocation;
            drawPixel.blue = 0;
            drawPixel.green = 0;
            drawPixel.red = 0;
            pixelMap.at(i).emplace_back(drawPixel);
        }
    }
	if(DEBUG_MODE){
    	Logger::log("Pixel map is ready");
	}
}

void Camera::putPixelInMap(int x,int y,float zValue,float red,float green,float blue){
	//TODO Maybe we need cliping before inserting into pixel array
	if(
		zValue <= cameraZLocation || 
		zValue >= cameraFieldOfView ||  
		x < left ||
		x >= right ||
		y < top ||
		y >= bottom
	){
		return;
	}
	assert(x>=left && x<right);
	assert(y>=top && y<bottom);
	currentPixel = &pixelMap.at((unsigned int)x).at((unsigned int)y);
	if(currentPixel->zValue == 0 || currentPixel->zValue > zValue){
		currentPixel->blue = blue;
		currentPixel->green = green;
		currentPixel->red = red;
		currentPixel->zValue = zValue;
	}
}

void Camera::update(double deltaTime){
	drawLight();	
}
//TODO Light bulb must be shape
void Camera::drawLight(){
	float radius = 10.0f;
	for(int i=-radius;i<radius;i++){
		for(int j=-radius;j<radius;j++){
			putPixelInMap(
				lightInstance.getLightPosition().getX() + i,
				lightInstance.getLightPosition().getY() + j,
				lightInstance.getLightPosition().getZ(),
				1.0f,
				1.0f,
				1.0f
			);
		}
	}
}

void Camera::render(double deltaTime){
    {//Drawing screen
		openGLInstance.beginDrawingPoints();
		for(unsigned int i=0;i<appScreenWidth;i++){
			for(unsigned int j=0;j<appScreenHeight;j++){
				currentPixel = &pixelMap.at(i).at(j); 
				if(currentPixel->blue!=0 || currentPixel->green!=0 || currentPixel->red!=0){
					openGLInstance.drawPixel(
						i,
						j,
						currentPixel->red,
						currentPixel->green,
						currentPixel->blue
					);
					currentPixel->blue = 0;
					currentPixel->red = 0;
					currentPixel->green = 0;
				}
				currentPixel->zValue = cameraZLocation;
			}
		}
		openGLInstance.resetProgram();
	}
}

float Camera::scaleBasedOnZDistance(float zLocation){
	return cameraFieldOfView/(zLocation - cameraZLocation);
}

float Camera::getLeft(){
    return left;
}

float Camera::getRight(){
    return right;
}

float Camera::getTop(){
    return top;
}

float Camera::getBottom(){
    return bottom;
}

float Camera::getCameraZLocation(){
	return cameraZLocation;
}

unsigned int Camera::getAppScreenWidth(){
	return appScreenWidth;
}

unsigned int Camera::getAppScreenHeight(){
	return appScreenHeight;
}

Light& Camera::getLight(){
	return lightInstance;
}