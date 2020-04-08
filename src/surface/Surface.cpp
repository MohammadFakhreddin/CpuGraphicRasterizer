#include "./Surface.h"

#include <cassert>
#include "../texture/Texture.h"
#include "../data_types/MatrixTemplate.h"

Surface::Surface(
  std::unique_ptr<Texture>& texture,
  const unsigned long& edge1Index,
  const unsigned long& edge2Index,
  const unsigned long& edge3Index
)
  :
  texture(texture)
{
  
  assert(texture);

  edgeIndices[0] = edge1Index;
  edgeIndices[1] = edge2Index;
  edgeIndices[2] = edge3Index;

  setTextureCoordinates(0, 0, 0);
  setTextureCoordinates(1, 0, 0);
  setTextureCoordinates(2, 0, 0);

}

void Surface::setNormalIndex(const short& edgeNumber, const unsigned long& index) {
  assert(edgeNumber >= 0 && edgeNumber < edgeCount);
  assert(index >= 0);
  normalVectorIndices[edgeNumber] = index;
}

void Surface::setTextureCoordinates(const short& edgeNumber, const float& x, const float& y) {
  assert(edgeNumber >= 0 && edgeNumber < edgeCount);
  textureCoordinate[edgeNumber].setXY(x,y);
}

const unsigned long& Surface::getEdgeByIndex(const unsigned short& index) {

  assert(index < 3);
  
  return edgeIndices[index];

}

const unsigned long& Surface::getNormalIndex(const unsigned short& index) {

  assert(index < 3);

  return normalVectorIndices[index];

}

void Surface::update(
  Camera& cameraInstance,
  std::vector<MatrixFloat>& worldPoints,
  std::vector<MatrixFloat>& normals,
  std::vector<std::unique_ptr<Light>>& lightSources
) {
  //TODO This method is cpu demanding
  if (!isVisibleToCamera(cameraInstance, worldPoints, normals)) {
    return;
  }
  computeColorIntensity(worldPoints,normals,lightSources);
  computePixelMapData(
    cameraInstance,
    worldPoints
  );
}

void Surface::computePixelMapData(
  Camera& cameraInstance,
  std::vector<MatrixFloat>& worldPoints
){

  const auto& point1 = worldPoints.at(edgeIndices[0]);
  const auto& point2 = worldPoints.at(edgeIndices[1]);
  const auto& point3 = worldPoints.at(edgeIndices[2]);

  triangleStartX = point1.get(0, 0);
  triangleStartY = point1.get(1, 0);
  triangleStartZ = point1.get(2, 0);

  triangleEndX = point2.get(0, 0);
  triangleEndY = point2.get(1, 0);
  triangleEndZ = point2.get(2, 0);

  triangleFinalX = point3.get(0, 0);
  triangleFinalY = point3.get(1, 0);
  triangleFinalZ = point3.get(2, 0);

  totalStepCount = 0;

  triangleStartStepValueX = 0;
  triangleStartStepValueY = 0;
  triangleStartStepValueZ = 0;

  triangleEndStepValueX = 0;
  triangleEndStepValueY = 0;
  triangleEndStepValueZ = 0;

  xDifference = triangleFinalX - triangleStartX;
  yDifference = triangleFinalY - triangleStartY;
  if (xDifference == 0 && yDifference == 0) {
    return;
  }
  {//Computing total step count
    if (abs(xDifference) > abs(yDifference)) {
      assert(xDifference != 0);
      calculateStepCount(
        cameraInstance,
        xDifference,
        &totalStepCount
      );
    }
    else {
      assert(yDifference != 0);
      calculateStepCount(
        cameraInstance,
        yDifference,
        &totalStepCount
      );
    }
    if (totalStepCount == 0) {
      return;
    }
  }
  {//Computing triangle
    calculateStepValueBasedOnStepCount(
      xDifference,
      totalStepCount,
      &triangleStartStepValueX
    );
    calculateStepValueBasedOnStepCount(
      yDifference,
      totalStepCount,
      &triangleStartStepValueY
    );
    calculateStepValueBasedOnStepCount(
      triangleFinalZ - triangleStartZ,
      totalStepCount,
      &triangleStartStepValueZ
    );
    calculateStepValueBasedOnStepCount(
      triangleFinalX - triangleEndX,
      totalStepCount,
      &triangleEndStepValueX
    );
    calculateStepValueBasedOnStepCount(
      triangleFinalY - triangleEndY,
      totalStepCount,
      &triangleEndStepValueY
    );
    calculateStepValueBasedOnStepCount(
      triangleFinalZ - triangleEndZ,
      totalStepCount,
      &triangleEndStepValueZ
    );
  }

  textureStartX = textureCoordinate[0].getX();
  textureStartY = textureCoordinate[0].getY();

  textureEndX = textureCoordinate[1].getX();
  textureEndY = textureCoordinate[1].getY();

  textureFinalX = textureCoordinate[2].getX();
  textureFinalY = textureCoordinate[2].getY();

  textureStartStepValueX = 0;
  textureStartStepValueY = 0;

  textureEndStepValueX = 0;
  textureEndStepValueY = 0;

  {//Computing texture step value
    calculateStepValueBasedOnStepCount(
      textureFinalX - textureStartX,
      totalStepCount,
      &textureStartStepValueX
    );
    calculateStepValueBasedOnStepCount(
      textureFinalY - textureStartY,
      totalStepCount,
      &textureStartStepValueY
    );
    calculateStepValueBasedOnStepCount(
      textureFinalX - textureEndX,
      totalStepCount,
      &textureEndStepValueX
    );
    calculateStepValueBasedOnStepCount(
      textureFinalY - textureEndY,
      totalStepCount,
      &textureEndStepValueY
    );
  }

  lightColorStartR = colorIntensity[0].get(0, 0);
  lightColorStartG = colorIntensity[0].get(1, 0);
  lightColorStartB = colorIntensity[0].get(2, 0);

  lightColorEndR = colorIntensity[1].get(0, 0);
  lightColorEndG = colorIntensity[1].get(1, 0);
  lightColorEndB = colorIntensity[1].get(2, 0);
 
  lightColorFinalR = colorIntensity[2].get(0, 0);
  lightColorFinalG = colorIntensity[2].get(1, 0);
  lightColorFinalB = colorIntensity[2].get(2, 0);

  lightColorStartStepValueR = 0.0f;
  lightColorStartStepValueG = 0.0f;
  lightColorStartStepValueB = 0.0f;

  lightColorEndStepValueR = 0.0f;
  lightColorEndStepValueG = 0.0f;
  lightColorEndStepValueB = 0.0f;

  {//Calculating light stepValue
    calculateStepValueBasedOnStepCount(
      lightColorFinalR - lightColorStartR,
      totalStepCount,
      &lightColorStartStepValueR
      );
    calculateStepValueBasedOnStepCount(
      lightColorFinalG - lightColorStartG,
      totalStepCount,
      &lightColorStartStepValueG
      );
    calculateStepValueBasedOnStepCount(
      lightColorFinalB - lightColorStartB,
      totalStepCount,
      &lightColorStartStepValueB
      );
    calculateStepValueBasedOnStepCount(
      lightColorFinalR - lightColorEndR,
      totalStepCount,
      &lightColorEndStepValueR
      );
    calculateStepValueBasedOnStepCount(
      lightColorFinalG - lightColorEndG,
      totalStepCount,
      &lightColorEndStepValueG
      );
    calculateStepValueBasedOnStepCount(
      lightColorFinalB - lightColorEndB,
      totalStepCount,
      &lightColorEndStepValueB
      );
  }

  for (unsigned long i = 0; i < totalStepCount; i++) {
    
    drawLineBetweenPoints(
      cameraInstance,

      triangleStartX,
      triangleStartY,
      triangleStartZ,
      
      triangleEndX,
      triangleEndY,
      triangleEndZ,
      
      textureStartX,
      textureStartY,
      
      textureEndX,
      textureEndY,
    
      lightColorStartR,
      lightColorStartG,
      lightColorStartB,

      lightColorEndR,
      lightColorEndG,
      lightColorEndB
    );
    
    triangleStartX += triangleStartStepValueX;
    triangleEndX += triangleEndStepValueX;

    triangleStartZ += triangleStartStepValueZ;
    triangleEndZ += triangleEndStepValueZ;

    triangleStartY += triangleStartStepValueY;
    triangleEndY += triangleEndStepValueY;
    
    textureStartX += textureStartStepValueX;
    textureEndX += textureEndStepValueX;
    
    textureStartY += textureStartStepValueY;
    textureEndY += textureEndStepValueY;

    lightColorStartR += lightColorStartStepValueR;
    lightColorEndR += lightColorEndStepValueR;

    lightColorStartG += lightColorStartStepValueG;
    lightColorEndG += lightColorEndStepValueG;
    
    lightColorStartB += lightColorStartStepValueB;
    lightColorEndB += lightColorEndStepValueB;

  }
}

void Surface::computeColorIntensity(
  std::vector<MatrixFloat>& worldPoints,
  std::vector<MatrixFloat>& normals,
  std::vector<std::unique_ptr<Light>>& lightSources
) {
  if (lightSources.empty() == false) {

    for (short edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++) {
      
      colorIntensity[edgeIndex].set(0, 0, 0.0f);
      colorIntensity[edgeIndex].set(1, 0, 0.0f);
      colorIntensity[edgeIndex].set(2, 0, 0.0f);

      currentWorldPoint = &worldPoints.at(edgeIndices[edgeIndex]);
      currentNormal = &normals.at(normalVectorIndices[edgeIndex]);
      
      for (auto& light : lightSources) {
        
        light->computeLightIntensity(
          *currentNormal,
          *currentWorldPoint,
          colorIntensityOutput
        );
        
        for (short i = 0; i < 3; i++) {
          colorIntensity[edgeIndex].set(
            i, 
            0, 
            Math::max(
              colorIntensityOutput.get(i,0),
              colorIntensity[edgeIndex].get(i, 0)
            )
          );
        }
      }
    }
  } else {
    
    for (unsigned short edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++) {
    
      for (unsigned short i = 0; i < 3; i++) {
      
        colorIntensity[edgeIndex].set(i, 0, 1.0f);
      
      }
    }
  }
}

bool Surface::areEdgeAndNormalsValid(
  const unsigned long& worldPointsSize,
  const unsigned long& normalsSize
) {
  for (unsigned short i = 0; i < 3; i++) {
    if (
      edgeIndices[i] < 0 ||
      edgeIndices[i] >= worldPointsSize ||
      normalVectorIndices[i] < 0 ||
      normalVectorIndices[i] >= normalsSize
    ) {
      return false;
    }
  }
  return true;
}

void Surface::calculateStepCount(
  Camera& cameraInstance,
  float difference,
  unsigned int* totalStepCount
){
  assert(difference != 0 && "Difference must be above 0 in BaseSurface::calculateStepCountAndStepValue");
  assert(drawStepValue != 0 && "Draw step value must be above 0 in BaseSurface::calculateStepCountAndStepValue");
  *totalStepCount = Math::min(
    (unsigned int)abs(difference/drawStepValue) + 1,
    cameraInstance.getAppScreenWidth()
  );
}

void Surface::calculateStepValueBasedOnStepCount(
  const float& difference,
  const unsigned int& stepCount,
  float* stepValue
) {
  assert(stepCount > 0);
  *stepValue = difference / float(stepCount);
}

bool Surface::isVisibleToCamera(
  Camera& cameraInstance,
  std::vector<MatrixFloat>& worldPoints,
  std::vector<MatrixFloat>& normals
) {
    
  cameraLeft = 0;
  cameraRight = cameraInstance.getAppScreenWidth();
  cameraTop = 0;
  cameraBottom = cameraInstance.getAppScreenHeight();

  isShapeCompletlyOutOfCamera = true;
  //Need to clip when shape is out of camera
  for (unsigned short i = 0; i < edgeCount; i++) {
    
    currentWorldPoint = &worldPoints.at(edgeIndices[i]);
    
    if (
      currentWorldPoint->get(0, 0) >= cameraLeft &&
      currentWorldPoint->get(0, 0) < cameraRight &&
      currentWorldPoint->get(1, 0) >= cameraTop &&
      currentWorldPoint->get(1, 0) < cameraBottom
    ) {
  
      isShapeCompletlyOutOfCamera = false;
      break;
    
    }
  
  }

  if (isShapeCompletlyOutOfCamera == true) {
    return false;
  }

  for (unsigned int i = 0; i < edgeCount; i++) {
    
    currentWorldPoint = &worldPoints.at(edgeIndices[i]);
    
    //Making camera vector
    cameraVector.set(
      0, 
      0, 
      Math::clamp(
        currentWorldPoint->get(0, 0),
        cameraLeft,
        cameraRight
      ) - currentWorldPoint->get(0, 0)
    );
    cameraVector.set(
      1,
      0,
      Math::clamp(
        currentWorldPoint->get(1, 0),
        cameraTop,
        cameraBottom
      ) - currentWorldPoint->get(1, 0)
    );
    cameraVector.set(2, 0, currentWorldPoint->get(2, 0));

    dotProductValue = normals.at(normalVectorIndices[i]).dotProduct(cameraVector);
    if (dotProductValue <= 0.0f) {
      return true;
    }

  }
  return false;
}

void Surface::drawLineBetweenPoints(
  Camera& cameraInstance,
  
  const float& triangleStartX,
  const float& triangleStartY,
  const float& triangleStartZ,
  
  const float& triangleEndX,
  const float& triangleEndY,
  const float& triangleEndZ,
  
  const float& textureStartX,
  const float& textureStartY,
  
  const float& textureEndX,
  const float& textureEndY,
  
  const float& lightColorStartR,
  const float& lightColorStartG,
  const float& lightColorStartB,

  const float& lightColorEndR,
  const float& lightColorEndG,
  const float& lightColorEndB

  ) {

  lineTriangleStartX = triangleStartX;
  lineTriangleStartY = triangleStartY;
  lineTriangleStartZ = triangleStartZ;

  lineTriangleEndX = triangleEndX;
  lineTriangleEndY = triangleEndY;
  lineTriangleEndZ = triangleEndZ;

  lineTextureStartX = textureStartX;
  lineTextureStartY = textureStartY;

  lineTextureEndX = textureEndX;
  lineTextureEndY = textureEndY;

  lineLightColorStartR = lightColorStartR;
  lineLightColorStartG = lightColorStartG;
  lineLightColorStartB = lightColorStartB;

  lineLightColorEndR = lightColorEndR;
  lineLightColorEndG = lightColorEndG;
  lineLightColorEndB = lightColorEndB;

  lineTriangleTotalStepCount = 0;
  lineTriangleXStepValue = 0;
  lineTriangleYStepValue = 0;
  lineTriangleZStepValue = 0;

  lineXDifference = lineTriangleEndX - lineTriangleStartX;
  lineYDifference = lineTriangleEndY - lineTriangleStartY;
  if (lineXDifference == 0 && lineYDifference == 0) {
    return;
  }
  {//TriangleStepValue
    if (abs(lineXDifference) > abs(lineYDifference)) {
      assert(lineXDifference != 0);
      calculateStepCount(
        cameraInstance,
        lineXDifference,
        &lineTriangleTotalStepCount
      );
    }
    else {
      assert(lineYDifference != 0);
      calculateStepCount(
        cameraInstance,
        lineYDifference,
        &lineTriangleTotalStepCount
      );
    }
    if (lineTriangleTotalStepCount == 0) {
      return;
    }
    calculateStepValueBasedOnStepCount(
      lineXDifference,
      lineTriangleTotalStepCount,
      &lineTriangleXStepValue
    );
    calculateStepValueBasedOnStepCount(
      lineYDifference,
      lineTriangleTotalStepCount, 
      &lineTriangleYStepValue
    );
    calculateStepValueBasedOnStepCount(
      (lineTriangleEndZ - lineTriangleStartZ),
      lineTriangleTotalStepCount,
      &lineTriangleZStepValue
    );
  }

  lineTextureXStepValue = 0;
  lineTextureYStepValue = 0;
  {//TextureStepValue
    calculateStepValueBasedOnStepCount(
      lineTextureEndX - lineTextureStartX,
      lineTriangleTotalStepCount,
      &lineTextureXStepValue
    );
    calculateStepValueBasedOnStepCount(
      lineTextureEndY - lineTextureStartY,
      lineTriangleTotalStepCount,
      &lineTextureYStepValue
    );
  }

  lineLightColorRStepValue = 0;
  lineLightColorGStepValue = 0;
  lineLightColorBStepValue = 0;
  {//ColorStepValue
    calculateStepValueBasedOnStepCount(
      lineLightColorEndR - lineLightColorStartR,
      lineTriangleTotalStepCount,
      &lineLightColorRStepValue
    );
    calculateStepValueBasedOnStepCount(
      lineLightColorEndG - lineLightColorStartG,
      lineTriangleTotalStepCount,
      &lineLightColorGStepValue
    );
    calculateStepValueBasedOnStepCount(
      lineLightColorEndB - lineLightColorStartB,
      lineTriangleTotalStepCount,
      &lineLightColorBStepValue
    );
  }

  for (unsigned long j = 0; j < lineTriangleTotalStepCount; j++) {
    texture->getPixelForPosition(lineTextureStartX, lineTextureStartY, &red, &green, &blue);

    // Multiply color by light value
    red *= lineLightColorStartR;
    green *= lineLightColorStartG;
    blue *= lineLightColorStartB;

    cameraInstance.putPixelInMap(
      int(lineTriangleStartX),
      int(lineTriangleStartY),
      lineTriangleStartZ,
      Math::max(red, 0.0f),
      Math::max(green, 0.0f),
      Math::max(blue, 0.0f)
    );

    lineTriangleStartX += lineTriangleXStepValue;
    lineTriangleStartY += lineTriangleYStepValue;
    lineTriangleStartZ += lineTriangleZStepValue;

    lineTextureStartX += lineTextureXStepValue;
    lineTextureStartY += lineTextureYStepValue;

    lineLightColorStartR += lineLightColorRStepValue;
    lineLightColorStartG += lineLightColorGStepValue;
    lineLightColorStartB += lineLightColorBStepValue;
  }
}
