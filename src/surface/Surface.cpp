#include "./Surface.h"

#include <cassert>
#include "../texture/Texture.h"
#include "../data_types/MatrixTemplate.h"

Surface::Surface(
  LightPrecision lightPercision,
  std::unique_ptr<Texture>& texture,
  const unsigned long& edge1Index,
  const unsigned long& edge2Index,
  const unsigned long& edge3Index
)
  :
  texture(texture),
  lightPercision(lightPercision),
  lineNormalStartValue(3, 1, 0.0f),
  worldPointPlaceholder(3, 1, 0.0f),
  worldNormalPlaceholder(3, 1, 0.0f),
  cameraVector(3, 1, 0.0f),
  colorIntensityOutput(3, 1, 0.0f)
{

  assert(texture);

  colorIntensity[0].reset(3, 1, 0.0f);
  colorIntensity[1].reset(3, 1, 0.0f);
  colorIntensity[2].reset(3, 1, 0.0f);

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
  if (lightPercision == LightPrecision::perSurface) {
    computeColorIntensity(worldPoints, normals, lightSources, cameraInstance);
  }
  computePixelMapData(
    cameraInstance,
    worldPoints,
    normals,
    lightSources
  );
}

void Surface::computePixelMapData(
  Camera& cameraInstance,
  std::vector<MatrixFloat>& worldPoints,
  std::vector<MatrixFloat>& normals,
  std::vector<std::unique_ptr<Light>>& lightSources
){

  assert(lightPercision == LightPrecision::perPixel || lightPercision == LightPrecision::perSurface);

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

  if (lightPercision == LightPrecision::perSurface) {

    lightColorStartR = colorIntensity[0].get(0, 0);
    lightColorStartG = colorIntensity[0].get(1, 0);
    lightColorStartB = colorIntensity[0].get(2, 0);

    lightColorEndR = colorIntensity[1].get(0, 0);
    lightColorEndG = colorIntensity[1].get(1, 0);
    lightColorEndB = colorIntensity[1].get(2, 0);

    lightColorFinalR = colorIntensity[2].get(0, 0);
    lightColorFinalG = colorIntensity[2].get(1, 0);
    lightColorFinalB = colorIntensity[2].get(2, 0);

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
  }
  else if (lightPercision == LightPrecision::perPixel)
  {
    currentNormal = &normals.at(normalVectorIndices[0]);
   
    normalStartX = currentNormal->get(0, 0);
    normalStartY = currentNormal->get(1, 0);
    normalStartZ = currentNormal->get(2, 0);
  
    currentNormal = &normals.at(normalVectorIndices[1]);

    normalEndX = currentNormal->get(0, 0);
    normalEndY = currentNormal->get(1, 0);
    normalEndZ = currentNormal->get(2, 0);

    currentNormal = &normals.at(normalVectorIndices[2]);

    normalFinalX = currentNormal->get(0, 0);
    normalFinalY = currentNormal->get(1, 0);
    normalFinalZ = currentNormal->get(2, 0);

    calculateStepValueBasedOnStepCount(
      normalFinalX - normalStartX,
      totalStepCount,
      &normalStartStepValueX
    );

    calculateStepValueBasedOnStepCount(
      normalFinalY - normalStartY,
      totalStepCount,
      &normalStartStepValueY
    );

    calculateStepValueBasedOnStepCount(
      normalFinalZ - normalStartZ,
      totalStepCount,
      &normalStartStepValueZ
    );

    calculateStepValueBasedOnStepCount(
      normalFinalX - normalEndX,
      totalStepCount,
      &normalEndStepValueX
    );
    
    calculateStepValueBasedOnStepCount(
      normalFinalY - normalEndY,
      totalStepCount,
      &normalEndStepValueY
    );

    calculateStepValueBasedOnStepCount(
      normalFinalZ - normalEndZ,
      totalStepCount,
      &normalEndStepValueZ
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
      lightColorEndB,

      normalStartX,
      normalStartY,
      normalStartZ,

      normalEndX,
      normalEndY,
      normalEndZ,

      lightSources
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

    if (lightPercision == LightPrecision::perSurface) {
      lightColorStartR += lightColorStartStepValueR;
      lightColorEndR += lightColorEndStepValueR;

      lightColorStartG += lightColorStartStepValueG;
      lightColorEndG += lightColorEndStepValueG;

      lightColorStartB += lightColorStartStepValueB;
      lightColorEndB += lightColorEndStepValueB;
    }
    else if (lightPercision == LightPrecision::perPixel)
    {
      normalStartX += normalStartStepValueX;
      normalStartY += normalStartStepValueY;
      normalStartZ += normalStartStepValueZ;

      normalEndX += normalEndStepValueX;
      normalEndY += normalEndStepValueY;
      normalEndZ += normalEndStepValueZ;
    }
  }
}

void Surface::computeColorIntensityForPoint(
  const MatrixFloat& worldPoint,
  const MatrixFloat& worldNormal,
  std::vector<std::unique_ptr<Light>>& lightSources,
  const Camera& cameraInstance,
  MatrixFloat& output
) {
  for (auto& light : lightSources) {

    light->computeLightIntensity(
      worldNormal,
      worldPoint,
      cameraInstance,
      colorIntensityOutput
    );

    assert(colorIntensityOutput.get(0, 0) >= 0);
    assert(colorIntensityOutput.get(0, 0) <= 1);
    assert(colorIntensityOutput.get(1, 0) >= 0);
    assert(colorIntensityOutput.get(1, 0) <= 1);
    assert(colorIntensityOutput.get(2, 0) >= 0);
    assert(colorIntensityOutput.get(2, 0) <= 1);

    output.sum(colorIntensityOutput);

  }
  for (unsigned short int i = 0; i < 3; i++) {
    output.set(i, 0, Math::clamp(output.get(i, 0), 0, 1));
  }
}

void Surface::computeColorIntensity(
  std::vector<MatrixFloat>& worldPoints,
  std::vector<MatrixFloat>& normals,
  std::vector<std::unique_ptr<Light>>& lightSources,
  const Camera& cameraInstance
) {
  if (lightSources.empty() == false) {

    for (short edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++) {
      
      colorIntensity[edgeIndex].set(0, 0, 0.0f);
      colorIntensity[edgeIndex].set(1, 0, 0.0f);
      colorIntensity[edgeIndex].set(2, 0, 0.0f);

      currentWorldPoint = &worldPoints.at(edgeIndices[edgeIndex]);
      currentNormal = &normals.at(normalVectorIndices[edgeIndex]);
      
      computeColorIntensityForPoint(
        worldPoints.at(edgeIndices[edgeIndex]),
        normals.at(normalVectorIndices[edgeIndex]),
        lightSources,
        cameraInstance,
        colorIntensity[edgeIndex]
      );
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
  const Camera& cameraInstance,
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
  const Camera& cameraInstance,
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
  const float& lightColorEndB,

  const float& normalStartX,
  const float& normalStartY,
  const float& normalStartZ,

  const float& normalEndX,
  const float& normalEndY,
  const float& normalEndZ,

  std::vector<std::unique_ptr<Light>>& lightSources

  ) {

  lineTriangleStartX = triangleStartX;
  lineTriangleStartY = triangleStartY;
  lineTriangleStartZ = triangleStartZ;


  {//TriangleStepValue
    lineTextureStartX = textureStartX;
    lineTextureStartY = textureStartY;

    lineXDifference = triangleEndX - lineTriangleStartX;
    lineYDifference = triangleEndY - lineTriangleStartY;
    if (lineXDifference == 0 && lineYDifference == 0) {
      return;
    }
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
      triangleEndZ - lineTriangleStartZ,
      lineTriangleTotalStepCount,
      &lineTriangleZStepValue
    );
  }

  {//TextureStepValue
    calculateStepValueBasedOnStepCount(
      textureEndX - lineTextureStartX,
      lineTriangleTotalStepCount,
      &lineTextureXStepValue
    );
    calculateStepValueBasedOnStepCount(
      textureEndY - lineTextureStartY,
      lineTriangleTotalStepCount,
      &lineTextureYStepValue
    );
  }


  if (lightPercision == LightPrecision::perSurface) {
    lineLightColorStartR = lightColorStartR;
    lineLightColorStartG = lightColorStartG;
    lineLightColorStartB = lightColorStartB;

    {//ColorStepValue
      calculateStepValueBasedOnStepCount(
        lightColorEndR - lightColorStartR,
        lineTriangleTotalStepCount,
        &lineLightColorRStepValue
      );
      calculateStepValueBasedOnStepCount(
        lightColorEndG - lightColorStartG,
        lineTriangleTotalStepCount,
        &lineLightColorGStepValue
      );
      calculateStepValueBasedOnStepCount(
        lightColorEndB - lightColorStartB,
        lineTriangleTotalStepCount,
        &lineLightColorBStepValue
      );
    }
  }
  else if (lightPercision == LightPrecision::perPixel)
  {
    lineNormalStartValue.set(0, 0, normalStartX);
    lineNormalStartValue.set(1, 0, normalStartY);
    lineNormalStartValue.set(2, 0, normalStartZ);

    calculateStepValueBasedOnStepCount(
      normalEndX - normalStartX,
      totalStepCount,
      &lineNormalStepValueX
    );
    calculateStepValueBasedOnStepCount(
      normalEndY - normalStartY,
      totalStepCount,
      &lineNormalStepValueY
    );
    calculateStepValueBasedOnStepCount(
      normalEndZ - normalStartZ,
      totalStepCount,
      &lineNormalStepValueZ
    );
  }
  
  for (unsigned long j = 0; j < lineTriangleTotalStepCount; j++) {
    texture->getPixelForPosition(lineTextureStartX, lineTextureStartY, &red, &green, &blue);

    if (lightPercision == LightPrecision::perSurface) {
      // Multiply color by light value
      red *= lineLightColorStartR;
      green *= lineLightColorStartG;
      blue *= lineLightColorStartB;
    }
    else if(lightPercision == LightPrecision::perPixel)
    {
      worldPointPlaceholder.set(0, 0, lineTriangleStartX);
      worldPointPlaceholder.set(1, 0, lineTriangleStartY);
      worldPointPlaceholder.set(2, 0, lineTriangleStartZ);

      lineNormalStartValue.hat<float>(worldNormalPlaceholder);
      
      colorIntensity->set(0, 0, 0.0f);
      colorIntensity->set(1, 0, 0.0f);
      colorIntensity->set(2, 0, 0.0f);

      computeColorIntensityForPoint(
        worldPointPlaceholder, 
        worldNormalPlaceholder, 
        lightSources, 
        cameraInstance,
        colorIntensity[0]
      );

      red *= colorIntensity[0].get(0, 0);
      green *= colorIntensity[0].get(1, 0);
      blue *= colorIntensity[0].get(2, 0);

    }

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

    if (lightPercision == LightPrecision::perSurface) {
      lineLightColorStartR += lineLightColorRStepValue;
      lineLightColorStartG += lineLightColorGStepValue;
      lineLightColorStartB += lineLightColorBStepValue;
    }
    else if (lightPercision == LightPrecision::perPixel) {
      lineNormalStartValue.set(0, 0, lineNormalStartValue.get(0, 0) + lineNormalStepValueX);
      lineNormalStartValue.set(1, 0, lineNormalStartValue.get(1, 0) + lineNormalStepValueY);
      lineNormalStartValue.set(2, 0, lineNormalStartValue.get(2, 0) + lineNormalStepValueZ);
    }
  }
}
