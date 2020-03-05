#include "./../data_types/VectorTemplate.h"
#include "../3d_shape/edge/texture_edge/TextureEdge.h"
#include <string>

class Dice{
public:
  const std::string diceTextureAddress = "src/assets/dice_texture.png";
	const float diceCubeWidth = 400;
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
	std::vector<BaseEdge*> diceCubeEdgeList = std::vector<BaseEdge*>{
    //1
    new TextureEdge(0,1,2,
      diceCubeTexture,
      Vec2DFloat(0,0), 
      Vec2DFloat(diceCubeSquare,0), 
      Vec2DFloat(0,diceCubeSquare)
    ),
    //2
    new TextureEdge(3,1,2,diceCubeTexture,
      Vec2DFloat(0,diceCubeSquare), 
      Vec2DFloat(diceCubeSquare,0), 
      Vec2DFloat(diceCubeSquare,diceCubeSquare)
    ),
    //3
    new TextureEdge(3,1,5,diceCubeTexture,
      Vec2DFloat(diceCubeSquare,0), 
      Vec2DFloat(2 * diceCubeSquare,0), 
      Vec2DFloat(diceCubeSquare,diceCubeSquare)
    ),
    //4
    new TextureEdge(3,7,5,diceCubeTexture,
      Vec2DFloat(diceCubeSquare,diceCubeSquare), 
      Vec2DFloat(diceCubeSquare * 2,0), 
      Vec2DFloat(diceCubeSquare * 2,diceCubeSquare)
    ),
    //5
    new TextureEdge(0,4,5,diceCubeTexture,
      Vec2DFloat(diceCubeSquare * 2,0), 
      Vec2DFloat(3 * diceCubeSquare - 1,0), 
      Vec2DFloat(diceCubeSquare*2,diceCubeSquare)
    ),
    //6
    new TextureEdge(5,0,1,diceCubeTexture,
      Vec2DFloat(diceCubeSquare * 2,diceCubeSquare), 
      Vec2DFloat(diceCubeSquare * 3 - 1,0), 
      Vec2DFloat(diceCubeSquare * 3 - 1,diceCubeSquare)
    ),
    //7
    new TextureEdge(4,0,6,diceCubeTexture,
      Vec2DFloat(0,diceCubeSquare), 
      Vec2DFloat(diceCubeSquare,diceCubeSquare), 
      Vec2DFloat(0,diceCubeSquare * 2 -1)
    ),//8
    new TextureEdge(2,0,6,diceCubeTexture,
      Vec2DFloat(0,diceCubeSquare * 2-1), 
      Vec2DFloat(diceCubeSquare,diceCubeSquare), 
      Vec2DFloat(diceCubeSquare,diceCubeSquare * 2-1)
    ),//9
    new TextureEdge(2,7,6,diceCubeTexture,
      Vec2DFloat(diceCubeSquare,diceCubeSquare), 
      Vec2DFloat(diceCubeSquare*2,diceCubeSquare), 
      Vec2DFloat(diceCubeSquare,diceCubeSquare * 2 -1)
    ),
    //10
    new TextureEdge(2,7,3,diceCubeTexture,
      Vec2DFloat(diceCubeSquare*1,diceCubeSquare * 2-1), 
      Vec2DFloat(diceCubeSquare*2,diceCubeSquare), 
      Vec2DFloat(diceCubeSquare*2,diceCubeSquare * 2-1)
    ),
    //11
    new TextureEdge(4,5,7,diceCubeTexture,
      Vec2DFloat(diceCubeSquare*2,diceCubeSquare), 
      Vec2DFloat(diceCubeSquare*3-1,diceCubeSquare), 
      Vec2DFloat(diceCubeSquare*2,diceCubeSquare * 2 -1)
    ),
    //12
    new TextureEdge(4,6,7,diceCubeTexture,
      Vec2DFloat(diceCubeSquare*2,diceCubeSquare * 2-1), 
      Vec2DFloat(diceCubeSquare*3-1,diceCubeSquare), 
      Vec2DFloat(diceCubeSquare*3-1,diceCubeSquare * 2-1)
    )
  };
};