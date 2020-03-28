#ifndef TextureSurface_class
#define TextureSurface_class

#include "../../../fa_texture/FaTexture.h"
#include "./../base_surface/BaseSurface.h"
#include <memory>

class TextureSurface : public BaseSurface
{
private:

  static constexpr bool DEBUG_MODE = false;

	void drawTextureBetweenPoints(
    Camera& cameraInstance,
		std::unique_ptr<FaTexture>& texture,
		float triangleStartX,
		float triangleStartY,
		float triangleStartZ,
    float triangleEndX,
		float triangleEndY,
		float triangleEndZ,
    float textureStartX,
		float textureStartY,
    float textureEndX,
		float textureEndY
	);

  void computePixelMapData(
    Camera& cameraInstance,
    std::vector<MatrixFloat>& worldPoints
  ) override;

  Vec2DFloat edge1TexturePoint;
  Vec2DFloat edge2TexturePoint;
  Vec2DFloat edge3TexturePoint;
  std::unique_ptr<FaTexture>& textureReference;

public:

  TextureSurface(
    int edge1,
    int edge2,
    int edge3,
    std::unique_ptr<FaTexture>& textureReference,
    Vec2DFloat edge1TexturePoint,
    Vec2DFloat edge2TexturePoint,
    Vec2DFloat edge3TexturePoint
  );

};

#endif