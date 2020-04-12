#ifndef DiceCube_class
#define DiceCube_class

#include <string>
#include <memory>

#include "./../data_types/VectorTemplate.h"
#include "../utils/path/Path.h"
#include "./ShapeGenerator.h"
#include "../texture/ImageTexture/ImageTexture.h"

class DiceCube{

private:

  float diceCubeWidth = 300.0f;
  float diceCubeHeight = 200.0f;
  float diceCubeSquare = 100.0f;

  std::unique_ptr<ImageTexture> diceCubeTexture = std::make_unique<ImageTexture>(
    Path::generateAssetPath("dice_texture", ".png"),
    diceCubeWidth,
    diceCubeHeight
  );

public:

  std::unique_ptr<Shape3d> generateCube(
    Surface::LightPercision percision,
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

    //TODO We need constructors to auto generate normals
    std::vector<std::unique_ptr<Surface>> diceCubeEdgeList;
      //1
    diceCubeEdgeList.emplace_back(std::make_unique<Surface>(
      percision,
      (std::unique_ptr<Texture>&)diceCubeTexture,
      2, 1, 0
    ));
    diceCubeEdgeList.back()->setTextureCoordinates(0, diceCubeSquare, 0);
    diceCubeEdgeList.back()->setTextureCoordinates(1, 0, diceCubeSquare);
    diceCubeEdgeList.back()->setTextureCoordinates(2, diceCubeSquare, diceCubeSquare);
      //2
    diceCubeEdgeList.emplace_back(std::make_unique<Surface>(
      percision,
      (std::unique_ptr<Texture>&)diceCubeTexture,
      3, 1, 2
    ));
    diceCubeEdgeList.back()->setTextureCoordinates(0, 0, 0);
    diceCubeEdgeList.back()->setTextureCoordinates(1, 0, diceCubeSquare);
    diceCubeEdgeList.back()->setTextureCoordinates(2, diceCubeSquare, 0);
      //3
    diceCubeEdgeList.emplace_back(std::make_unique<Surface>(
      percision,
      (std::unique_ptr<Texture>&)diceCubeTexture,
      1, 3, 5 
    ));
    diceCubeEdgeList.back()->setTextureCoordinates(0, 2 * diceCubeSquare, diceCubeSquare);
    diceCubeEdgeList.back()->setTextureCoordinates(1, diceCubeSquare, diceCubeSquare);
    diceCubeEdgeList.back()->setTextureCoordinates(2, 2 * diceCubeSquare, 0);
      //4
    diceCubeEdgeList.emplace_back(std::make_unique<Surface>(
      percision,
      (std::unique_ptr<Texture>&)diceCubeTexture,
      5, 3, 7
    ));
    diceCubeEdgeList.back()->setTextureCoordinates(0, diceCubeSquare * 2, 0);
    diceCubeEdgeList.back()->setTextureCoordinates(1, diceCubeSquare, diceCubeSquare);
    diceCubeEdgeList.back()->setTextureCoordinates(2, diceCubeSquare, 0);
    // 5
    diceCubeEdgeList.emplace_back(std::make_unique<Surface>(
      percision,
      (std::unique_ptr<Texture>&)diceCubeTexture,
      4, 0, 5
    ));
    diceCubeEdgeList.back()->setTextureCoordinates(0, 3 * diceCubeSquare - 1, diceCubeSquare);
    diceCubeEdgeList.back()->setTextureCoordinates(1, diceCubeSquare * 2, diceCubeSquare);
    diceCubeEdgeList.back()->setTextureCoordinates(2, diceCubeSquare * 3 - 1, 0);
    // 6
    diceCubeEdgeList.emplace_back(std::make_unique<Surface>(
      percision,
      (std::unique_ptr<Texture>&)diceCubeTexture,
      5, 0, 1
    ));
    diceCubeEdgeList.back()->setTextureCoordinates(0, diceCubeSquare * 3 - 1, 0);
    diceCubeEdgeList.back()->setTextureCoordinates(1, diceCubeSquare * 2, diceCubeSquare);
    diceCubeEdgeList.back()->setTextureCoordinates(2, diceCubeSquare * 2, 0);
      //7
    diceCubeEdgeList.emplace_back(std::make_unique<Surface>(
      percision,
      (std::unique_ptr<Texture>&)diceCubeTexture,
      6, 0, 4
    ));
    diceCubeEdgeList.back()->setTextureCoordinates(0, diceCubeSquare, diceCubeSquare);
    diceCubeEdgeList.back()->setTextureCoordinates(1, 0, diceCubeSquare * 2 - 1);
    diceCubeEdgeList.back()->setTextureCoordinates(2, diceCubeSquare, diceCubeSquare * 2 - 1);
      //8
    diceCubeEdgeList.emplace_back(std::make_unique<Surface>(
      percision,
      (std::unique_ptr<Texture>&)diceCubeTexture,
      2, 0, 6
    ));
    diceCubeEdgeList.back()->setTextureCoordinates(0, 0, diceCubeSquare);
    diceCubeEdgeList.back()->setTextureCoordinates(1, 0, diceCubeSquare * 2 - 1);
    diceCubeEdgeList.back()->setTextureCoordinates(2, diceCubeSquare, diceCubeSquare);
      //9
    diceCubeEdgeList.emplace_back(std::make_unique<Surface>(
      percision,
      (std::unique_ptr<Texture>&)diceCubeTexture,
      7, 2, 6
    ));
    diceCubeEdgeList.back()->setTextureCoordinates(0, diceCubeSquare * 2, diceCubeSquare);
    diceCubeEdgeList.back()->setTextureCoordinates(1, diceCubeSquare, diceCubeSquare * 2 - 1);
    diceCubeEdgeList.back()->setTextureCoordinates(2, diceCubeSquare * 2, diceCubeSquare * 2 - 1);
      //10
    diceCubeEdgeList.emplace_back(std::make_unique<Surface>(
      percision,
      (std::unique_ptr<Texture>&)diceCubeTexture,
      3, 2, 7
    ));
    diceCubeEdgeList.back()->setTextureCoordinates(0, diceCubeSquare, diceCubeSquare);
    diceCubeEdgeList.back()->setTextureCoordinates(1, diceCubeSquare, diceCubeSquare * 2 - 1);
    diceCubeEdgeList.back()->setTextureCoordinates(2, diceCubeSquare * 2, diceCubeSquare);
      //11
    diceCubeEdgeList.emplace_back(std::make_unique<Surface>(
      percision,
      (std::unique_ptr<Texture>&)diceCubeTexture,
      7, 4, 5
    ));
    diceCubeEdgeList.back()->setTextureCoordinates(0, diceCubeSquare * 3 - 1, diceCubeSquare);
    diceCubeEdgeList.back()->setTextureCoordinates(1, diceCubeSquare * 2, diceCubeSquare * 2 - 1);
    diceCubeEdgeList.back()->setTextureCoordinates(2, diceCubeSquare * 3 - 1, diceCubeSquare * 2 - 1);
      //12
    diceCubeEdgeList.emplace_back(std::make_unique<Surface>(
      percision,
      (std::unique_ptr<Texture>&)diceCubeTexture,
      6, 4, 7
    ));
    diceCubeEdgeList.back()->setTextureCoordinates(0, diceCubeSquare * 2, diceCubeSquare);
    diceCubeEdgeList.back()->setTextureCoordinates(1, diceCubeSquare * 2, diceCubeSquare * 2 - 1);
    diceCubeEdgeList.back()->setTextureCoordinates(2, diceCubeSquare * 3 - 1, diceCubeSquare);

    return ShapeGenerator::cube( 
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