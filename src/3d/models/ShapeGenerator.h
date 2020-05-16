#ifndef ShapeGenerator_Class
#define ShapeGenerator_Class

#include <memory>

#include "../shape/Shape3d.h"

class ShapeGenerator {

public:

  static std::unique_ptr<Shape3d> cube(
    std::vector<std::unique_ptr<Surface>>& surfaceList,
    float w,
    float h,
    float d,
    float transformX,
    float transformY,
    float transformZ,
    float rotationX,
    float rotationY,
    float rotationZ,
    float scale
  ) {

    assert(surfaceList.size() == 12);

    float x = -w / 2;
    float y = -h / 2;
    float z = -d / 2;

    std::vector<Matrix3X1Float> nodeList;

    auto isNodeValid = [](const Matrix3X1Float& matrix) {
      if (std::isnan(matrix.getX()) == true) {
        Logger::log("IsNan nodeList[index].getX is true");
        return false;
      }
      if (std::isnan(matrix.getY()) == true) {
        Logger::log("IsNan nodeList[index].getY() is true");
        return false;
      }
      if (std::isnan(matrix.getZ()) == true) {
        Logger::log("IsNan nodeList[index].getZ() is true");
        return false;
      }
      return true;
    };

    nodeList.emplace_back(x,y,z);
    assert(isNodeValid(nodeList.back()));
    
    nodeList.emplace_back(x, y, z + d);
    assert(isNodeValid(nodeList.back()));

    nodeList.emplace_back(x, y + h, z);
    assert(isNodeValid(nodeList.back()));

    nodeList.emplace_back(x, y + h, z + d);
    assert(isNodeValid(nodeList.back()));

    nodeList.emplace_back(x + w, y, z);
    assert(isNodeValid(nodeList.back()));

    nodeList.emplace_back(x + w, y, z + d);
    assert(isNodeValid(nodeList.back()));

    nodeList.emplace_back(x + w, y + h, z);
    assert(isNodeValid(nodeList.back()));

    nodeList.emplace_back(x + w, y + h, z + d);
    assert(isNodeValid(nodeList.back()));

    //Generating normals
    std::vector<Matrix3X1Float> normals = Shape3d::generateNormals(
      surfaceList,
      nodeList,
      Shape3d::NormalType::sharp
    );
    return std::make_unique<Shape3d>(
      nodeList,
      surfaceList,
      normals,
      0.0f,
      transformX,
      transformY,
      transformZ,
      rotationX,
      rotationY,
      rotationZ,
      scale
    );
  }

  static std::unique_ptr<Shape3d> sphere(
    const Constants::LightPrecision lightPrecision,
    Texture* texture,
    const float& radius,
    const unsigned int& numberOfLat,
    const unsigned int& numberOfLong,
    const Vec3DFloat& transform,
    const Vec3DFloat& rotation,
    const float& scale
  )
  {

    assert(radius > 0 && "ShapeGenerator::sphere radius must be above 0");
    assert(numberOfLat > 0 && "ShapeGenerator::sphere number of lat must be above 0");
    assert(numberOfLong > 0 && "ShapeGenerator::sphere number of long must be above 0");

    std::vector<Matrix3X1Float> vertices;

    auto isVertexValid = [](const Matrix3X1Float& vertex) {
      if (std::isnan(vertex.getX()) == true) {
        Logger::log("IsNan vertex.getX is true");
        return false;
      }
      if (std::isnan(vertex.getY()) == true) {
        Logger::log("IsNan vertex.getY() is true");
        return false;
      }
      if (std::isnan(vertex.getZ()) == true) {
        Logger::log("IsNan vertex.getZ() is true");
        return false;
      }
      return true;
    };

    {//Filling vertices
      Matrix3X1Float initialPoint;
      initialPoint.set(1, 0, radius);
      
      const auto latitudeStepDegree = Math::piFloat / float(numberOfLat);

      Matrix4X4Float latitudeRotationMatrix;
     
      const auto longitudeStepDegree = (Math::piFloat * 2.0f) / float(numberOfLong);

      Matrix4X4Float longitudeRotationMatrix;
   
      Matrix4X1Float latitudePoint;

      Matrix4X1Float longitudePoint;

      for (unsigned int latitudeIndex = 1; latitudeIndex < numberOfLat - 1; latitudeIndex++) {

        latitudePoint.assign(initialPoint);

        Matrix4X4Float::assignRotationZ(
          latitudeRotationMatrix, 
          latitudeStepDegree * latitudeIndex
        );
      
        latitudePoint.multiply(latitudeRotationMatrix);

        for (unsigned int longitudeIndex = 0; longitudeIndex < numberOfLong ; longitudeIndex++) {

          longitudePoint.assign(latitudePoint);
          
          Matrix4X4Float::assignRotationY(
            longitudeRotationMatrix,
            longitudeStepDegree * longitudeIndex
          );
          
          longitudePoint.multiply(longitudeRotationMatrix);

          vertices.emplace_back();
          vertices.back().assign(longitudePoint);
          assert(isVertexValid(vertices.back()));

        }

      }

      vertices.emplace_back();
      vertices.back().assign(initialPoint);
      assert(isVertexValid(vertices.back()));
      
      initialPoint.set(1, 0, -1 * radius);

      vertices.emplace_back();
      vertices.back().assign(initialPoint);
      assert(isVertexValid(vertices.back()));
    
    }

    const auto sideLatCount = numberOfLat - 2;

    auto convertLatAndLongToVertices = [numberOfLat, numberOfLong, sideLatCount](unsigned int latIndex, unsigned int longIndex) {
      assert(
        latIndex >= 0 && latIndex < numberOfLat &&
        "ShapeGenerator::sphere::convertLatAndLongToVertices latIndex must be above 0 and bellow numberOfLat"
      );
      assert(
        longIndex >= 0 && longIndex < numberOfLong &&
        "ShapeGenerator::sphere::convertLatAndLongToVertices longIndex must be above 0 and bellow numberOfLong"
      );
      assert(
        (latIndex < sideLatCount || longIndex == 0) &&
        "ShapeGenerator::sphere::convertLatAndLongToVertices when latIndex is 0 or numberOfLat-1 longIndex must be 0"
      );
      unsigned int index = 0u;
      if (latIndex >= sideLatCount) {
        index = numberOfLong * sideLatCount + latIndex - sideLatCount;
      }
      else {
        index = latIndex * numberOfLong + longIndex;
      }
      return index;
    };

    std::vector<std::unique_ptr<Surface>> indices;

    {//Filling indices
      
      for (unsigned int i = 0; i < numberOfLong; i++) {
        unsigned long edgeIndices[3] = {
          (unsigned long)convertLatAndLongToVertices(sideLatCount, 0),
          (unsigned long)convertLatAndLongToVertices(1, (i + 1) % numberOfLong),
          (unsigned long)convertLatAndLongToVertices(1, i % numberOfLong)
        };
        indices.emplace_back(std::make_unique<Surface>(
          lightPrecision,
          texture,
          edgeIndices
        ));
      }
      
      for (unsigned int i = 0; i < numberOfLong; i++) {
        unsigned long edgeIndices[3] = {
          (unsigned long)convertLatAndLongToVertices(sideLatCount - 1, i % numberOfLong),
          (unsigned long)convertLatAndLongToVertices(sideLatCount - 1, (i + 1) % numberOfLong),
          (unsigned long)convertLatAndLongToVertices(sideLatCount + 1, 0)
        };
        indices.emplace_back(std::make_unique<Surface>(
          lightPrecision,
          texture,
          edgeIndices
        ));
      }
      
      for (unsigned int i = 0; i < sideLatCount - 1; i++) {
        for (unsigned int j = 0; j < numberOfLong; j++) {
          {
            unsigned long edgeIndices[3] = {
              (unsigned long)convertLatAndLongToVertices(i, j % numberOfLong),
              (unsigned long)convertLatAndLongToVertices(i, (j + 1) % numberOfLong),
              (unsigned long)convertLatAndLongToVertices(i + 1, (j + 1) % numberOfLong)
            };
            indices.emplace_back(std::make_unique<Surface>(
              lightPrecision,
              texture,
              edgeIndices
            ));
          }
          {
            unsigned long edgeIndices[3] = {
              (unsigned long)convertLatAndLongToVertices(i + 1, j % numberOfLong),
              (unsigned long)convertLatAndLongToVertices(i, j % numberOfLong),
              (unsigned long)convertLatAndLongToVertices(i + 1, (j + 1) % numberOfLong)
            };
            indices.emplace_back(std::make_unique<Surface>(
              lightPrecision,
              texture,
              edgeIndices
            ));
          }
        }
      }

    }

    std::vector<Matrix3X1Float> normals = Shape3d::generateNormals(indices, vertices, Shape3d::NormalType::smooth);

    return std::make_unique<Shape3d>(
      vertices,
      indices,
      normals,
      0.0f,
      transform.getX(),
      transform.getY(),
      transform.getZ(),
      rotation.getX(),
      rotation.getY(),
      rotation.getZ(),
      scale
    );

  }

};

#endif // !ShapeGenerator_Class
