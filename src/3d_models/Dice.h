#ifndef Dice_class
#define Dice_class

#include <string>
#include <memory>

#include "./../data_types/VectorTemplate.h"
#include "../3d_shape/surface/texture_surface/TextureSurface.h"
#include "../utils/path/Path.h"
#include "./ShapeGenerator.h"



class Dice{

private:

  float diceCubeWidth = 300.0f;
  float diceCubeHeight = 200.0f;
  float diceCubeSquare = 100.0f;

  std::unique_ptr<FaTexture> diceCubeTexture = std::make_unique<FaTexture>(
    Path::generateAssetPath("dice_texture", ".png"),
    diceCubeWidth,
    diceCubeHeight
  );

public:

  std::unique_ptr<Shape3d> generateCube(
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


    std::vector<std::unique_ptr<Surface>> diceCubeEdgeList;
      //1
    diceCubeEdgeList.emplace_back(std::make_unique<TextureSurface>(2, 1, 0,
      diceCubeTexture,
      Vec2DFloat(diceCubeSquare, 0),
      Vec2DFloat(0, diceCubeSquare),
      Vec2DFloat(diceCubeSquare, diceCubeSquare)
    ));
      //2
    diceCubeEdgeList.emplace_back(std::make_unique<TextureSurface>(3, 1, 2, diceCubeTexture,
      Vec2DFloat(0, 0),
      Vec2DFloat(0, diceCubeSquare),
      Vec2DFloat(diceCubeSquare, 0)
    ));
      //3
    diceCubeEdgeList.emplace_back(std::make_unique<TextureSurface>(1, 3, 5, diceCubeTexture,
      Vec2DFloat(2 * diceCubeSquare, diceCubeSquare),
      Vec2DFloat(diceCubeSquare, diceCubeSquare),
      Vec2DFloat(2 * diceCubeSquare, 0)
    ));
      //4
    diceCubeEdgeList.emplace_back(std::make_unique<TextureSurface>(5, 3, 7, diceCubeTexture,
      Vec2DFloat(diceCubeSquare * 2, 0),
      Vec2DFloat(diceCubeSquare, diceCubeSquare),
      Vec2DFloat(diceCubeSquare, 0)
    ));
    // 5
    diceCubeEdgeList.emplace_back(std::make_unique<TextureSurface>(4, 0, 5, diceCubeTexture,
      Vec2DFloat(3 * diceCubeSquare - 1, diceCubeSquare),
      Vec2DFloat(diceCubeSquare * 2, diceCubeSquare),
      Vec2DFloat(diceCubeSquare * 3 - 1, 0)
    ));
    // 6
    diceCubeEdgeList.emplace_back(std::make_unique<TextureSurface>(5, 0, 1, diceCubeTexture,
      Vec2DFloat(diceCubeSquare * 3 - 1, 0),
      Vec2DFloat(diceCubeSquare * 2, diceCubeSquare),
      Vec2DFloat(diceCubeSquare * 2, 0)
    ));
      //7
    diceCubeEdgeList.emplace_back(std::make_unique<TextureSurface>(6, 0, 4, diceCubeTexture,
      Vec2DFloat(diceCubeSquare, diceCubeSquare),
      Vec2DFloat(0, diceCubeSquare * 2 - 1),
      Vec2DFloat(diceCubeSquare, diceCubeSquare * 2 - 1)
    ));
      //8
    diceCubeEdgeList.emplace_back(std::make_unique<TextureSurface>(2, 0, 6, diceCubeTexture,
      Vec2DFloat(0, diceCubeSquare),
      Vec2DFloat(0, diceCubeSquare * 2 - 1),
      Vec2DFloat(diceCubeSquare, diceCubeSquare)
    ));
      //9
    diceCubeEdgeList.emplace_back(std::make_unique<TextureSurface>(7, 2, 6, diceCubeTexture,
      Vec2DFloat(diceCubeSquare * 2, diceCubeSquare),
      Vec2DFloat(diceCubeSquare, diceCubeSquare * 2 - 1),
      Vec2DFloat(diceCubeSquare * 2, diceCubeSquare * 2 - 1)
    ));
      //10
    diceCubeEdgeList.emplace_back(std::make_unique<TextureSurface>(3, 2, 7, diceCubeTexture,
      Vec2DFloat(diceCubeSquare, diceCubeSquare),
      Vec2DFloat(diceCubeSquare, diceCubeSquare * 2 - 1),
      Vec2DFloat(diceCubeSquare * 2, diceCubeSquare)
    ));
      //11
    diceCubeEdgeList.emplace_back(std::make_unique<TextureSurface>(7, 4, 5, diceCubeTexture,
      Vec2DFloat(diceCubeSquare * 3 - 1, diceCubeSquare),
      Vec2DFloat(diceCubeSquare * 2, diceCubeSquare * 2 - 1),
      Vec2DFloat(diceCubeSquare * 3 - 1, diceCubeSquare * 2 - 1)
    ));
      //12
    diceCubeEdgeList.emplace_back(std::make_unique<TextureSurface>(6, 4, 7, diceCubeTexture,
      Vec2DFloat(diceCubeSquare * 2, diceCubeSquare),
      Vec2DFloat(diceCubeSquare * 2, diceCubeSquare * 2 - 1),
      Vec2DFloat(diceCubeSquare * 3 - 1, diceCubeSquare)
    ));

    return ShapeGenerator::generateTextured3DCube(
      diceCubeEdgeList,
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