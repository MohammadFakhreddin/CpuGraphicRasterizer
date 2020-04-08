#ifndef ShapeGenerator_Class
#define ShapeGenerator_Class

#include <memory>

#include "../3d_shape/Shape3d.h"
#include "../utils/math/Math.h"
#include "../data_types/MatrixTemplate.h"
#include "../utils/math/Math.h"

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
    std::vector<MatrixFloat> nodeList = {
      MatrixFloat(3, 1, std::vector<std::vector<float>>{
        std::vector<float>{x},
        std::vector<float>{y},
        std::vector<float>{z}
      }),
      MatrixFloat(3, 1, std::vector<std::vector<float>>{
        std::vector<float>{x},
        std::vector<float>{y},
        std::vector<float>{z + d}
      }),
      MatrixFloat(3, 1, std::vector<std::vector<float>>{
        std::vector<float>{x},
        std::vector<float>{y + h},
        std::vector<float>{z}
      }),
      MatrixFloat(3, 1, std::vector<std::vector<float>>{
        std::vector<float>{x},
        std::vector<float>{y + h},
        std::vector<float>{z + d}
      }),
      MatrixFloat(3, 1, std::vector<std::vector<float>>{
        std::vector<float>{x + w},
        std::vector<float>{y},
        std::vector<float>{z}
      }),
      MatrixFloat(3, 1, std::vector<std::vector<float>>{
        std::vector<float>{x + w},
        std::vector<float>{y},
        std::vector<float>{z + d}
      }),
      MatrixFloat(3, 1, std::vector<std::vector<float>>{
        std::vector<float>{x + w},
        std::vector<float>{y + h},
        std::vector<float>{z}
      }),
      MatrixFloat(3, 1, std::vector<std::vector<float>>{
        std::vector<float>{x + w},
        std::vector<float>{y + h},
        std::vector<float>{z + d}
      })
    };
    //Generating normals
    std::vector<MatrixFloat> normals = Shape3d::generateNormals(
      surfaceList,
      nodeList,
      Shape3d::NormalType::sharp
    );
    return std::make_unique<Shape3d>(
      nodeList,
      surfaceList,
      normals,
      transformX,
      transformY,
      transformZ,
      rotationX,
      rotationY,
      rotationZ,
      scale
    );
  }

  static std::vector<Vec2DFloat> gear(
    const float radius,
    const int numberOfCorners
  )
  {
    std::vector<Vec2DFloat> polyLines;
    {
      //I assume that center is zero in both x and y
      const double rectAngle = (2.0f * Math::piDouble) / (numberOfCorners);
      const double startingAngle = numberOfCorners % 2 == 0 ? 0 : rectAngle / 2;
      {
        double x = 0;
        double y = 0;
        double currentAngle = startingAngle;
        for (int i = 0; i < numberOfCorners; i++) {
          x = radius * cos(currentAngle);
          y = radius * sin(currentAngle);
          currentAngle += rectAngle;
          polyLines.emplace_back(Vec2DFloat((float)x, (float)y));
        }
        polyLines.emplace_back(
          Vec2DFloat(
            polyLines.at(0).getX(),
            polyLines.at(0).getY()
          )
        );
      }
    }
    return polyLines;
  }

  static std::unique_ptr<Shape3d> sphere(
    std::unique_ptr<Texture>& texture,
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

    std::vector<MatrixFloat> vertices;

    {//Filling vertices
      MatrixFloat initialPoint = MatrixFloat(3, 1, 0.0f);
      
      initialPoint.set(1, 0, radius);
      
      const auto latitudeStepDegree = Math::piFloat / float(numberOfLat);

      MatrixFloat latitudeRotationMatrix = MatrixFloat(3, 3, 0.0f);
     
      const auto longitudeStepDegree = (Math::piFloat * 2.0f) / float(numberOfLong);

      MatrixFloat longitudeRotationMatrix = MatrixFloat(3, 3, 0.0f);
   
      MatrixFloat latitudePoint = MatrixFloat(3, 1, 0.0f);

      MatrixFloat longitudePoint = MatrixFloat(3, 1, 0.0f);

      for (unsigned int latitudeIndex = 1; latitudeIndex < numberOfLat - 1; latitudeIndex++) {

        latitudePoint.assign(initialPoint);

        MatrixFloat::assignAsRotationZMatrix(
          latitudeRotationMatrix, 
          latitudeStepDegree * latitudeIndex
        );
      
        latitudePoint.multiply(latitudeRotationMatrix);

        for (unsigned int longitudeIndex = 0; longitudeIndex < numberOfLong ; longitudeIndex++) {

          longitudePoint.assign(latitudePoint);
          
          MatrixFloat::assignAsRoationYMatrix(
            longitudeRotationMatrix,
            longitudeStepDegree * longitudeIndex
          );
          
          longitudePoint.multiply(longitudeRotationMatrix);

          vertices.emplace_back(longitudePoint.clone());

        }

      }

      vertices.emplace_back(initialPoint.clone());
      
      initialPoint.set(1, 0, -1 * radius);

      vertices.emplace_back(initialPoint.clone());
    
    }

    const auto sideLatCount = numberOfLat - 2;

    auto convertLatAndLongToVertices = [numberOfLat,numberOfLong,vertices,sideLatCount](unsigned int latIndex,unsigned int longIndex) {
      assert(
        latIndex >= 0 && latIndex<numberOfLat && 
        "ShapeGenerator::sphere::convertLatAndLongToVertices latIndex must be above 0 and bellow numberOfLat"
      );
      assert(
        longIndex >= 0 && longIndex<numberOfLong && 
        "ShapeGenerator::sphere::convertLatAndLongToVertices longIndex must be above 0 and bellow numberOfLong"
      );
      assert(
        (latIndex<sideLatCount || longIndex == 0) &&
        "ShapeGenerator::sphere::convertLatAndLongToVertices when latIndex is 0 or numberOfLat-1 longIndex must be 0"
      );
      unsigned int index = 0u;
      if (latIndex >= sideLatCount) {
        index = numberOfLong * sideLatCount + latIndex - sideLatCount;
      } else {
        index = latIndex * numberOfLong + longIndex;
      }
      return index;
    };

    std::vector<std::unique_ptr<Surface>> indices;

    {//Filling indices
      
      for (unsigned int i = 0; i < numberOfLong; i++) {
        indices.emplace_back(std::make_unique<Surface>(
          texture,
          convertLatAndLongToVertices(sideLatCount, 0),
          convertLatAndLongToVertices(1, (i + 1) % numberOfLong),
          convertLatAndLongToVertices(1, i % numberOfLong)
        ));
      }
      
      for (unsigned int i = 0; i < numberOfLong; i++) {
        indices.emplace_back(std::make_unique<Surface>(
          texture,
          convertLatAndLongToVertices(sideLatCount - 1, i % numberOfLong),
          convertLatAndLongToVertices(sideLatCount - 1, (i + 1) % numberOfLong),
          convertLatAndLongToVertices(sideLatCount + 1, 0)
        ));
      }
      
      for (unsigned int i = 0; i < sideLatCount - 1; i++) {
        for (unsigned int j = 0; j < numberOfLong; j++) {
          indices.emplace_back(std::make_unique<Surface>(
            texture,
            convertLatAndLongToVertices(i, j% numberOfLong),
            convertLatAndLongToVertices(i, (j + 1) % numberOfLong),
            convertLatAndLongToVertices(i + 1, (j + 1) % numberOfLong)
          ));
          indices.emplace_back(std::make_unique<Surface>(
            texture,
            convertLatAndLongToVertices(i + 1, j% numberOfLong),
            convertLatAndLongToVertices(i, j% numberOfLong),
            convertLatAndLongToVertices(i + 1, (j + 1) % numberOfLong)
          ));
        }
      }

    }

    std::vector<MatrixFloat> normals = Shape3d::generateNormals(indices, vertices, Shape3d::NormalType::smooth);

    return std::make_unique<Shape3d>(
      vertices,
      indices,
      normals,
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