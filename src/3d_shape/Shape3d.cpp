#include "./Shape3d.h"

#include <cassert>
#include <cmath>
#include <memory>
#include <functional>
#include <thread>

#include "../open_gl/OpenGl.h"
#include "./../shaders/light/Light.h"
#include "../data_access_point/DataAccessPoint.h"


std::vector<MatrixFloat> Shape3d::generateNormals(
  std::vector<std::unique_ptr<Surface>>& surfaceList,
  std::vector<MatrixFloat>& nodes,
  NormalType normalType
)
{

  assert(normalType != NormalType::fileDefault);
  assert(!surfaceList.empty());
  assert(!nodes.empty());
  
  std::vector<MatrixFloat> normals;

  MatrixFloat vector1 = MatrixFloat(3, 1, 0.0f);
  MatrixFloat vector2 = MatrixFloat(3, 1, 0.0f);
  MatrixFloat* targetNode = nullptr;
  MatrixFloat* sideNode1 = nullptr;
  MatrixFloat* sideNode2 = nullptr;
  MatrixFloat normalVector = MatrixFloat(3, 1, 0.0f);
  MatrixFloat normalVectorHat = MatrixFloat(3, 1, 0.0f);

  const auto computeEdgeIndex = [](short int index) {
    return index % 3;
  };

  const auto generateDifferenceVector = [](
    MatrixFloat& output,
    const MatrixFloat* vector2,
    const MatrixFloat* vector1
  ) {
    for (short int i = 0; i < 3; i++) {
      output.set(i, 0, vector2->get(i, 0) - vector1->get(i, 0));
    }
  };

  if (normalType == Shape3d::NormalType::sharp) {

    for (auto& surface : surfaceList) {

      for (short int i = 0; i < 3; i++) {

        targetNode = &nodes.at(surface->getEdgeByIndex(computeEdgeIndex(i)));
        
        sideNode1 = &nodes.at(surface->getEdgeByIndex(computeEdgeIndex(i + 1)));
        
        sideNode2 = &nodes.at(surface->getEdgeByIndex(computeEdgeIndex(i + 2)));

        generateDifferenceVector(vector1, sideNode1, targetNode);
        
        generateDifferenceVector(vector2, targetNode, sideNode2);

        normalVector.crossProduct(vector1, vector2);
        
        normalVector.hat<float>(normalVectorHat);

        normals.emplace_back(normalVectorHat);

        surface->setNormalIndex(i, (unsigned long)(normals.size() - 1));

      }

    }

  }
  else if(normalType==Shape3d::NormalType::smooth)
  {

    std::vector<MatrixFloat> currentNodeNormals;

    size_t nodeIndex;

    unsigned short surfaceEdgeIndex;

    size_t currentNodeNormalsIndex;

    unsigned short vectorIndex;

    for (nodeIndex = 0; nodeIndex < nodes.size(); nodeIndex++) {

      currentNodeNormals.clear();

      targetNode = &nodes.at(nodeIndex);
      
      for (auto& surface : surfaceList) {
        
        for (surfaceEdgeIndex = 0; surfaceEdgeIndex < 3; surfaceEdgeIndex++) {

          if (surface->getEdgeByIndex(surfaceEdgeIndex) == nodeIndex) {
            
            sideNode1 = &nodes.at(surface->getEdgeByIndex(computeEdgeIndex(surfaceEdgeIndex + 1)));
            
            sideNode2 = &nodes.at(surface->getEdgeByIndex(computeEdgeIndex(surfaceEdgeIndex + 2)));

            generateDifferenceVector(vector1, sideNode1, targetNode);
            
            generateDifferenceVector(vector2, targetNode, sideNode2);
            
            normalVector.crossProduct(vector1, vector2);
            
            normalVector.hat<float>(normalVectorHat);

            currentNodeNormals.emplace_back(normalVectorHat);

            surface->setNormalIndex(surfaceEdgeIndex, (unsigned long)normals.size());

            break;
          }

        }

      }

      assert(currentNodeNormals.size() != 0);

      normals.emplace_back(MatrixFloat(3, 1, 0.0f));

      for (
        currentNodeNormalsIndex = 0;
        currentNodeNormalsIndex < currentNodeNormals.size();
        currentNodeNormalsIndex++
      ) {

        for (vectorIndex = 0; vectorIndex < 3; vectorIndex++) {
        
          normals.back().set(
            vectorIndex, 0, 
            normals.back().get(vectorIndex, 0) + 
            currentNodeNormals.at(currentNodeNormalsIndex).get(vectorIndex,0)
          );
        
        }
      }

      assert(
        isnan(normals.back().get(0, 0)) == false &&
        isnan(normals.back().get(1, 0)) == false &&
        isnan(normals.back().get(2, 0)) == false
      );

      for (vectorIndex = 0; vectorIndex < 3; vectorIndex++) {

        normals.back().set(vectorIndex, 0, normals.back().get(vectorIndex, 0) / currentNodeNormals.size());

      }

      normals.back().hat<float>(normalVectorHat);

      normals.back().assign(normalVectorHat);

      assert(
        isnan(normals.back().get(0, 0)) == false && 
        isnan(normals.back().get(1, 0)) == false && 
        isnan(normals.back().get(2, 0)) == false
      );

    }

  }
  else
  {
    Logger::exception("Unhandled normal type ");
  }

  return normals;

}

Shape3d::Shape3d(
  std::vector<MatrixFloat>& nodes,
  std::vector<std::unique_ptr<Surface>>& surfaces,
  std::vector<MatrixFloat>& normals
  )
  :
  Shape3d(nodes, surfaces, normals, 0, 0, 0, 0, 0, 0, 1) {}

Shape3d::Shape3d(
  std::vector<MatrixFloat>& nodes,
  std::vector<std::unique_ptr<Surface>>& surfaces,
  std::vector<MatrixFloat>& normals,
  float initialTransformX,
  float initialTransformY,
  float initialTransformZ
)
:
Shape3d(
  nodes,
  surfaces,
  normals,
  initialTransformX,
  initialTransformY,
  initialTransformZ,
  0,
  0,
  0,
  1
) {}

Shape3d::Shape3d(
  std::vector<MatrixFloat>& paramNodes,
  std::vector<std::unique_ptr<Surface>>& paramSurfaces,
  std::vector<MatrixFloat>& paramNormals,
  float transformX,
  float transformY,
  float transformZ,
  float rotationDegreeX,
  float rotationDegreeY,
  float rotationDegreeZ,
  float paramScaleValue
) :
  nodes(std::move(paramNodes)),
  surfaces(std::move(paramSurfaces)),
  normals(std::move(paramNormals)),
  transformMatrix(3, 1, 0.0f),
  rotationDegreeMatrix(3, 1, 0.0f),
  rotationXYZMatrix(3, 3, 0.0f),
  scaleValueMatrix(3,3,0.0f),
  threadPool(DataAccessPoint::getInstance()->getThreadPool())
 {
  assert(numberOfSupportedThreads > 0);
  assert(checkDataValidation());

  if (!nodes.empty()) {
    for (auto& node : nodes) {
      worldPoints.emplace_back(node.clone());
    }
  }

  if (!normals.empty()) {
    for (auto& normal : normals) {
      worldNormals.emplace_back(normal.clone());
    }
  }

  this->transformX(transformX);
  this->transformY(transformY);
  this->transformZ(transformZ);
  this->rotateXYZ(rotationDegreeX, rotationDegreeY, rotationDegreeZ);
  this->scale(paramScaleValue);

  //For simple shapes using threads have overhead
  if (surfaces.size() < 100) {
    numberOfSupportedThreads = 1;
  }
  else
  {
    numberOfSupportedThreads = DataAccessPoint::getInstance()->getThreadPool().getNumberOfAvailableThreads() + 1;
  }

}

bool Shape3d::checkDataValidation() {

  if (nodes.empty()) {
    Logger::log("Shape3d nodes cannot be empty");
    return false;
  }

  if (surfaces.empty()) {
    Logger::log("Shap3d surfaces cannot be empty");
    return false;
  }

  if (normals.empty()) {
    Logger::log("Normals cannot be empty");
    return false;
  }
  
  for (auto& node : nodes) {
    if (node.getWidth() != 3 || node.getHeight() != 1) {
      Logger::log("Shape3d::constructor On of the nodes matrix width or height is invalid");
      return false;
    }
  }
  
  for (const auto& normal : normals) {
    if (normal.getWidth() != 3 || normal.getHeight() != 1) {
      Logger::log("Shape3d::constructor On of the normals width or height is invalid");
      return false;
    }
  }
  
  for (const auto& surface : surfaces) {
    if (!surface->areEdgeAndNormalsValid((unsigned long)nodes.size(), (unsigned long)normals.size())) {
      Logger::log("Shape3d::constructor surface->areEdgeAndNormalsValid failed");
      return false;
    }
  }

  return true;

}

void Shape3d::updateNodes(const unsigned int& threadNumber) {
  for (
    unsigned int nodeIndex = threadNumber;
    nodeIndex < nodes.size();
    nodeIndex += numberOfSupportedThreads
    ) {
    //TODO Create pipleline class from this part
    worldPoints[nodeIndex].assign(nodes[nodeIndex]);

    worldPoints[nodeIndex].multiply(rotationXYZMatrix);

    worldPoints[nodeIndex].multiply(scaleValueMatrix);

    worldPoints[nodeIndex].multiply(cameraInstance->getRotationXYZ());

    float scaleValue = cameraInstance->scaleBasedOnZDistance(worldPoints[nodeIndex].get(2, 0) + transformMatrix.get(2, 0));

    worldPoints[nodeIndex].set(0, 0, worldPoints[nodeIndex].get(0, 0) * scaleValue);

    worldPoints[nodeIndex].set(1, 0, worldPoints[nodeIndex].get(1, 0) * scaleValue);

    worldPoints[nodeIndex].sum(transformMatrix);

    worldPoints[nodeIndex].minus(cameraInstance->getTransformMatrix());
  }
}

void Shape3d::updateNormals(const unsigned int& threadNumber) {
  for (
    unsigned int normalIndex = threadNumber;
    normalIndex < normals.size();
    normalIndex += numberOfSupportedThreads
    ) {

    worldNormals[normalIndex].assign(normals[normalIndex]);

    worldNormals[normalIndex].multiply(rotationXYZMatrix);

    //TODO We need pipline
    worldNormals[normalIndex].multiply(cameraInstance->getRotationXYZ());

  }
}

void Shape3d::updateSurfaces(
  const unsigned int& threadNumber
) {

  for (
    unsigned int surfaceIndex = threadNumber;
    surfaceIndex < surfaces.size();
    surfaceIndex += numberOfSupportedThreads
    ) {
    surfaces.at(surfaceIndex)->update(
      *cameraInstance,
      worldPoints,
      worldNormals,
      *lightSources
    );
  }
}

void Shape3d::update(
  double deltaTime,
  Camera& cameraInstance,
  std::vector<std::unique_ptr<Light>>& lightSources
) {
  this->cameraInstance = &cameraInstance;
  this->lightSources = &lightSources;
  if (numberOfSupportedThreads == 1) {
    updateNodes(0);
    updateNormals(0);
    updateSurfaces(0);
  }
  else {
    {//Updating nodes
      for (
        threadNumberIndex = 0;
        threadNumberIndex < numberOfSupportedThreads - 1;
        threadNumberIndex++
        ) {
        threadPool.assignTask(threadNumberIndex, &updateNodesRefrence);
      }
      updateNodesRefrence(numberOfSupportedThreads - 1);
    }
    {//Updating normals
      for (
        threadNumberIndex = 0;
        threadNumberIndex < numberOfSupportedThreads - 1;
        threadNumberIndex++
        ) {
        threadPool.assignTask(threadNumberIndex, &updateNormalsRefrence);
      }
      updateNormalsRefrence(numberOfSupportedThreads - 1);
    }
    threadPool.waitForThreadsToFinish();
    {//Updating surface
      for (
        threadNumberIndex = 0;
        threadNumberIndex < numberOfSupportedThreads - 1;
        threadNumberIndex++
        ) {
        threadPool.assignTask(threadNumberIndex, &updatSurfacesRefrence);
      }
      updatSurfacesRefrence(numberOfSupportedThreads - 1);
    }
    threadPool.waitForThreadsToFinish();
  }
}

void Shape3d::transformX(float x) {
  transformMatrix.set(0, 0, transformMatrix.get(0, 0) + x);
}

void Shape3d::transformY(float y) {
  transformMatrix.set(1, 0, transformMatrix.get(1, 0) + y);
}

void Shape3d::transformZ(float z) {
  transformMatrix.set(2, 0, transformMatrix.get(2, 0) + z);
}

void Shape3d::scale(float value) {
  scaleValueMatrix.set(0, 0, scaleValueMatrix.get(0, 0) + value);
  scaleValueMatrix.set(1, 1, scaleValueMatrix.get(1, 1) + value);
  scaleValueMatrix.set(2, 2, scaleValueMatrix.get(2, 2) + value);
}
//TODO Maybe we can multiply current rotation matrix instead of re-assigning everything
void Shape3d::rotateXYZ(const float& x, const float& y, const float& z) {
  //TODO Degree might overflow
  rotationDegreeMatrix.set(0, 0, rotationDegreeMatrix.get(0, 0) + x);
  rotationDegreeMatrix.set(1, 0, rotationDegreeMatrix.get(1, 0) + y);
  rotationDegreeMatrix.set(2, 0, rotationDegreeMatrix.get(2, 0) + z);

  MatrixFloat::assignAsRotationXYZMatrix(
    rotationXYZMatrix,
    rotationDegreeMatrix.get(0, 0),
    rotationDegreeMatrix.get(1, 0),
    rotationDegreeMatrix.get(2, 0)
  );

}
