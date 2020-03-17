#include "./BaseEdge.h"
#include <cassert>
#include "../../../utils/math/Math.h"

int BaseEdge::getEdgeByIndex(int index) {
    assert(index > -1);
    assert(index < 3);
    switch (index) {
        case 0:
            return edge1;
        case 1:
            return edge2;
        case 2:
            return edge3;
    }
    return -1;
}

EdgeType BaseEdge::getEdgeType() {
    return EdgeType::base;
}

void BaseEdge::render(
        std::vector<MatrixFloat> *worldPoints,
        Vec3DFloat &cameraLocation,
        unsigned int appScreenWidth,
        unsigned int appScreenHeight,
        float shapeTransformX,
        float shapeTransformY,
        float shapeTransformZ
) {
    assert(false);
}

bool BaseEdge::areEdgesDataValid(int nodesSize) {
    int currentEdge = -1;
    for (int i = 0; i < 3; i++) {
        currentEdge = getEdgeByIndex(i);
        if (currentEdge < 0 || currentEdge >= nodesSize) {
            return false;
        }
    }
    return true;
}

bool BaseEdge::isVisibleToCamera(
        std::vector<MatrixFloat> *worldPoints,
        Vec3DFloat &cameraLocation,
        unsigned int appScreenWidth,
        unsigned int appScreenHeight,
        float shapeTransformX,
        float shapeTransformY,
        float shapeTransformZ
) {
    //Calculating center of triangle
    triangleCenter.setX((
        worldPoints->at(edge1).get(0, 0) +
        worldPoints->at(edge2).get(0, 0) +
        worldPoints->at(edge3).get(0, 0)) / 3.0f);
    triangleCenter.setY((
        worldPoints->at(edge1).get(1, 0) +
        worldPoints->at(edge2).get(1, 0) +
        worldPoints->at(edge3).get(1, 0)
    ) / 3.0f);
    triangleCenter.setZ((
        worldPoints->at(edge1).get(2, 0) +
        worldPoints->at(edge2).get(2, 0) +
        worldPoints->at(edge3).get(2, 0)
    ) / 3.0f);
    //Making camera vector
    cameraVector.setX(Math::clamp(
        shapeTransformX,
        cameraLocation.getX() - (float(appScreenWidth) / 2.0f),
        cameraLocation.getX() + (float(appScreenWidth) / 2.0f)) - shapeTransformX
    );
    cameraVector.setY(
        Math::clamp(
            shapeTransformY,
            cameraLocation.getY() - (float(appScreenHeight) / 2.0f),
            cameraLocation.getY() + (float(appScreenHeight) / 2.0f)
        ) - shapeTransformY
    );
    cameraVector.setZ(triangleCenter.getZ() - cameraLocation.getZ());
    //Creating edge vectors for normal vector computing
    edge1To2Vector.setX(worldPoints->at(edge2).get(0, 0) - worldPoints->at(edge1).get(0, 0));
    edge1To2Vector.setY(worldPoints->at(edge2).get(1, 0) - worldPoints->at(edge1).get(1, 0));
    edge1To2Vector.setZ(worldPoints->at(edge2).get(2, 0) - worldPoints->at(edge1).get(2, 0));

    edge1To3Vector.setX(worldPoints->at(edge3).get(0, 0) - worldPoints->at(edge1).get(0, 0));
    edge1To3Vector.setY(worldPoints->at(edge3).get(1, 0) - worldPoints->at(edge1).get(1, 0));
    edge1To3Vector.setZ(worldPoints->at(edge3).get(2, 0) - worldPoints->at(edge1).get(2, 0));
    //Generating normal vector from edge vectors
    normalVector.crossProduct(edge1To3Vector, edge1To2Vector);
    //When normalVectorDirectionFactor is zero it means that it is uninitialized
    //We use this element to check if we need to reverse our dot products result due to
    //Normal vector being reverse
    if (normalVectorExtraDirectionFactor == 0) {

        triangleCenterToPolygonCenterPoint.setX(shapeTransformX - triangleCenter.getX());
        triangleCenterToPolygonCenterPoint.setY(shapeTransformY - triangleCenter.getY());
        triangleCenterToPolygonCenterPoint.setZ(shapeTransformZ - triangleCenter.getZ());

        dotProductValue = triangleCenterToPolygonCenterPoint.dotProduct(normalVector);
        normalVectorExtraDirectionFactor = dotProductValue > 0 ? -1 : +1;
    }

    dotProductValue = normalVector.dotProduct(cameraVector);

    dotProductValue = dotProductValue > 0 ? 1 : -1;

    if (normalVectorExtraDirectionFactor * dotProductValue <= 0.0f) {
        return true;
    }

    return false;
}