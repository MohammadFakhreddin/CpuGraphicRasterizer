#include "./Shape3d.h"

#include <cassert>
#include <cmath>
#include <memory>
#include <thread>

Shape3d::Shape3d(
  std::vector<Matrix3X1Float>& paramNodes,
  std::vector<std::unique_ptr<Surface>>& paramSurfaces,
  std::vector<Matrix3X1Float>& paramNormals
  )
  :
  Shape3d(paramNodes, paramSurfaces, paramNormals,0, 0, 0, 0, 0, 0, 0, 1)
{}

Shape3d::Shape3d(
  std::vector<Matrix3X1Float>& paramNodes,
  std::vector<std::unique_ptr<Surface>>& paramSurfaces,
  std::vector<Matrix3X1Float>& paramNormals,
  const float paramSpecularIntensity
)
  :
  Shape3d(paramNodes, paramSurfaces, paramNormals, paramSpecularIntensity, 0, 0, 0, 0, 0, 0, 1)
{}


Shape3d::Shape3d(
  std::vector<Matrix3X1Float>& paramNodes,
  std::vector<std::unique_ptr<Surface>>& paramSurfaces,
  std::vector<Matrix3X1Float>& paramNormals,
  const float paramSpecularIntensity,
  float paramInitialTransformX,
  float paramInitialTransformY,
  float paramInitialTransformZ
)
:
Shape3d(
  paramNodes,
  paramSurfaces,
  paramNormals,
  paramSpecularIntensity,
  paramInitialTransformX,
  paramInitialTransformY,
  paramInitialTransformZ,
  0,
  0,
  0,
  1
) {}

Shape3d::Shape3d(
  std::vector<Matrix3X1Float>& paramNodes,
  std::vector<std::unique_ptr<Surface>>& paramSurfaces,
  std::vector<Matrix3X1Float>& paramNormals,
  const float paramSpecularIntensity,
  float paramTransformX,
  float paramTransformY,
  float paramTransformZ,
  float paramRotationDegreeX,
  float paramRotationDegreeY,
  float paramRotationDegreeZ,
  float paramScaleValue
) :
  surfaces(std::move(paramSurfaces)),
  specularIntensity(paramSpecularIntensity)
 {
  
  assert(specularIntensity >= 0);

  if (!paramNodes.empty()) {
    for (auto& node : paramNodes) {
      nodes.emplace_back();
      nodes.back().assign(node);
      worldPoints.emplace_back();
      worldPoints.back().assign(node);
    }
  }

  if (!paramNormals.empty()) {
    for (auto& normal : paramNormals) {
      
      normals.emplace_back();
      normals.back().assign(normal);

      worldNormals.emplace_back();
      worldNormals.back().assign(normal);
    
    }
  }

  this->transformXYZ(paramTransformX, paramTransformY, paramTransformZ);
  this->rotateXYZ(paramRotationDegreeX, paramRotationDegreeY, paramRotationDegreeZ);
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
  for (auto& node : nodes) {
    if (isnan(node.getX())) {
      Logger::log("Shape3d node.getX() is isNan == true");
      return false;
    }
    if (isnan(node.getY())) {
      Logger::log("Shape3d node.getY() is isNan == true");
      return false;
    }
    if (isnan(node.getZ())) {
      Logger::log("Shape3d node.getZ() is isNan == true");
      return false;
    }
  }

  if (normals.empty()) {
    Logger::log("Normals cannot be empty");
    return false;
  }
  for (auto& normal : normals) {
    if (isnan(normal.getX())) {
      Logger::log("Shape3d normal.getX() is isNan == true");
      return false;
    }
    if (isnan(normal.getY())) {
      Logger::log("Shape3d normal.getY() is isNan == true");
      return false;
    }
    if (isnan(normal.getZ())) {
      Logger::log("Shape3d normal.getZ() is isNan == true");
      return false;
    }
  }

  if (surfaces.empty()) {
    Logger::log("Shap3d surfaces cannot be empty");
    return false;
  }
  
  for (const auto& surface : surfaces) {
    if (!surface->areEdgeAndNormalsValid((unsigned long)nodes.size(), (unsigned long)normals.size())) {
      Logger::log("Shape3d::constructor surface->areEdgeAndNormalsValid failed");
      return false;
    }
  }

  return true;

}

//Try using transformXYZ instead which is more efficient
void Shape3d::transformX(const float& x) {
  transformValue.setX(transformValue.getX() + x);
  Matrix4X4Float::assignTransformation(
    transformMatrix,
    transformValue.getX(),
    transformValue.getY(),
    transformValue.getZ()
  );
}

//Try using transformXYZ instead which is more efficient
void Shape3d::transformY(const float& y) {
  transformValue.setY(transformValue.getY() + y);
  Matrix4X4Float::assignTransformation(
    transformMatrix,
    transformValue.getX(),
    transformValue.getY(),
    transformValue.getZ()
  );
}

//Try using transformXYZ instead which is more efficient
void Shape3d::transformZ(const float& z) {
  transformValue.setZ(transformValue.getZ() + z);
  Matrix4X4Float::assignTransformation(
    transformMatrix,
    transformValue.getX(),
    transformValue.getY(),
    transformValue.getZ()
  );
}

void Shape3d::transformXYZ(const float& x, const float& y, const float& z) {
  transformValue.setX(transformValue.getX() + x);
  transformValue.setY(transformValue.getY() + y);
  transformValue.setZ(transformValue.getZ() + z);
  Matrix4X4Float::assignTransformation(
    transformMatrix,
    transformValue.getX(),
    transformValue.getY(),
    transformValue.getZ()
  );
}

void Shape3d::scale(float value) {
  scaleValue += value;
  assert(scaleValue > 0);
  Matrix4X4Float::assignScale(scaleMatrix, scaleValue);
}

//Try using rotateXYZ instead which is more efficient
void Shape3d::rotateX(const float& x) {
  rotationValue.setX(rotationValue.getX() + x);
 
  Matrix4X4Float::assignRotationXYZ(
    rotationXYZMatrix,
    rotationValue.getX(),
    rotationValue.getY(),
    rotationValue.getZ()
  );
}

//Try using rotateXYZ instead which is more efficient
void Shape3d::rotateY(const float& y) {
  rotationValue.setY(rotationValue.getY() + y);
  
  Matrix4X4Float::assignRotationXYZ(
    rotationXYZMatrix,
    rotationValue.getX(),
    rotationValue.getY(),
    rotationValue.getZ()
  );
}

//Try using rotateXYZ instead which is more efficient
void Shape3d::rotateZ(const float& z) {
  rotationValue.setZ(rotationValue.getZ() + z);
  
  Matrix4X4Float::assignRotationXYZ(
    rotationXYZMatrix,
    rotationValue.getX(),
    rotationValue.getY(),
    rotationValue.getZ()
  );
}

//TODO Maybe we can multiply current rotation matrix instead of re-assigning everything
void Shape3d::rotateXYZ(const float& x, const float& y, const float& z) {

  //TODO Degree might overflow
  rotationValue.setX(rotationValue.getX() + x);
  rotationValue.setY(rotationValue.getY() + y);
  rotationValue.setZ(rotationValue.getZ() + z);

  Matrix4X4Float::assignRotationXYZ(
    rotationXYZMatrix,
    rotationValue.getX(),
    rotationValue.getY(),
    rotationValue.getZ()
  );

}

std::vector<Matrix3X1Float> Shape3d::generateNormals(
  std::vector<std::unique_ptr<Surface>>& surfaceList,
  std::vector<Matrix3X1Float>& nodes,
  NormalType normalType
)
{

  assert(normalType != NormalType::fileDefault);
  assert(!surfaceList.empty());
  assert(!nodes.empty());


  auto areNodesValid = [&nodes]() {
    for (auto& node : nodes) {
      if (isnan(node.getX()) == true) {
        Logger::log("IsNan nodeList[index].getX is true");
        return false;
      }
      if (isnan(node.getY()) == true) {
        Logger::log("IsNan nodeList[index].getY() is true");
        return false;
      }
      if (isnan(node.getZ()) == true) {
        Logger::log("IsNan nodeList[index].getZ() is true");
        return false;
      }
    }
    return true;
  };
  assert(areNodesValid());
  

  std::vector<Matrix3X1Float> normals;

  Matrix3X1Float vector1;
  Matrix3X1Float vector2;
  Matrix3X1Float* targetNode = nullptr;
  Matrix3X1Float* sideNode1 = nullptr;
  Matrix3X1Float* sideNode2 = nullptr;
  Matrix3X1Float normalVector;
  Matrix3X1Float normalVectorHat;

  const auto computeEdgeIndex = [](short int index) {
    return index % 3;
  };

  const auto generateDifferenceVector = [](
    Matrix3X1Float& output,
    const Matrix3X1Float* vector2,
    const Matrix3X1Float* vector1
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

        normalVector.hat(normalVectorHat);

        normals.emplace_back();
        normals.back().assign(normalVectorHat);

        surface->setNormalIndex(i, (unsigned long)(normals.size() - 1));

      }

    }

  }
  else if (normalType == Shape3d::NormalType::smooth)
  {

    std::vector<Matrix3X1Float> currentNodeNormals;

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

            normalVector.hat(normalVectorHat);

            currentNodeNormals.emplace_back();
            currentNodeNormals.back().assign(normalVectorHat);

            surface->setNormalIndex(surfaceEdgeIndex, (unsigned long)normals.size());

            break;
          }

        }

      }

      assert(currentNodeNormals.size() != 0);

      normals.emplace_back();

      for (
        currentNodeNormalsIndex = 0;
        currentNodeNormalsIndex < currentNodeNormals.size();
        currentNodeNormalsIndex++
        ) {

        for (vectorIndex = 0; vectorIndex < 3; vectorIndex++) {

          normals.back().set(
            vectorIndex, 0,
            normals.back().get(vectorIndex, 0) +
            currentNodeNormals.at(currentNodeNormalsIndex).get(vectorIndex, 0)
          );

        }
      }

      assert(isnan(normals.back().get(0, 0)) == false);
      assert(isnan(normals.back().get(1, 0)) == false);
      assert(isnan(normals.back().get(2, 0)) == false);

      for (vectorIndex = 0; vectorIndex < 3; vectorIndex++) {

        normals.back().set(vectorIndex, 0, normals.back().get(vectorIndex, 0) / currentNodeNormals.size());

      }

      normals.back().hat(normalVectorHat);

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
