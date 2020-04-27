#include "./Pipeline.h"

#include "../camera/Camera.h"
#include "../shaders/ambient_light/AmbientLight.h"
#include "../shaders/directional_light/DirectionalLight.h"
#include "../shaders/point_light/PointLight.h"
#include "../data_access_point/DataAccessPoint.h"

PipeLine::PipeLine(
  Camera& camera
) 
  :
  ambientLight(nullptr),
  threadPool(DataAccessPoint::getInstance()->getThreadPool()),
  camera(camera),
  numberOfSupportedThreads(threadPool.getNumberOfAvailableThreads()),
  shapes(std::move(shapes))
{}

PipeLine::PipeLine(
  Camera& camera,
  AmbientLight* ambientLight,
  std::vector<DirectionalLight*>& directionalLights,
  std::vector<PointLight*>& pointLights,
  std::vector<Shape3d*>& shapes
)
  :
  threadPool(DataAccessPoint::getInstance()->getThreadPool()),
  camera(camera),
  ambientLight(ambientLight),
  pointLights(std::move(pointLights)),
  directionalLights(std::move(directionalLights)),
  numberOfSupportedThreads(threadPool.getNumberOfAvailableThreads()),
  shapes(std::move(shapes))
{}

void PipeLine::assignAmbientLight(AmbientLight* ambientLight) {
  this->ambientLight = ambientLight;
}

void PipeLine::assignDirectionalLight(std::vector<DirectionalLight*>& directionalLights) {
  assert(directionalLights.empty() == false);
  for (auto& light : directionalLights) {
    this->directionalLights.emplace_back(light);
  }
}

void PipeLine::assignPointLight(std::vector<PointLight*>& pointLights) {
  assert(pointLights.empty() == false);
  for (auto& light : pointLights) {
    this->pointLights.emplace_back(light);
  }
}

void PipeLine::assignShapes(std::vector<Shape3d*>& shapes) {
  assert(shapes.empty() == false);
  for (auto& shape : shapes) {
    this->shapes.emplace_back(shape);
  }
}

void PipeLine::assignDirectionalLight(DirectionalLight* directionalLight) {
  assert(directionalLight!=nullptr);
  this->directionalLights.emplace_back(directionalLight);
}

void PipeLine::assignPointLight(PointLight* pointLight) {
  assert(pointLight != nullptr);
  pointLights.emplace_back(pointLight);
}

void PipeLine::assignShapes(Shape3d* shape) {
  assert(shape != nullptr);
  this->shapes.clear();
  this->shapes.emplace_back(shape);
}

void PipeLine::update(double deltaTime)
{
  if (directionalLights.empty() == false) {
    for (auto& light : directionalLights) {
      light->update(camera);
    }
  }
  if (pointLights.empty() == false) {
    for (auto& light : pointLights) {
      threadPool.assignTaskToAllThreads(&updateShapeNodesReference, light->getShape());
      threadPool.assignTaskToAllThreads(&updateShapeNormalsReference, light->getShape());
      light->update(camera);
    }
  }
  if (shapes.empty() == false) {
    for (auto& shape : shapes) {
      threadPool.assignTaskToAllThreads(&updateShapeNodesReference, shape);
      threadPool.assignTaskToAllThreads(&updateShapeNormalsReference, shape);
    }
  }
  threadPool.waitForThreadsToFinish();
  if (pointLights.empty() == false) {
    for (auto& light : pointLights) {
      threadPool.assignTaskToAllThreads(&updateShapeSurfacesReference, light->getShape());
    }
  }
  if (shapes.empty() == false) {
    for (auto& shape : shapes) {
      threadPool.assignTaskToAllThreads(&updateShapeSurfacesReference, shape);
    }
  }
  threadPool.waitForThreadsToFinish();
}

void PipeLine::updateShapeNodes(
  const unsigned int& threadNumber,
  Shape3d* shape
) {
  for (
    unsigned int nodeIndex = threadNumber;
    nodeIndex < shape->nodes.size();
    nodeIndex += numberOfSupportedThreads
    ) {
    //TODO Start from here for checking camera transformations
    /*shape->worldPoints[nodeIndex].assign(shape->nodes[nodeIndex]);

    shape->worldPoints[nodeIndex].multiply(shape->rotationXYZMatrix);

    shape->worldPoints[nodeIndex].multiply(camera.rotationInverseMatrix);

    shape->worldPoints[nodeIndex].multiply(shape->scaleMatrix);

    float scaleValue = camera.scaleBasedOnZDistance(shape->worldPoints[nodeIndex].getZ() + camera.transformInverseValue.getZ() + shape->transformValue.getZ());

    shape->worldPoints[nodeIndex].setX(shape->worldPoints[nodeIndex].getX() * scaleValue);

    shape->worldPoints[nodeIndex].setY(shape->worldPoints[nodeIndex].getY() * scaleValue);
    
    shape->worldPoints[nodeIndex].multiply(shape->transformMatrix);

    shape->worldPoints[nodeIndex].multiply(camera.transformInverseMatrix);*/

    shape->worldPoints[nodeIndex].assign(shape->nodes[nodeIndex]);

    shape->worldPoints[nodeIndex].multiply(shape->rotationXYZMatrix);

    shape->worldPoints[nodeIndex].multiply(shape->scaleMatrix);
    
    shape->worldPoints[nodeIndex].multiply(shape->transformMatrix);

    shape->worldPoints[nodeIndex].multiply(camera.transformInverseMatrix);

    shape->worldPoints[nodeIndex].minus(camera.screenCenter);

    shape->worldPoints[nodeIndex].multiply(camera.rotationInverseMatrix);

    float scaleValue = camera.scaleBasedOnZDistance(shape->worldPoints[nodeIndex].getZ());

    shape->worldPoints[nodeIndex].setX(shape->worldPoints[nodeIndex].getX() * scaleValue);

    shape->worldPoints[nodeIndex].setY(shape->worldPoints[nodeIndex].getY() * scaleValue);

    shape->worldPoints[nodeIndex].sum(camera.screenCenter);

  }
}

void PipeLine::computeLightIntensityForPoint(
  const Matrix4X1Float& worldPoint,
  const Matrix4X1Float& worldNormal,
  const float& specularIntensity,
  Matrix4X1Float& colorOutputPlaceholder,
  Matrix4X1Float& cameraVectorPlaceholder,
  Matrix4X1Float& cameraVectorHatPlaceholder,
  Matrix4X1Float& lightVectorPlaceholder,
  Matrix4X1Float& lightVectorHatPlaceholder,
  Matrix4X1Float& lightReflectionVectorPlaceholder,
  Matrix4X1Float& lightReflectionVectorHatPlaceholder,
  Matrix4X1Float& output
)
{

  output.setX(0);
  output.setY(0);
  output.setZ(0);
  output.setW(0);

  if (ambientLight) {
    ambientLight->computeLightIntensity(colorOutputPlaceholder);
    output.sum(colorOutputPlaceholder);
  }
  if (directionalLights.empty() == false) {
    for (const auto& light : directionalLights) {
      light->computeLightIntensity(worldNormal, colorOutputPlaceholder);

      assert(colorOutputPlaceholder.getX() >= 0);
      assert(colorOutputPlaceholder.getX() <= 1);
      assert(colorOutputPlaceholder.getY() >= 0);
      assert(colorOutputPlaceholder.getY() <= 1);
      assert(colorOutputPlaceholder.getZ() >= 0);
      assert(colorOutputPlaceholder.getZ() <= 1);

      output.sum(colorOutputPlaceholder);
    }
  }
  if (pointLights.empty() == false) {
    for (const auto& light : pointLights) {
      light->computeLightIntensity(
        camera,
        worldNormal,
        worldPoint,
        specularIntensity,
        lightVectorPlaceholder,
        lightVectorHatPlaceholder,
        lightReflectionVectorPlaceholder,
        lightReflectionVectorHatPlaceholder,
        cameraVectorPlaceholder,
        cameraVectorHatPlaceholder,
        colorOutputPlaceholder
      );

      assert(colorOutputPlaceholder.getX() >= 0);
      assert(colorOutputPlaceholder.getX() <= 1);
      assert(colorOutputPlaceholder.getY() >= 0);
      assert(colorOutputPlaceholder.getY() <= 1);
      assert(colorOutputPlaceholder.getZ() >= 0);
      assert(colorOutputPlaceholder.getZ() <= 1);
      assert(colorOutputPlaceholder.getW() == 0);

      output.sum(colorOutputPlaceholder);

    }
  }

  output.setX(Math::clamp(output.getX(), 0.0f, 1.0f));
  output.setY(Math::clamp(output.getY(), 0.0f, 1.0f));
  output.setZ(Math::clamp(output.getZ(), 0.0f, 1.0f));
  output.setW(0);

}

void PipeLine::updateShapeNodesConversionMethod(const unsigned int& threadNumber, void* shape) {
  updateShapeNodes(threadNumber, (Shape3d*)shape);
}

void PipeLine::updateShapeNormals(
  const unsigned int& threadNumber,
  Shape3d* shape
) {

  for (
    unsigned int normalIndex = threadNumber;
    normalIndex < shape->normals.size();
    normalIndex += numberOfSupportedThreads
    ) {

    shape->worldNormals[normalIndex].assign(shape->normals[normalIndex]);

    shape->worldNormals[normalIndex].multiply(shape->rotationXYZMatrix);

    //TODO We need pipline
    shape->worldNormals[normalIndex].multiply(camera.rotationInverseMatrix);

  }

}

void PipeLine::updateShapeNormalsConversionMethod(const unsigned int& threadNumber, void* shape) { 
  updateShapeNormals(threadNumber, (Shape3d*)shape); 
}

void PipeLine::updateShapeSurfaces(
  const unsigned int& threadNumber,
  Shape3d* shape
) {

  for (
    unsigned int surfaceIndex = threadNumber;
    surfaceIndex < shape->surfaces.size();
    surfaceIndex += numberOfSupportedThreads
    ) {
    updateSurface(shape, shape->surfaces[surfaceIndex].get());
  }

}

void PipeLine::updateSurface(Shape3d* shape3d, Surface* surface)
{

  if (
    camera.isVisibleToCamera(
      shape3d->worldPoints,
      shape3d->worldNormals,
      surface->edgeIndices,
      surface->normalIndices,
      surface->cameraVectorPlaceholder
    ) == false
  ) {
    return;
  }
  
  assembleTriangles(shape3d, surface);

}

void PipeLine::assembleTriangles(Shape3d* shape3d, Surface* surface)
{

  surface->triangleMemoryPool.triangleStart.assign(shape3d->worldPoints.at(surface->edgeIndices[0]));

  surface->triangleMemoryPool.triangleEnd.assign(shape3d->worldPoints.at(surface->edgeIndices[1]));

  surface->triangleMemoryPool.triangleFinal.assign(shape3d->worldPoints.at(surface->edgeIndices[2]));

  surface->triangleMemoryPool.totalStepCount = 0;

  surface->triangleMemoryPool.xDifference = surface->triangleMemoryPool.triangleFinal.getX() - surface->triangleMemoryPool.triangleStart.getX();

  surface->triangleMemoryPool.yDifference = surface->triangleMemoryPool.triangleFinal.getY() - surface->triangleMemoryPool.triangleStart.getY();

  if (surface->triangleMemoryPool.xDifference == 0 && surface->triangleMemoryPool.yDifference == 0) {
    return;
  }
  {//Computing total step count
    if (abs(surface->triangleMemoryPool.xDifference) > abs(surface->triangleMemoryPool.yDifference)) {
      assert(surface->triangleMemoryPool.xDifference != 0);
      camera.calculateStepCount(
        surface->triangleMemoryPool.xDifference,
        &surface->triangleMemoryPool.totalStepCount
      );
    }
    else {
      assert(surface->triangleMemoryPool.yDifference != 0);
      camera.calculateStepCount(
        surface->triangleMemoryPool.yDifference,
        &surface->triangleMemoryPool.totalStepCount
      );
    }
    if (surface->triangleMemoryPool.totalStepCount == 0) {
      return;
    }
  }
  {//Computing triangle
    camera.calculateStepValueBasedOnStepCount(
      surface->triangleMemoryPool.xDifference,
      surface->triangleMemoryPool.yDifference,
      surface->triangleMemoryPool.triangleFinal.getZ() - surface->triangleMemoryPool.triangleStart.getZ(),
      surface->triangleMemoryPool.totalStepCount,
      surface->triangleMemoryPool.triangleStartStepValue
    );
    camera.calculateStepValueBasedOnStepCount(
      surface->triangleMemoryPool.triangleFinal.getX() - surface->triangleMemoryPool.triangleEnd.getX(),
      surface->triangleMemoryPool.triangleFinal.getY() - surface->triangleMemoryPool.triangleEnd.getY(),
      surface->triangleMemoryPool.triangleFinal.getZ() - surface->triangleMemoryPool.triangleEnd.getZ(),
      surface->triangleMemoryPool.totalStepCount,
      surface->triangleMemoryPool.triangleEndStepValue
    );
  }

  {//Computing texture step value
    
    surface->triangleMemoryPool.textureStart.assign(surface->textureCoordinate[0]);

    surface->triangleMemoryPool.textureStart.multiply(
        camera.scaleBasedOnZDistance(surface->triangleMemoryPool.triangleStart.getZ())
    );

    surface->triangleMemoryPool.textureEnd.assign(surface->textureCoordinate[1]);

    surface->triangleMemoryPool.textureEnd.multiply(
        camera.scaleBasedOnZDistance(surface->triangleMemoryPool.triangleEnd.getZ())
    );

    surface->triangleMemoryPool.textureFinal.assign(surface->textureCoordinate[2]);

    surface->triangleMemoryPool.textureFinal.multiply(
      camera.scaleBasedOnZDistance(surface->triangleMemoryPool.triangleFinal.getZ())
    );

    camera.calculateStepValueBasedOnStepCount(
      surface->triangleMemoryPool.textureFinal.getX() - surface->triangleMemoryPool.textureStart.getX(),
      surface->triangleMemoryPool.textureFinal.getY() - surface->triangleMemoryPool.textureStart.getY(),
      surface->triangleMemoryPool.totalStepCount,
      surface->triangleMemoryPool.textureStartStepValue
    );

    camera.calculateStepValueBasedOnStepCount(
      surface->triangleMemoryPool.textureFinal.getX() - surface->triangleMemoryPool.textureEnd.getX(),
      surface->triangleMemoryPool.textureFinal.getY() - surface->triangleMemoryPool.textureEnd.getY(),
      surface->triangleMemoryPool.totalStepCount,
      surface->triangleMemoryPool.textureEndStepValue
    );

  }
  //TODO We need seperate methods instead
  if (surface->lightPrecision == Constants::LightPrecision::perSurface) {

    computeLightIntensityForPoint(
      surface->triangleMemoryPool.triangleStart,
      shape3d->worldNormals[surface->normalIndices[0]],
      shape3d->specularIntensity,
      surface->triangleMemoryPool.colorOutputPlaceholder,
      surface->triangleMemoryPool.cameraVectorPlaceholder,
      surface->triangleMemoryPool.cameraVectorHatPlaceholder,
      surface->triangleMemoryPool.lightVectorPlaceholder,
      surface->triangleMemoryPool.lightVectorHatPlaceholder,
      surface->triangleMemoryPool.lightReflectionPlaceholder,
      surface->triangleMemoryPool.lightReflectionHatPlaceholder,
      surface->triangleMemoryPool.colorStart
    );

    computeLightIntensityForPoint(
      surface->triangleMemoryPool.triangleEnd,
      shape3d->worldNormals[surface->normalIndices[1]],
      shape3d->specularIntensity,
      surface->triangleMemoryPool.colorOutputPlaceholder,
      surface->triangleMemoryPool.cameraVectorPlaceholder,
      surface->triangleMemoryPool.cameraVectorHatPlaceholder,
      surface->triangleMemoryPool.lightVectorPlaceholder,
      surface->triangleMemoryPool.lightVectorHatPlaceholder,
      surface->triangleMemoryPool.lightReflectionPlaceholder,
      surface->triangleMemoryPool.lightReflectionHatPlaceholder,
      surface->triangleMemoryPool.colorEnd
    );

    computeLightIntensityForPoint(
      surface->triangleMemoryPool.triangleFinal,
      shape3d->worldNormals[surface->normalIndices[2]],
      shape3d->specularIntensity,
      surface->triangleMemoryPool.colorOutputPlaceholder,
      surface->triangleMemoryPool.cameraVectorPlaceholder,
      surface->triangleMemoryPool.cameraVectorHatPlaceholder,
      surface->triangleMemoryPool.lightVectorPlaceholder,
      surface->triangleMemoryPool.lightVectorHatPlaceholder,
      surface->triangleMemoryPool.lightReflectionPlaceholder,
      surface->triangleMemoryPool.lightReflectionHatPlaceholder,
      surface->triangleMemoryPool.colorFinal
    );

    {//Calculating light stepValue
      camera.calculateStepValueBasedOnStepCount(
        surface->triangleMemoryPool.colorFinal.getR() - surface->triangleMemoryPool.colorStart.getR(),
        surface->triangleMemoryPool.colorFinal.getG() - surface->triangleMemoryPool.colorStart.getG(),
        surface->triangleMemoryPool.colorFinal.getB() - surface->triangleMemoryPool.colorStart.getB(),
        surface->triangleMemoryPool.totalStepCount,
        surface->triangleMemoryPool.colorStartStepValue
      );
      camera.calculateStepValueBasedOnStepCount(
        surface->triangleMemoryPool.colorFinal.getR() - surface->triangleMemoryPool.colorEnd.getR(),
        surface->triangleMemoryPool.colorFinal.getG() - surface->triangleMemoryPool.colorEnd.getG(),
        surface->triangleMemoryPool.colorFinal.getB() - surface->triangleMemoryPool.colorEnd.getB(),
        surface->triangleMemoryPool.totalStepCount,
        surface->triangleMemoryPool.colorEndStepValue
      );
    }
  }
  else if (surface->lightPrecision == Constants::LightPrecision::perPixel)
  {

    surface->triangleMemoryPool.normalStart.assign(shape3d->normals.at(surface->normalIndices[0]));

    surface->triangleMemoryPool.normalEnd.assign(shape3d->normals.at(surface->normalIndices[1]));

    surface->triangleMemoryPool.normalFinal.assign(shape3d->normals.at(surface->normalIndices[2]));

    camera.calculateStepValueBasedOnStepCount(
      surface->triangleMemoryPool.normalFinal.getX() - surface->triangleMemoryPool.normalStart.getX(),
      surface->triangleMemoryPool.normalFinal.getY() - surface->triangleMemoryPool.normalStart.getY(),
      surface->triangleMemoryPool.normalFinal.getZ() - surface->triangleMemoryPool.normalStart.getZ(),
      surface->triangleMemoryPool.totalStepCount,
      surface->triangleMemoryPool.normalStartStepValue
    );

    camera.calculateStepValueBasedOnStepCount(
      surface->triangleMemoryPool.normalFinal.getX() - surface->triangleMemoryPool.normalEnd.getX(),
      surface->triangleMemoryPool.normalFinal.getY() - surface->triangleMemoryPool.normalEnd.getY(),
      surface->triangleMemoryPool.normalFinal.getZ() - surface->triangleMemoryPool.normalEnd.getZ(),
      surface->triangleMemoryPool.totalStepCount,
      surface->triangleMemoryPool.normalEndStepValue
    );

  }
  else if (surface->lightPrecision != Constants::LightPrecision::none) {
    Logger::exception("Unhandled light precision detected");
  }

  for (unsigned long i = 0; i < surface->triangleMemoryPool.totalStepCount; i++) {

    assembleLines(
      shape3d,
      surface,
      surface->triangleMemoryPool.triangleStart,
      surface->triangleMemoryPool.triangleEnd,
      surface->triangleMemoryPool.textureStart,
      surface->triangleMemoryPool.textureEnd,
      surface->triangleMemoryPool.colorStart,
      surface->triangleMemoryPool.colorEnd,
      surface->triangleMemoryPool.normalStart,
      surface->triangleMemoryPool.normalEnd
    );

    surface->triangleMemoryPool.triangleStart.sum(surface->triangleMemoryPool.triangleStartStepValue);
    surface->triangleMemoryPool.triangleEnd.sum(surface->triangleMemoryPool.triangleEndStepValue);
    surface->triangleMemoryPool.textureStart.sum(surface->triangleMemoryPool.textureStartStepValue);
    surface->triangleMemoryPool.textureEnd.sum(surface->triangleMemoryPool.textureEndStepValue);

    if (surface->lightPrecision == Constants::LightPrecision::perSurface) {
      surface->triangleMemoryPool.colorStart.sum(surface->triangleMemoryPool.colorStartStepValue);
      surface->triangleMemoryPool.colorEnd.sum(surface->triangleMemoryPool.colorEndStepValue);
    }
    else if (surface->lightPrecision == Constants::LightPrecision::perPixel)
    {
      surface->triangleMemoryPool.normalStart.sum(surface->triangleMemoryPool.normalStartStepValue);
      surface->triangleMemoryPool.normalEnd.sum(surface->triangleMemoryPool.normalEndStepValue);
    }
    else if (surface->lightPrecision != Constants::LightPrecision::none)
    {
      Logger::exception("Unhandled light precision mode");
    }
  }
}
//TODO Assemble lines for perPixel and perSurface must be seperated
//TODO It must be Matrix3X1 there is no use for Matrix4X1 here
void PipeLine::assembleLines(
  Shape3d* shape,
  Surface* surface,
  const Matrix4X1Float& paramTriangleStart,
  const Matrix4X1Float& paramTriangleEnd,
  const Matrix2X1Float& paramTextureStart,
  const Matrix2X1Float& paramTextureEnd,
  const Matrix4X1Float& paramLightColorStart,
  const Matrix4X1Float& paramLightColorEnd,
  const Matrix4X1Float& paramNormalStart,
  const Matrix4X1Float& paramNormalEnd
)
{
  {//TriangleStepValue
    surface->lineMemoryPool.lineStart.assign(paramTriangleStart);

    surface->lineMemoryPool.xDifference = paramTriangleEnd.getX() - surface->lineMemoryPool.lineStart.getX();
    surface->lineMemoryPool.yDifference = paramTriangleEnd.getY() - surface->lineMemoryPool.lineStart.getY();

    if (surface->lineMemoryPool.xDifference == 0 && surface->lineMemoryPool.yDifference == 0) {
      return;
    }

    if (abs(surface->lineMemoryPool.xDifference) > abs(surface->lineMemoryPool.yDifference)) {
      assert(surface->lineMemoryPool.xDifference != 0);
      camera.calculateStepCount(
        surface->lineMemoryPool.xDifference,
        &surface->lineMemoryPool.totalStepCount
      );
    }
    else {
      assert(surface->lineMemoryPool.yDifference != 0);
      camera.calculateStepCount(
        surface->lineMemoryPool.yDifference,
        &surface->lineMemoryPool.totalStepCount
      );
    }
    if (surface->lineMemoryPool.totalStepCount == 0) {
      return;
    }
    camera.calculateStepValueBasedOnStepCount(
      surface->lineMemoryPool.xDifference,
      surface->lineMemoryPool.yDifference,
      paramTriangleEnd.getZ() - surface->lineMemoryPool.lineStart.getZ(),
      surface->lineMemoryPool.totalStepCount,
      surface->lineMemoryPool.lineStepValue
    );
  }

  {//TextureStepValue
    surface->lineMemoryPool.textureStart.assign(paramTextureStart);
    camera.calculateStepValueBasedOnStepCount(
      paramTextureEnd.getX() - surface->lineMemoryPool.textureStart.getX(),
      paramTextureEnd.getY() - surface->lineMemoryPool.textureStart.getY(),
      surface->lineMemoryPool.totalStepCount,
      surface->lineMemoryPool.textureStepValue
    );
  }

  if (surface->lightPrecision == Constants::LightPrecision::perSurface) {
    surface->lineMemoryPool.colorStart.assign(paramLightColorStart);
    camera.calculateStepValueBasedOnStepCount(
      paramLightColorEnd.getR() - surface->lineMemoryPool.colorStart.getR(),
      paramLightColorEnd.getG() - surface->lineMemoryPool.colorStart.getG(),
      paramLightColorEnd.getB() - surface->lineMemoryPool.colorStart.getB(),
      surface->lineMemoryPool.totalStepCount,
      surface->lineMemoryPool.colorStepValue
    );
  }
  else if (surface->lightPrecision == Constants::LightPrecision::perPixel)
  {
    surface->lineMemoryPool.normalStart.assign(paramNormalStart);
    camera.calculateStepValueBasedOnStepCount(
      paramNormalEnd.getX() - surface->lineMemoryPool.normalStart.getX(),
      paramNormalEnd.getY() - surface->lineMemoryPool.normalStart.getY(),
      paramNormalEnd.getZ() - surface->lineMemoryPool.normalStart.getZ(),
      surface->lineMemoryPool.totalStepCount,
      surface->lineMemoryPool.normalStepValue
    );
  }
  else if(surface->lightPrecision != Constants::LightPrecision::none)
  {
    Logger::exception("Unhandled light precision");
  }

  float scaleValue = 0.0;

  for (unsigned long j = 0; j < surface->lineMemoryPool.totalStepCount; j++) {

    scaleValue = camera.scaleBasedOnZDistance(surface->lineMemoryPool.lineStart.getZ());

    surface->texture->getPixelForPosition(
      surface->lineMemoryPool.textureStart.getX() / scaleValue,
      surface->lineMemoryPool.textureStart.getY() / scaleValue,
      &surface->lineMemoryPool.red,
      &surface->lineMemoryPool.green,
      &surface->lineMemoryPool.blue
    );
    if (surface->lightPrecision == Constants::LightPrecision::perSurface) {
      // Multiply color by light value
      surface->lineMemoryPool.red *= surface->lineMemoryPool.colorStart.getR();
      surface->lineMemoryPool.green *= surface->lineMemoryPool.colorStart.getG();
      surface->lineMemoryPool.blue *= surface->lineMemoryPool.colorStart.getB();
    }
    else if (surface->lightPrecision == Constants::LightPrecision::perPixel)
    {

      surface->lineMemoryPool.normalStart.hat(surface->lineMemoryPool.perPixelNormalHat);

      surface->lineMemoryPool.perPixelColorIntensity.set(0, 0, 0.0f);
      surface->lineMemoryPool.perPixelColorIntensity.set(1, 0, 0.0f);
      surface->lineMemoryPool.perPixelColorIntensity.set(2, 0, 0.0f);

      computeLightIntensityForPoint(
        surface->lineMemoryPool.lineStart,
        surface->lineMemoryPool.normalStart,
        shape->specularIntensity,
        surface->lineMemoryPool.colorOutputPlaceholder,
        surface->lineMemoryPool.cameraVectorPlaceholder,
        surface->lineMemoryPool.cameraVectorHatPlaceholder,
        surface->lineMemoryPool.lightVectorPlaceholder,
        surface->lineMemoryPool.lightVectorHatPlaceholder,
        surface->lineMemoryPool.lightReflectionPlaceholder,
        surface->lineMemoryPool.lightReflectionHatPlaceholder,
        surface->lineMemoryPool.perPixelColorIntensity
      );

      surface->lineMemoryPool.red *= surface->lineMemoryPool.perPixelColorIntensity.get(0, 0);
      surface->lineMemoryPool.green *= surface->lineMemoryPool.perPixelColorIntensity.get(1, 0);
      surface->lineMemoryPool.blue *= surface->lineMemoryPool.perPixelColorIntensity.get(2, 0);
    }
    else if(surface->lightPrecision != Constants::LightPrecision::none){
      Logger::exception("Unhandled light precision");
    }

    camera.putPixelInMap(
      int(surface->lineMemoryPool.lineStart.getX()),
      int(surface->lineMemoryPool.lineStart.getY()),
      surface->lineMemoryPool.lineStart.getZ(),
      fmaxf(surface->lineMemoryPool.red, 0.0f),
      fmaxf(surface->lineMemoryPool.green, 0.0f),
      fmaxf(surface->lineMemoryPool.blue, 0.0f)
    );

    surface->lineMemoryPool.lineStart.sum(surface->lineMemoryPool.lineStepValue);

    surface->lineMemoryPool.textureStart.sum(surface->lineMemoryPool.textureStepValue);

    if (surface->lightPrecision == Constants::LightPrecision::perSurface) {
      surface->lineMemoryPool.colorStart.sum(surface->lineMemoryPool.colorStepValue);
    }
    else if (surface->lightPrecision == Constants::LightPrecision::perPixel) {
      surface->lineMemoryPool.normalStart.sum(surface->lineMemoryPool.normalStepValue);
    }
    else if (surface->lightPrecision != Constants::LightPrecision::none) {
      Logger::exception("Unhandled light precision");
    }
  }
}

void PipeLine::updateShapeSurfacesConversionMethod(const unsigned int& threadNumber, void* shape) { 
  updateShapeSurfaces(threadNumber, (Shape3d*)shape); 
}