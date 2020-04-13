#ifndef ColorCube_Class
#define ColorCube_Class

#include <string>
#include <memory>

#include "./../data_types/VectorTemplate.h"
#include "../utils/path/Path.h"
#include "./ShapeGenerator.h"
#include "../texture/ColorTexture/ColorTexture.h"

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
    Surface::LightPrecision lightPrecision,
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
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, (std::unique_ptr<Texture>&)face1Color, 2, 1, 0));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, (std::unique_ptr<Texture>&)face1Color, 3, 1, 2));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, (std::unique_ptr<Texture>&)face2Color, 1, 3, 5));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, (std::unique_ptr<Texture>&)face2Color, 5, 3, 7));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, (std::unique_ptr<Texture>&)face3Color, 4, 0, 5));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, (std::unique_ptr<Texture>&)face3Color, 5, 0, 1));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, (std::unique_ptr<Texture>&)face4Color, 6, 0, 4));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, (std::unique_ptr<Texture>&)face4Color, 2, 0, 6));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, (std::unique_ptr<Texture>&)face5Color, 7, 2, 6));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, (std::unique_ptr<Texture>&)face5Color, 3, 2, 7));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, (std::unique_ptr<Texture>&)face6Color, 7, 4, 5));
    surfaceList.emplace_back(std::make_unique<Surface>(lightPrecision, (std::unique_ptr<Texture>&)face6Color, 6, 4, 7));

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