#ifndef ColorCube_Class
#define ColorCube_Class

#include <string>
#include <memory>

#include "./ShapeGenerator.h"
#include "../../texture/ColorTexture/ColorTexture.h"
#include "../shape/Shape3d.h"

class ColorCube {

private:

  std::unique_ptr<ColorTexture> face1Color = std::make_unique<ColorTexture>(245.0f / 255.0f, 144.0f / 255.0f, 66.0f / 255.0f);
  std::unique_ptr<ColorTexture> face2Color = std::make_unique<ColorTexture>(221.0f / 255.0f, 245.0f / 255.0f, 66.0f / 255.0f);
  std::unique_ptr<ColorTexture> face3Color = std::make_unique<ColorTexture>(66.0f / 255.0f, 245.0f / 255.0f, 126.0f / 255.0f);
  std::unique_ptr<ColorTexture> face4Color = std::make_unique<ColorTexture>(66.0f / 255.0f, 194.0f / 255.0f, 245.0f / 255.0f);
  std::unique_ptr<ColorTexture> face5Color = std::make_unique<ColorTexture>(96.0f / 255.0f, 66.0f / 255.0f, 245.0f / 255.0f);
  std::unique_ptr<ColorTexture> face6Color = std::make_unique<ColorTexture>(245.0f / 255.0f, 66.0f / 255.0f, 123.0f / 255.0f);

public:

  std::unique_ptr<Shape3d> generateCube(
    Constants::LightPrecision lightPrecision,
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

    std::vector<std::unique_ptr<Surface>> surfaceList;
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, face1Color.get(), 2, 1, 0));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, face1Color.get(), 3, 1, 2));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, face2Color.get(), 1, 3, 5));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, face2Color.get(), 5, 3, 7));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, face3Color.get(), 4, 0, 5));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, face3Color.get(), 5, 0, 1));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, face4Color.get(), 6, 0, 4));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, face4Color.get(), 2, 0, 6));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, face5Color.get(), 7, 2, 6));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, face5Color.get(), 3, 2, 7));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, face6Color.get(), 7, 4, 5));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, face6Color.get(), 6, 4, 7));

    return ShapeGenerator::cube(
      surfaceList,
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

};

#endif