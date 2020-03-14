#ifndef Wood_class
#define Wood_class

#include "./../data_types/VectorTemplate.h"
#include "../3d_shape/edge/texture_edge/TextureEdge.h"
#include <string>
#include "../utils/path/Path.h"

class Wood
{
public:
	const std::string woodCubeTextureAddress = Path::generateAssetPath("wood",".png");
	const float woodCubeVirtualWidthAndHeight = 100;
public:
	std::unique_ptr<FaTexture> woodCubeTexture = std::unique_ptr<FaTexture>(
		new FaTexture(
			woodCubeTextureAddress,
			woodCubeVirtualWidthAndHeight,
			woodCubeVirtualWidthAndHeight
		)
	);
public:
	std::vector<BaseEdge*> woodCubeEdgeList = std::vector<BaseEdge*>{
		new TextureEdge(0,1,2,woodCubeTexture,Vec2DFloat(0,0), Vec2DFloat(0,woodCubeVirtualWidthAndHeight-1), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,woodCubeVirtualWidthAndHeight-1)),
		new TextureEdge(3,1,2,woodCubeTexture,Vec2DFloat(0,0), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,0), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,woodCubeVirtualWidthAndHeight-1)),
		new TextureEdge(3,1,5,woodCubeTexture,Vec2DFloat(0,0), Vec2DFloat(0,woodCubeVirtualWidthAndHeight-1), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,woodCubeVirtualWidthAndHeight-1)),
		new TextureEdge(3,7,5,woodCubeTexture,Vec2DFloat(0,0), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,0), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,woodCubeVirtualWidthAndHeight-1)),
		new TextureEdge(0,4,5,woodCubeTexture,Vec2DFloat(0,0), Vec2DFloat(0,woodCubeVirtualWidthAndHeight-1), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,woodCubeVirtualWidthAndHeight-1)),
		new TextureEdge(5,0,1,woodCubeTexture,Vec2DFloat(0,0), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,0), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,woodCubeVirtualWidthAndHeight-1)),
		new TextureEdge(4,0,6,woodCubeTexture,Vec2DFloat(0,0), Vec2DFloat(0,woodCubeVirtualWidthAndHeight-1), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,woodCubeVirtualWidthAndHeight-1)),
		new TextureEdge(2,0,6,woodCubeTexture,Vec2DFloat(0,0), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,0), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,woodCubeVirtualWidthAndHeight-1)),
		new TextureEdge(2,7,6,woodCubeTexture,Vec2DFloat(0,0), Vec2DFloat(0,woodCubeVirtualWidthAndHeight-1), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,woodCubeVirtualWidthAndHeight-1)), 
		new TextureEdge(2,7,3,woodCubeTexture,Vec2DFloat(0,0), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,0), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,woodCubeVirtualWidthAndHeight-1)),
		new TextureEdge(4,5,7,woodCubeTexture,Vec2DFloat(0,0), Vec2DFloat(0,woodCubeVirtualWidthAndHeight-1), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,woodCubeVirtualWidthAndHeight-1)),
		new TextureEdge(4,6,7,woodCubeTexture,Vec2DFloat(0,0), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,0), Vec2DFloat(woodCubeVirtualWidthAndHeight-1,woodCubeVirtualWidthAndHeight-1))  
	};
};

#endif