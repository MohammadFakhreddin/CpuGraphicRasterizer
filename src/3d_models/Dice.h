#ifndef Dice_class
#define Dice_class

#include <string>

#include "./../data_types/VectorTemplate.h"
#include "../3d_shape/surface/texture_surface/TextureSurface.h"
#include "../utils/path/Path.h"

class Dice{
public:
  const std::string diceTextureAddress = Path::generateAssetPath("dice_texture",".png");
	const float diceCubeWidth = 300;
	const float diceCubeHeight = 200;
	const float diceCubeSquare = 100;
public:
	std::unique_ptr<FaTexture> diceCubeTexture = std::unique_ptr<FaTexture>(
		new FaTexture(
			diceTextureAddress,
			diceCubeWidth,
			diceCubeHeight
		)
	);
public:
	std::vector<BaseSurface*> diceCubeEdgeList = std::vector<BaseSurface*>{
    //1
    new TextureSurface(0,1,2,
      diceCubeTexture,
      Vec2DFloat(diceCubeSquare,diceCubeSquare),
      Vec2DFloat(0,diceCubeSquare), 
      Vec2DFloat(diceCubeSquare,0)
    ),
    //2
    new TextureSurface(3,1,2,diceCubeTexture,
      Vec2DFloat(0,0),
      Vec2DFloat(0,diceCubeSquare),
      Vec2DFloat(diceCubeSquare,0) 
    ),
    //3
    new TextureSurface(1,3,5,diceCubeTexture,
      Vec2DFloat(2 * diceCubeSquare,diceCubeSquare),
      Vec2DFloat(diceCubeSquare,diceCubeSquare),
      Vec2DFloat(2 * diceCubeSquare,0)
    ),
    //4
    new TextureSurface(7,3,5,diceCubeTexture,
      Vec2DFloat(diceCubeSquare,0),
      Vec2DFloat(diceCubeSquare,diceCubeSquare),
      Vec2DFloat(diceCubeSquare * 2,0)
    ),
    //5
    new TextureSurface(4,0,5,diceCubeTexture,
      Vec2DFloat(3 * diceCubeSquare - 1,diceCubeSquare), 
      Vec2DFloat(diceCubeSquare * 2,diceCubeSquare), 
      Vec2DFloat(diceCubeSquare * 3 -1,0)
    ),
    //6
    new TextureSurface(1,0,5,diceCubeTexture,
      Vec2DFloat(diceCubeSquare * 2,0), 
      Vec2DFloat(diceCubeSquare * 2,diceCubeSquare), 
      Vec2DFloat(diceCubeSquare * 3 - 1,0)
    ),
    //7
    new TextureSurface(4,0,6,diceCubeTexture,
      Vec2DFloat(diceCubeSquare,diceCubeSquare * 2 - 1),
      Vec2DFloat(0,diceCubeSquare * 2 - 1), 
      Vec2DFloat(diceCubeSquare,diceCubeSquare)
    ),
    //8
    new TextureSurface(2,0,6,diceCubeTexture,
      Vec2DFloat(0,diceCubeSquare),
      Vec2DFloat(0,diceCubeSquare * 2 - 1),
      Vec2DFloat(diceCubeSquare,diceCubeSquare) 
    ),
    //9
    new TextureSurface(6,2,7,diceCubeTexture,
      Vec2DFloat(diceCubeSquare * 2,diceCubeSquare * 2 - 1),
      Vec2DFloat(diceCubeSquare,diceCubeSquare * 2 - 1), 
      Vec2DFloat(diceCubeSquare * 2,diceCubeSquare)
    ),
    // //10
    new TextureSurface(3,2,7,diceCubeTexture,
      Vec2DFloat(diceCubeSquare,diceCubeSquare),
      Vec2DFloat(diceCubeSquare,diceCubeSquare * 2 - 1),
      Vec2DFloat(diceCubeSquare * 2,diceCubeSquare) 
    ),
    // //11
    new TextureSurface(5,4,7,diceCubeTexture,
      Vec2DFloat(diceCubeSquare * 3 - 1,diceCubeSquare * 2 - 1),
      Vec2DFloat(diceCubeSquare * 2,diceCubeSquare * 2 - 1), 
      Vec2DFloat(diceCubeSquare * 3 - 1,diceCubeSquare)
    ),
    // //12
    new TextureSurface(6,4,7,diceCubeTexture,
      Vec2DFloat(diceCubeSquare * 2,diceCubeSquare),
      Vec2DFloat(diceCubeSquare * 2,diceCubeSquare * 2 - 1),
      Vec2DFloat(diceCubeSquare * 3 - 1,diceCubeSquare) 
    )
  };
};

#endif