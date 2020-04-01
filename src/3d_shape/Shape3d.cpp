#include "./Shape3d.h"

#include <cassert>
#include <cmath>
#include <memory>

#include "../open_gl/OpenGl.h"
#include "./surface/base_surface/BaseSurface.h"
#include "./surface/color_surface/ColorSurface.h"
#include "./../shaders/light/Light.h"

//TODO Replace this with uniq_ptr and use std::move instead for edges
Shape3d::Shape3d(
  std::vector<MatrixFloat>& nodes,
  std::vector<std::unique_ptr<BaseSurface>>& edges
)
:
Shape3d(nodes,edges,0,0,0,0,0,0,1) {}

Shape3d::Shape3d(
  std::vector<MatrixFloat>& nodes,
  std::vector<std::unique_ptr<BaseSurface>>& edges,
  float initialTransformX,
  float initialTransformY,
  float initialTransformZ
)
:
Shape3d(
  nodes,
  edges,
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
  std::vector<std::unique_ptr<BaseSurface>>& paramEdges,
  float transformX,
  float transformY,
  float transformZ,
  float rotationDegreeX,
  float rotationDegreeY,
  float rotationDegreeZ,
  float scaleValue
 ) :
  nodes(std::move(paramNodes)),
  edges(std::move(paramEdges)),
  transformMatrix(3, 1, 0.0f),
  rotationDegreeMatrix(3, 1, 0.0f),
  //TODO Check these rotations correctness
  rotationValueXMatrix(3, 3, std::vector<std::vector<float>>{
    std::vector<float>{cosf(0), -sinf(0), 0},
    std::vector<float>{sinf(0), cosf(0), 0},
    std::vector<float>{0, 0, 1}
  }),
  rotationValueYMatrix(3, 3, std::vector<std::vector<float>>{
    std::vector<float>{cosf(0), 0, sinf(0)},
    std::vector<float>{0, 1, 0},
    std::vector<float>{-sinf(0), 0, cosf(0)}
  }),
  rotationValueZMatrix(3, 3, std::vector<std::vector<float>>{
    std::vector<float>{1, 0, 0},
    std::vector<float>{0, cosf(0), sinf(0)},
    std::vector<float>{0, -sinf(0), cosf(0)}
  }),
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
    for (auto &node:nodes) {
      worldPoints.emplace_back(node.clone());
    }
  }
    
  assert(areAllEdgesDataValid());

  this->transformX(transformX);
  this->transformY(transformY);
  this->transformZ(transformZ);
  this->rotateX(rotationDegreeX);
  this->rotateY(rotationDegreeY);
  this->rotateZ(rotationDegreeZ);
}

bool Shape3d::areAllEdgesDataValid() {
  if (!edges.empty()) {
    for (auto& edge : edges) {
      if (!edge->areEdgesDataValid(nodes.size())) {
        return false;
      }
    }
  }
  return true;
}

bool Shape3d::checkForNodesValidation() {
  if (nodes.empty() == false) {
    for (auto &node:nodes) {
      if (node.getWidth() != 3 || node.getHeight() != 1) {
        return false;
      }
    }
  }
  return true;
}

void Shape3d::update(
  double deltaTime,
  Camera& cameraInstance,
  std::vector<std::unique_ptr<Light>>& lightSources
) {
  
  float zLocation = 0;
  float scaleValue = 0;

  for (unsigned int i = 0; i < nodes.size(); i++) {
    //TODO Create pipleline class from this part
    //TODO Needs optimization
    transformResultMatrix.assign(nodes[i]);
    transformResultMatrix.multiply(rotationValueXMatrix);
    transformResultMatrix.multiply(rotationValueYMatrix);
    transformResultMatrix.multiply(rotationValueZMatrix);
    transformResultMatrix.multiply(scaleValueMatrix);
      
    transformResultMatrix.multiply(cameraInstance.getRotationX());
    transformResultMatrix.multiply(cameraInstance.getRotationY());
    transformResultMatrix.multiply(cameraInstance.getRotationZ());
      
    zLocation = transformResultMatrix.get(2, 0) + transformMatrix.get(2,0);
    scaleValue = cameraInstance.scaleBasedOnZDistance(zLocation);
    zScaleMatrix.set(0, 0, scaleValue);
    zScaleMatrix.set(1, 1, scaleValue);

    transformResultMatrix.multiply(zScaleMatrix);
    transformResultMatrix.sum(transformMatrix);
    transformResultMatrix.minus(cameraInstance.getTransformMatrix());

    worldPoints.at(i) = transformResultMatrix;

  }
    
  for (auto &edge:edges) {
    edge->update(
      cameraInstance,
      worldPoints,
      lightSources
    );
  }
}

void Shape3d::render(
  double deltaTime,
  Camera& cameraInstance
) {
  if (!edges.empty()) {
    for (auto& edge : edges) {
      edge->render(
        cameraInstance,
        worldPoints
      );
    }
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
  //TODO we need to generate rotation matrix in matrix class
  rotationValueXMatrix.set(0, 0, cosf(rotationDegreeMatrix.get(0, 0)));
  rotationValueXMatrix.set(0, 1, -sinf(rotationDegreeMatrix.get(0, 0)));
  rotationValueXMatrix.set(1, 0, sinf(rotationDegreeMatrix.get(0, 0)));
  rotationValueXMatrix.set(1, 1, cosf(rotationDegreeMatrix.get(0, 0)));
}

void Shape3d::rotateY(float y) {
  rotationDegreeMatrix.set(1, 0, rotationDegreeMatrix.get(1, 0) + y);
  rotationValueYMatrix.set(0, 0, cosf(rotationDegreeMatrix.get(1, 0)));
  rotationValueYMatrix.set(0, 2, sinf(rotationDegreeMatrix.get(1, 0)));
  rotationValueYMatrix.set(2, 0, -sinf(rotationDegreeMatrix.get(1, 0)));
  rotationValueYMatrix.set(2, 2, cosf(rotationDegreeMatrix.get(1, 0)));
}

void Shape3d::rotateZ(float z) {
  rotationDegreeMatrix.set(2, 0, rotationDegreeMatrix.get(2, 0) + z);
  rotationValueZMatrix.set(1, 1, cosf(rotationDegreeMatrix.get(2, 0)));
  rotationValueZMatrix.set(1, 2, sinf(rotationDegreeMatrix.get(2, 0)));
  rotationValueZMatrix.set(2, 1, -sinf(rotationDegreeMatrix.get(2, 0)));
  rotationValueZMatrix.set(2, 2, cosf(rotationDegreeMatrix.get(2, 0)));
}