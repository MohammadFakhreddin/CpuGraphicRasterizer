#include "./Shape3d.h"

#include <cassert>
#include <cmath>
#include <memory>
#include <thread>

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

  MatrixFloat vector1(3, 1, 0.0f);
  MatrixFloat vector2(3, 1, 0.0f);
  MatrixFloat* targetNode = nullptr;
  MatrixFloat* sideNode1 = nullptr;
  MatrixFloat* sideNode2 = nullptr;
  MatrixFloat normalVector(3, 1, 0.0f);
  MatrixFloat normalVectorHat(3, 1, 0.0f);

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

        normals.emplace_back(3,1,0.0f);
        normals.back().assign(normalVectorHat);

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

            currentNodeNormals.emplace_back(3, 1, 0.0f);
            currentNodeNormals.back().assign(normalVectorHat);

            surface->setNormalIndex(surfaceEdgeIndex, (unsigned long)normals.size());

            break;
          }

        }

      }

      assert(currentNodeNormals.size() != 0);

      normals.emplace_back(3, 1, 0.0f);

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

      assert(isnan(normals.back().get(0, 0)) == false);
      assert(isnan(normals.back().get(1, 0)) == false);
      assert(isnan(normals.back().get(2, 0)) == false);

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
  Shape3d(nodes, surfaces, normals,0, 0, 0, 0, 0, 0, 0, 1) 
{}

Shape3d::Shape3d(
  std::vector<MatrixFloat> & nodes, 
  std::vector<std::unique_ptr<Surface>> & surfaces, 
  std::vector<MatrixFloat> & normals, 
  const float specularIntensity
)
  :
  Shape3d(nodes, surfaces, normals, specularIntensity, 0, 0, 0, 0, 0, 0, 1)
{}


Shape3d::Shape3d(
  std::vector<MatrixFloat>& nodes,
  std::vector<std::unique_ptr<Surface>>& surfaces,
  std::vector<MatrixFloat>& normals,
  const float specularIntensity,
  float initialTransformX,
  float initialTransformY,
  float initialTransformZ
)
:
Shape3d(
  nodes,
  surfaces,
  normals,
  specularIntensity,
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
  const float specularIntensity,
  float transformX,
  float transformY,
  float transformZ,
  float rotationDegreeX,
  float rotationDegreeY,
  float rotationDegreeZ,
  float paramScaleValue
) :
  surfaces(std::move(paramSurfaces)),
  transformMatrix(3, 1, 0.0f),
  rotationDegreeMatrix(3, 1, 0.0f),
  rotationXYZMatrix(3, 3, 0.0f),
  scaleValueMatrix(3,3,0.0f),
  specularIntensity(specularIntensity)
 {
  
  assert(specularIntensity >= 0);

  if (!paramNodes.empty()) {
    for (auto& node : paramNodes) {
      nodes.emplace_back(3, 1, 0.0f);
      nodes.back().assign(node);
      worldPoints.emplace_back(3, 1, 0.0f);
      worldPoints.back().assign(node);
    }
  }

  if (!paramNormals.empty()) {
    for (auto& normal : paramNormals) {
      normals.emplace_back(3, 1, 0.0f);
      normals.back().assign(normal);
      worldNormals.emplace_back(3, 1, 0.0f);
      worldNormals.back().assign(normal);
    }
  }

  this->transformX(transformX);
  this->transformY(transformY);
  this->transformZ(transformZ);
  this->rotateXYZ(rotationDegreeX, rotationDegreeY, rotationDegreeZ);
  this->scale(paramScaleValue);

  assert(checkDataValidation());

}

void Shape3d::setSpecularIntensity(const float& value)
{
  assert(value >= 0);
  specularIntensity = value;
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

void Shape3d::transformX(float x) {
  MatrixFloat::addToTransfromXMatrix(transformMatrix, x);
}

void Shape3d::transformY(float y) {
  MatrixFloat::addToTransfromYMatrix(transformMatrix, y);
}

void Shape3d::transformZ(float z) {
  MatrixFloat::addToTransfromZMatrix(transformMatrix, z);
}

void Shape3d::scale(float value) {
  MatrixFloat::addToScaleMatrix(scaleValueMatrix, value);
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
