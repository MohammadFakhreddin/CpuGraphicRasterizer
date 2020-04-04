#include "./Shape3d.h"

#include <cassert>
#include <cmath>
#include <memory>

#include "../open_gl/OpenGl.h"
#include "./../shaders/light/Light.h"

//TODO Replace this with uniq_ptr and use std::move instead for edges
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
  float scaleValue
  ) :
  nodes(std::move(paramNodes)),
  surfaces(std::move(paramSurfaces)),
  normals(std::move(paramNormals)),
  transformMatrix(3, 1, 0.0f),
  rotationDegreeMatrix(3, 1, 0.0f),
  rotationValueXMatrix(3, 3, 0.0f),
  rotationValueYMatrix(3, 3, 0.0f),
  rotationValueZMatrix(3, 3, 0.0f),
  scaleValueMatrix(3, 3, std::vector<std::vector<float>>{
  std::vector<float>{1, 0, 0},
    std::vector<float>{0, 1, 0},
    std::vector<float>{0, 0, 1}
}),
zScaleMatrix(3, 3, std::vector<std::vector<float>>{
  std::vector<float>{1, 0, 0},
    std::vector<float>{0, 1, 0},
    std::vector<float>{0, 0, 1}
}) {


  assert(nodes.size() > 0);
  assert(edges.size() > 0);

  assert(checkForNodesValidation());

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

  assert([nodes]() {
    if (!nodes.empty()) {
      for (auto& node : nodes) {
        if (node.getWidth() != 3 || node.getHeight() != 1) {
          return false;
        }
      }
    }
    return true
    } && "Shape3d::constructor On of the nodes matrix width or height is invalid");

  assert([normals]() {
    if (!normals.empty()) {
      for (const auto& normal : normals) {
        if (normals.getWidth() != 3 || normals.getHeight() != 1) {
          return false;
        }
      }
    }
    return true;
    } && "Shape3d::constructor On of the normals width or height is invalid");

  assert([nodes, normals,surfaces]() {
    if (!surfaces.empty()) {
      for (const auto& surface : surfaces) {
        if (!surface->areEdgeAndNormalsValid(nodes.size(), normals.size())) {
          return false;
        }
      }
    }
    return true;
    } && "Shape3d::constructor surface->areEdgeAndNormalsValid failed");

  this->transformX(transformX);
  this->transformY(transformY);
  this->transformZ(transformZ);
  this->rotateX(rotationDegreeX);
  this->rotateY(rotationDegreeY);
  this->rotateZ(rotationDegreeZ);
}

void Shape3d::update(
  double deltaTime,
  Camera& cameraInstance,
  std::vector<std::unique_ptr<Light>>& lightSources
  ) {

  zLocation = 0;
  scaleValue = 0;

  for (i = 0; i < nodes.size(); i++) {
    //TODO Create pipleline class from this part
    //TODO Needs optimization
    currentWorldPoint.assign(nodes[i]);
    currentWorldPoint.multiply(rotationValueXMatrix);
    currentWorldPoint.multiply(rotationValueYMatrix);
    currentWorldPoint.multiply(rotationValueZMatrix);
    currentWorldPoint.multiply(scaleValueMatrix);

    currentWorldPoint.multiply(cameraInstance.getRotationX());
    currentWorldPoint.multiply(cameraInstance.getRotationY());
    currentWorldPoint.multiply(cameraInstance.getRotationZ());

    zLocation = currentWorldPoint.get(2, 0) + transformMatrix.get(2, 0);
    scaleValue = cameraInstance.scaleBasedOnZDistance(zLocation);
    zScaleMatrix.set(0, 0, scaleValue);
    zScaleMatrix.set(1, 1, scaleValue);

    currentWorldPoint.multiply(zScaleMatrix);
    currentWorldPoint.sum(transformMatrix);
    currentWorldPoint.minus(cameraInstance.getTransformMatrix());

    worldPoints.at(i).assign(currentWorldPoint);

  }

  for (i = 0; i < normals.size(); i++) {

    currentWorldNormal.assign(normals[i]);

    currentWorldNormal.multiply(rotationValueXMatrix);
    currentWorldNormal.multiply(rotationValueYMatrix);
    currentWorldNormal.multiply(rotationValueZMatrix);
    
    worldNormals.at(i).assign(currentWorldNormal);

  }
    
  for (auto &edge:surfaces) {
    edge->update(
      cameraInstance,
      worldPoints,
      worldNormals,
      lightSources
    );
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

void Shape3d::rotateX(float x) {
  rotationDegreeMatrix.set(0, 0, rotationDegreeMatrix.get(0, 0) + x);
  MatrixFloat::assignAsRotationXMatrix(rotationValueXMatrix,rotationDegreeMatrix.get(0, 0));
}

void Shape3d::rotateY(float y) {
  rotationDegreeMatrix.set(1, 0, rotationDegreeMatrix.get(1, 0) + y);
  MatrixFloat::assignAsRoationYMatrix(rotationValueYMatrix,rotationDegreeMatrix.get(1, 0));
}

void Shape3d::rotateZ(float z) {
  rotationDegreeMatrix.set(2, 0, rotationDegreeMatrix.get(2, 0) + z);
  MatrixFloat::assignAsRotationZMatrix(rotationValueZMatrix, rotationDegreeMatrix.get(2, 0));
}

