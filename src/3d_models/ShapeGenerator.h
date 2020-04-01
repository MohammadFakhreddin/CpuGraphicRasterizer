#ifndef ShapeGenerator_Class
#define ShapeGenerator_Class

#include <memory>

#include "../3d_shape/Shape3d.h"
#include "../fa_texture/FaTexture.h"
#include "../3d_shape/surface/base_surface/BaseSurface.h"
#include "../3d_shape/surface/color_surface/ColorSurface.h"
#include "../utils/math/Math.h"
#include "../data_types/MatrixTemplate.h"
#include "../utils/math/Math.h"

class ShapeGenerator {

public:


  static std::unique_ptr<Shape3d> generateColored3DCube(
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
    std::vector<std::unique_ptr<BaseSurface>> edgeList;
    edgeList.emplace_back(std::make_unique<ColorSurface>(2, 1, 0, 245.0f / 255.0f, 144.0f / 255.0f, 66.0f / 255.0f));
    edgeList.emplace_back(std::make_unique<ColorSurface>(3, 1, 2, 245.0f / 255.0f, 144.0f / 255.0f, 66.0f / 255.0f));
    edgeList.emplace_back(std::make_unique<ColorSurface>(1, 3, 5, 221.0f / 255.0f, 245.0f / 255.0f, 66.0f / 255.0f));
    edgeList.emplace_back(std::make_unique<ColorSurface>(5, 3, 7, 221.0f / 255.0f, 245.0f / 255.0f, 66.0f / 255.0f));
    edgeList.emplace_back(std::make_unique<ColorSurface>(4, 0, 5, 66.0f / 255.0f, 245.0f / 255.0f, 126.0f / 255.0f));
    edgeList.emplace_back(std::make_unique<ColorSurface>(5, 0, 1, 66.0f / 255.0f, 245.0f / 255.0f, 126.0f / 255.0f));
    edgeList.emplace_back(std::make_unique<ColorSurface>(6, 0, 4, 66.0f / 255.0f, 194.0f / 255.0f, 245.0f / 255.0f));
    edgeList.emplace_back(std::make_unique<ColorSurface>(2, 0, 6, 66.0f / 255.0f, 194.0f / 255.0f, 245.0f / 255.0f));
    edgeList.emplace_back(std::make_unique<ColorSurface>(7, 2, 6, 96.0f / 255.0f, 66.0f / 255.0f, 245.0f / 255.0f));
    edgeList.emplace_back(std::make_unique<ColorSurface>(3, 2, 7, 96.0f / 255.0f, 66.0f / 255.0f, 245.0f / 255.0f));
    edgeList.emplace_back(std::make_unique<ColorSurface>(7, 4, 5, 245.0f / 255.0f, 66.0f / 255.0f, 123.0f / 255.0f));
    edgeList.emplace_back(std::make_unique<ColorSurface>(6, 4, 7, 245.0f / 255.0f, 66.0f / 255.0f, 123.0f / 255.0f));
    return ShapeGenerator::generateTextured3DCube(
      edgeList,
      w,
      h,
      d,
      transformX,
      transformY,
      transformZ,
      rotationX,
      rotationY,
      rotationZ,
      scale
    );
  }

  static std::unique_ptr<Shape3d> generateTextured3DCube(
    std::vector<std::unique_ptr<BaseSurface>>& edgeList,
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
    //TODO Move these two methods into new class
    //Also nodeList for cube is only part that is needed    
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
    return std::make_unique<Shape3d>(
      nodeList,
      edgeList,
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
    const float& radius,
    const unsigned int& numberOfLat,
    const unsigned int& numberOfLong,
    const Vec3DFloat& color,
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
      MatrixFloat vertexPoint = MatrixFloat(3, 1, 0.0f);
      vertexPoint.set(2, 0, radius);
      
      const auto latitudeStepDegree = Math::piFloat / float(numberOfLat);

      MatrixFloat latitudeRotationMatrix = MatrixFloat(3, 3, 0.0f);
      //TODO We have to refactor and have rotation matrix generators inside matrix class instead
      latitudeRotationMatrix.set(1, 1, cosf(latitudeStepDegree));
      latitudeRotationMatrix.set(1, 2, sinf(latitudeStepDegree));
      latitudeRotationMatrix.set(2, 1, -sinf(latitudeStepDegree));
      latitudeRotationMatrix.set(2, 2, cosf(latitudeStepDegree));

      const auto longitudeStepDegree = (Math::piFloat * 2.0f) / float(numberOfLong);

      MatrixFloat longitudeRotationMatrix = MatrixFloat(3, 3, 0.0f);
      longitudeRotationMatrix.set(0, 0, cosf(longitudeStepDegree));
      longitudeRotationMatrix.set(0, 1, -sinf(longitudeStepDegree));
      longitudeRotationMatrix.set(1, 0, sinf(longitudeStepDegree));
      longitudeRotationMatrix.set(1, 1, cosf(longitudeStepDegree));
      //longitudeRotationMatrix.set(0, 0, cosf(longitudeStepDegree));
      //longitudeRotationMatrix.set(0, 2, sinf(longitudeStepDegree));
      //longitudeRotationMatrix.set(2, 0, -sinf(longitudeStepDegree));
      //longitudeRotationMatrix.set(2, 2, cosf(longitudeStepDegree));

      //MatrixFloat temporaryVertexPoint = MatrixFloat(3, 1, 0.0f);

      //MatrixFloat finalPointLocation = MatrixFloat(3, 1, 0.0f);
      //finalPointLocation.set(2, 0, -1 * radius);

      vertices.emplace_back(vertexPoint.clone());
      for (unsigned int latitudeIndex = 0; latitudeIndex < numberOfLat - 1; latitudeIndex++) {
        vertexPoint.multiply(latitudeRotationMatrix);
        //temporaryVertexPoint.assign(vertexPoint);
        for (unsigned int longitudeIndex = 0; longitudeIndex < numberOfLong - 1; longitudeIndex++) {
          vertexPoint.multiply(longitudeRotationMatrix);
          vertices.emplace_back(vertexPoint.clone());
        }
        //vertexPoint.assign(temporaryVertexPoint);
        vertexPoint.multiply(longitudeRotationMatrix);
        /*if (vertexPoint.equal(temporaryVertexPoint) == false) {
          Logger::log("Point is:");
          vertexPoint.print();
          Logger::log("Iniital point is:");
          temporaryVertexPoint.print();
          Logger::exception("Vertex point is not equal to its inital value");
        }*/
      }
      vertexPoint.multiply(latitudeRotationMatrix);
      vertices.emplace_back(vertexPoint.clone());
      //vertexPoint.print();
      //finalPointLocation.print();
      //assert(vertexPoint.equal(finalPointLocation));
    }

    auto convertLatAndLongToVertices = [numberOfLat,numberOfLong,vertices](unsigned int latIndex,unsigned int longIndex) {
      assert(
        latIndex >= 0 && latIndex<numberOfLat && 
        "ShapeGenerator::sphere::convertLatAndLongToVertices latIndex must be above 0 and bellow numberOfLat"
      );
      assert(
        longIndex >= 0 && longIndex<numberOfLong && 
        "ShapeGenerator::sphere::convertLatAndLongToVertices longIndex must be above 0 and bellow numberOfLong"
      );
      assert(
        ((latIndex != 0 && latIndex!=numberOfLat-1) || longIndex == 0) &&
        "ShapeGenerator::sphere::convertLatAndLongToVertices when latIndex is 0 or numberOfLat-1 longIndex must be 0"
      );
      unsigned int index = 0u;
      if (latIndex == 0) {
        index = 0u;
      } else if (longIndex == numberOfLat - 1) {
        index  = vertices.size() - 1;
      } else {
        index = (latIndex - 1) * numberOfLong + longIndex;
      }
      return index;
    };

    std::vector<std::unique_ptr<BaseSurface>> indices;

    {//Filling indices
      /*
      for (unsigned int i = 0; i < numberOfLong; i++) {
        indices.emplace_back(std::make_unique<ColorSurface>(
          convertLatAndLongToVertices(0, 0),
          convertLatAndLongToVertices(1, i % numberOfLong),
          convertLatAndLongToVertices(1, (i + 1) % numberOfLong),
          color.getX(),
          color.getY(),
          color.getZ()
        ));
      }*/
      
      /*for (unsigned int i = 0; i < numberOfLong; i++) {
        indices.emplace_back(std::make_unique<ColorSurface>(
          convertLatAndLongToVertices(numberOfLat - 1, 0),
          convertLatAndLongToVertices(numberOfLat - 2, i % numberOfLong),
          convertLatAndLongToVertices(numberOfLat - 2, (i + 1) % numberOfLong),
          color.getX(),
          color.getY(),
          color.getZ()
        ));
      }*/

    /*for (unsigned int i = 1; i < numberOfLat - 2; i++) {
        for (unsigned int j = 0; j <= numberOfLong; j++) {
          indices.emplace_back(std::make_unique<ColorSurface>(
            convertLatAndLongToVertices(i, (j + 1) % numberOfLong),
            convertLatAndLongToVertices(i, j % numberOfLong),
            convertLatAndLongToVertices(i + 1, (j + 1) % numberOfLong),
            color.getX(),
            color.getY(),
            color.getZ()
          ));
          indices.emplace_back(std::make_unique<ColorSurface>(
            convertLatAndLongToVertices(i + 1, j % numberOfLong),
            convertLatAndLongToVertices(i, j % numberOfLong),
            convertLatAndLongToVertices(i + 1, (j + 1) % numberOfLong),
            color.getX(),
            color.getY(),
            color.getZ()
          ));
        }
      }
    }*/

    return std::make_unique<Shape3d>(
      vertices,
      indices,
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
