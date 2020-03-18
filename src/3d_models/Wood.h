#ifndef Wood_class
#define Wood_class

#include <string>

#include "./../data_types/VectorTemplate.h"
#include "../3d_shape/surface/base_surface/BaseSurface.h"
#include "../3d_shape/surface/texture_surface/TextureSurface.h"
#include "../utils/path/Path.h"

class Wood
{
public:
	const std::string woodCubeTextureAddress = Path::generateAssetPath("wood",".png");
	const float vLength = 100;
public:
	std::unique_ptr<FaTexture> woodCubeTexture = std::unique_ptr<FaTexture>(
		new FaTexture(
                woodCubeTextureAddress,
                vLength,
                vLength
		)
	);
public:
	std::vector<BaseSurface*> woodCubeEdgeList = std::vector<BaseSurface*>{
		new TextureSurface(0, 1, 2, woodCubeTexture, Vec2DFloat(0,0), Vec2DFloat(0, vLength - 1), Vec2DFloat(vLength - 1, vLength - 1)),
		new TextureSurface(3, 1, 2, woodCubeTexture, Vec2DFloat(0,0), Vec2DFloat(vLength - 1, 0), Vec2DFloat(vLength - 1, vLength - 1)),
		new TextureSurface(3, 1, 5, woodCubeTexture, Vec2DFloat(0,0), Vec2DFloat(0, vLength - 1), Vec2DFloat(vLength - 1, vLength - 1)),
		new TextureSurface(3, 7, 5, woodCubeTexture, Vec2DFloat(0,0), Vec2DFloat(vLength - 1, 0), Vec2DFloat(vLength - 1, vLength - 1)),
		new TextureSurface(0, 4, 5, woodCubeTexture, Vec2DFloat(0,0), Vec2DFloat(0, vLength - 1), Vec2DFloat(vLength - 1, vLength - 1)),
		new TextureSurface(5, 0, 1, woodCubeTexture, Vec2DFloat(0,0), Vec2DFloat(vLength - 1, 0), Vec2DFloat(vLength - 1, vLength - 1)),
		new TextureSurface(4, 0, 6, woodCubeTexture, Vec2DFloat(0,0), Vec2DFloat(0, vLength - 1), Vec2DFloat(vLength - 1, vLength - 1)),
		new TextureSurface(2, 0, 6, woodCubeTexture, Vec2DFloat(0,0), Vec2DFloat(vLength - 1, 0), Vec2DFloat(vLength - 1, vLength - 1)),
		new TextureSurface(2, 7, 6, woodCubeTexture, Vec2DFloat(0,0), Vec2DFloat(0, vLength - 1), Vec2DFloat(vLength - 1, vLength - 1)),
		new TextureSurface(2, 7, 3, woodCubeTexture, Vec2DFloat(0,0), Vec2DFloat(vLength - 1, 0), Vec2DFloat(vLength - 1, vLength - 1)),
		new TextureSurface(4, 5, 7, woodCubeTexture, Vec2DFloat(0,0), Vec2DFloat(0, vLength - 1), Vec2DFloat(vLength - 1, vLength - 1)),
		new TextureSurface(4, 6, 7, woodCubeTexture, Vec2DFloat(0,0), Vec2DFloat(vLength - 1, 0), Vec2DFloat(vLength - 1, vLength - 1))
	};
};

#endif