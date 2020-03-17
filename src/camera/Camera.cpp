#include "./Camera.h"

#include <cassert>

Camera::Camera(
    OpenGL& openGLInstance,
    float cameraZLocation,
    float cameraFieldOfView,
    float left,
    float right,
    float top,
    float bottom,
    unsigned int appScreenWidth,
    unsigned int appScreenHeight
) :
openGLInstance(openGLInstance),
cameraZLocation(cameraZLocation),
cameraFieldOfView(cameraFieldOfView),
left(left),
right(right),
top(top),
bottom(bottom),
appScreenWidth(appScreenWidth),
appScreenHeight(appScreenHeight)
{
    initPixelMap();
}

void Camera::notifyScreenSurfaceIsChanged(
    float left,
    float right,
    float top,
    float bottom,
    unsigned int appScreenWidth,
    unsigned int appScreenHeight    
){
    this->cameraZLocation = cameraZLocation;
    this->cameraFieldOfView = cameraFieldOfView;
    this->left = left;
    this->right = right;
    this->top = top;
    this->bottom = bottom;
    this->appScreenWidth = appScreenWidth;
    this->appScreenHeight = appScreenHeight;

    pixelMap.erase(pixelMap.begin(),pixelMap.end());
    initPixelMap();
}

void Camera::initPixelMap(){
   	Logger::log("Initiating pixel map:");
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
    Logger::log("Pixel map is ready");
}

void Camera::drawLineBetweenPoints(
  float startX,
  float startY,
  float startZ,
  float endX,
  float endY,
  float endZ,
  float red,
  float green,
  float blue
){
  bool moveByX = true;
  if(abs(startX-endX)<abs(startY-endY)){
    moveByX = false;
  }
  if(moveByX){
    float xDifference = endX - startX;
	if(xDifference==0){
		return;
	}
	float yM = (endY - startY)/xDifference;
    float zM = (endZ - startZ)/xDifference;
    putPixelInMap(static_cast<int>(round(startX)), static_cast<int>(round(startY)), startZ, red, green, blue);
    float stepMoveValue = startX - endX > 0 ? -1 : +1;
	do{
		startX += stepMoveValue;
		startY += yM * stepMoveValue;
		startZ += zM * stepMoveValue;
		putPixelInMap(int(round(startX)),int(round(startY)),startZ,red,green,blue);
	}while (
		( stepMoveValue > 0 && startX + stepMoveValue < endX ) || 
		( stepMoveValue < 0 && startX - stepMoveValue > endX )
	);
  } else {
    float yDifference = endY - startY;
	if(yDifference==0){
		return;
	}
    float xM = (endX - startX)/yDifference;
    float zM = (endZ - startZ)/yDifference;
    putPixelInMap(int(round(startX)),int(round(startY)),startZ,red,green,blue);
    float stepMoveValue = startY - endY > 0 ? -1 : +1;
	do{
		startY += stepMoveValue;
      	startX += xM * stepMoveValue;
      	startZ += zM * stepMoveValue;
      	putPixelInMap(int(round(startX)),int(round(startY)),startZ,red,green,blue);
	}while (
      	(stepMoveValue > 0 && startY + stepMoveValue < endY) ||
		( stepMoveValue <0 && startY - stepMoveValue > endY )
    );
  }
}

void Camera::drawTextureBetweenPoints(
	std::unique_ptr<FaTexture>& texture,
	float triangleStartX,
	float triangleStartY,
	float triangleStartZ,
	float triangleEndX,
	float triangleEndY,
	float triangleEndZ,
	float textureStartX,
	float textureStartY,
	float textureEndX,
	float textureEndY
){

	float triangleTotalStepCount = 0;
	float triangleXStepValue = 0;
	float triangleYStepValue = 0;
	float triangleZStepValue = 0;
	{//TriangleStepValue
		if(abs(triangleEndX - triangleStartX) > abs(triangleEndY - triangleStartY)){
			float xDifference = triangleEndX - triangleStartX;
			assert(xDifference!=0);
			triangleXStepValue = calculateStepValue(xDifference);
			triangleTotalStepCount = calculateStepCount(xDifference);
			assert(triangleTotalStepCount!=0);
            triangleYStepValue = ((triangleEndY - triangleStartY)/xDifference) * triangleXStepValue;
			triangleZStepValue = ((triangleEndZ - triangleStartZ)/xDifference) * triangleXStepValue;
		}else{
			float yDifference = triangleEndY - triangleStartY;
			assert(yDifference!=0);
			triangleTotalStepCount = calculateStepCount(yDifference);
			assert(triangleTotalStepCount!=0);
            triangleYStepValue = calculateStepValue(yDifference);
			triangleXStepValue = ((triangleEndX - triangleStartX)/yDifference) * triangleYStepValue;
			triangleZStepValue = ((triangleEndZ - triangleStartZ)/yDifference) * triangleYStepValue;
		}
	}

	float textureXStepValue = 0;
	float textureYStepValue = 0;
	{//TextureStepValue
		if(abs(textureEndX - textureStartX)>abs(textureEndY - textureStartY)){
			float xDifference = textureEndX - textureStartX;
			assert(xDifference!=0);
			textureXStepValue = xDifference/triangleTotalStepCount;
			textureYStepValue = ((textureEndY - textureStartY)/xDifference) * textureXStepValue;
		}else
		{
			float yDifference = textureEndY - textureStartY;
			assert(yDifference!=0);
			textureYStepValue = yDifference/triangleTotalStepCount;
			textureXStepValue = ((textureEndX - textureStartX)/yDifference) * textureYStepValue;
		}
	}
	
	float red = 0;
	float green = 0;
	float blue = 0;

	texture->getColorForPosition(textureStartX,textureStartY,&red,&green,&blue);
	putPixelInMap(
		int(floor(triangleStartX)),
		int(floor(triangleStartY)),
		triangleStartZ,
		red,
		green,
		blue
	);
	for(int i=0;i<triangleTotalStepCount;i++){
		triangleStartX += triangleXStepValue;
		triangleStartY += triangleYStepValue;
		triangleStartZ += triangleZStepValue;
		textureStartX += textureXStepValue;
		textureStartY += textureYStepValue;
		texture->getColorForPosition(textureStartX,textureStartY,&red,&green,&blue);
		putPixelInMap(
			int(floor(triangleStartX)),
			int(floor(triangleStartY)),
			triangleStartZ,
			red,
			green,
			blue
		);
	}
}

void Camera::putPixelInMap(int x,int y,float zValue,float red,float green,float blue){
	if(
		zValue <= cameraZLocation || 
		zValue >= cameraFieldOfView ||  
		x < left ||
		x >= right ||
		y < left ||
		y >= right
	){
		return;
	}
	currentPixel = &pixelMap.at(x).at(y);
	if(currentPixel->zValue == 0 || currentPixel->zValue > zValue){
		currentPixel->blue = blue;
		currentPixel->green = green;
		currentPixel->red = red;
		currentPixel->zValue = zValue;
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
					currentPixel->zValue = cameraZLocation;
				}
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

unsigned int Camera::calculateStepCount(float difference){
	return ceil(abs(difference/Camera::drawStepValue));
}

float Camera::calculateStepValue(float difference){
	return (difference > 0 ? 1 : -1) * Camera::drawStepValue;
}

float Camera::getCameraZLocation(){
	return cameraZLocation;
}