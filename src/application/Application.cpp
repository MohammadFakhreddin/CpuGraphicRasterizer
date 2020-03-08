#include "Application.h"
#include <memory>
#include "../Constants.h"
#include "../open_gl/OpenGl.h"
#include <vector>
#include "../3d_shape/Shape3d.h"

Application::Application(
	Application::Platform platform,
	unsigned int physicalScreenWidth,
	unsigned int physicalScreenHeight
)
	:
	platform(platform),
	physicalScreenWidth(physicalScreenWidth),
	physicalScreenHeight(physicalScreenHeight)
{
	instance = this;
	shape = Shape3d::generateTextured3DCube(
		dice.diceCubeTexture,
		dice.diceCubeEdgeList,
		100,
		100,
		100,
		Constants::Window::screenWidth/2,
		Constants::Window::screenHeight/2,
		0,
		0,
		0,
		0,
		1
	);
	{//Creating pixelMap
		for(int i=0;i<Constants::Window::screenWidth;i++){
			std::vector<DrawPixel> innerMap;
			pixelMap.emplace_back(innerMap);
			for(int j=0;j<Constants::Window::screenHeight;j++){
				DrawPixel drawPixel;
				drawPixel.zValue = maximumFov * 2;
				pixelMap.at(i).emplace_back(drawPixel);
			}
		}
	}
}

void Application::drawLineBetweenPoints(
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
  if( moveByX == true ){
    float xDifference = endX - startX;
		assert(xDifference!=0);
    float yM = (endY - startY)/xDifference;
    float zM = (endZ - startZ)/xDifference;
    putPixelInMap(round(startX),round(startY),startZ,red,green,blue);
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
		assert(yDifference!=0);
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
      ( stepMoveValue > 0 && startY + stepMoveValue < endY ) || 
			( stepMoveValue <0 && startY - stepMoveValue > endY )
    );
  }
}

void Application::drawTextureBetweenPoints(
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
	float traingleYStepValue = 0;
	float triangleZStepValue = 0;
	{//TriangleStepValue
		if(abs(triangleEndX - triangleStartX) > abs(triangleEndY - triangleStartY)){
			float xDifference = triangleEndX - triangleStartX;
			assert(xDifference!=0);
			triangleXStepValue = (xDifference>0 ? 1:-1) * drawStepValue;
			triangleTotalStepCount = abs(xDifference/Application::drawStepValue);
			assert(triangleTotalStepCount!=0);
			traingleYStepValue = ((triangleEndY - triangleStartY)/xDifference) * triangleXStepValue;
			triangleZStepValue = ((triangleEndZ - triangleStartZ)/xDifference) * triangleXStepValue;
		}else{
			float yDifference = triangleEndY - triangleStartY;
			assert(yDifference!=0);
			triangleTotalStepCount = abs(yDifference/Application::drawStepValue);
			assert(triangleTotalStepCount!=0);
			traingleYStepValue = (yDifference>0 ? 1:-1) * drawStepValue;
			triangleXStepValue = ((triangleEndX - triangleStartX)/yDifference) * traingleYStepValue;
			triangleZStepValue = ((triangleEndZ - triangleStartZ)/yDifference) * traingleYStepValue;
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
		triangleStartY += traingleYStepValue;
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

void Application::putPixelInMap(int x,int y,float zValue,float red,float green,float blue){
	//TODO Optimize and find better solution
	if(x<0||x>=Constants::Window::screenWidth||y<0||y>=Constants::Window::screenHeight){
		return;
	}
	assert(x<Constants::Window::screenWidth);
	assert(y<Constants::Window::screenHeight);
	currentPixel = &pixelMap.at(x).at(y);
	if(currentPixel->zValue>zValue){
		currentPixel->blue = blue;
		currentPixel->green = green;
		currentPixel->red = red;
		currentPixel->zValue = zValue;
	}
} 

void Application::render(float deltaTime) {
	OpenGL::clear();
	{//Drawing screen
		OpenGL::beginDrawingPoints();
		for(unsigned int i=0;i<Constants::Window::screenWidth;i++){
			for(unsigned int j=0;j<Constants::Window::screenHeight;j++){
				currentPixel = &pixelMap.at(i).at(j); 
				if(currentPixel->blue!=0 || currentPixel->green!=0 || currentPixel->red!=0){
					OpenGL::drawPixel(
						i,
						j,
						currentPixel->red,
						currentPixel->green,
						currentPixel->blue
					);
					currentPixel->blue = 0;
					currentPixel->red = 0;
					currentPixel->green = 0;
					currentPixel->zValue = maximumFov * 2;
				}
			}
		}
		OpenGL::end();
	}
	{//FPSText
		OpenGL::drawText(0,0,std::to_string(currentFps),1.0f,1.0f,1.0f);
	}
	// dice.diceCubeTexture->render();
	OpenGL::flush();
}

void Application::update(float deltaTime) {
	if (keyEvents[leftButton]==true) {
		shape->transformX(-1 * Application::shapeTransformSpeed * deltaTime);
		keyEvents[leftButton] = false;
	}
	if (keyEvents[rightButton]==true) {
		shape->transformX(Application::shapeTransformSpeed * deltaTime);
		keyEvents[rightButton] = false;
	}
	if (keyEvents[forwardButton] == true) {
		shape->transformY(Application::shapeTransformSpeed * deltaTime);
		keyEvents[forwardButton] = false;
	}
	if (keyEvents[backwardButton] == true) {
		shape->transformY(-1 * Application::shapeTransformSpeed * deltaTime);
		keyEvents[backwardButton] = false;
	}
  //TODO Add transformZ (It must be like scale factor)
	if (keyEvents[rotationZLeftButton] == true) {
		shape->rotateZ(Application::shapeRotationSpeed * deltaTime);
		keyEvents[rotationZLeftButton] = false;
	}
	if (keyEvents[rotationZRightButton]) {
		shape->rotateZ(-1* Application::shapeRotationSpeed * deltaTime);
		keyEvents[rotationZRightButton] = false;
	}
  if (keyEvents[rotationXLeftButton] == true) {
		shape->rotateX(Application::shapeRotationSpeed * deltaTime);
		keyEvents[rotationXLeftButton] = false;
	}
	if (keyEvents[rotationXRightButton]) {
		shape->rotateX(-1* Application::shapeRotationSpeed * deltaTime);
		keyEvents[rotationXRightButton] = false;
	}
  if (keyEvents[rotationYLeftButton] == true) {
		shape->rotateY(Application::shapeRotationSpeed * deltaTime);
		keyEvents[rotationYLeftButton] = false;
	}
	if (keyEvents[rotationYRightButton]) {
		shape->rotateY(-1* Application::shapeRotationSpeed * deltaTime);
		keyEvents[rotationYRightButton] = false;
	}
	if (keyEvents[zoomInButton]) {
		shape->scale(Application::shapeScaleSpeed * deltaTime);
		keyEvents[zoomInButton] = false;
	}
	if (keyEvents[zoomOutButton]) {
		shape->scale(-1 * Application::shapeScaleSpeed * deltaTime);
		keyEvents[zoomOutButton] = false;
	}
	if(keyEvents[forwardZButton]){
		shape->transformZ(Application::shapeTransformSpeed * deltaTime);
		keyEvents[forwardZButton] = false;
	}
	if(keyEvents[backwardZButton]){
		shape->transformZ(-1 * Application::shapeTransformSpeed * deltaTime);
		keyEvents[backwardZButton] = false;
	}
	shape->update(deltaTime);
}

void Application::notifyKeyIsPressed(Application::Buttons keyEvent)
{
	keyEvents[keyEvent] = true;
}

Application* Application::getInstance()
{
	return Application::instance;
}

void Application::mainLoop(){
	deltaTime = OpenGL::getElapsedTime();
	update(deltaTime);
	render(deltaTime);
	if(deltaTime>0){
		currentFps = 1000.0f/deltaTime;
	}
	#ifdef __APPLE__
		while((openGLError = glGetError()) != GL_NO_ERROR)
		{
			std::cout<<"OpenGLError"<<std::endl<<openGLError<<std::endl;
		}
	#endif
}

Application* Application::instance;
