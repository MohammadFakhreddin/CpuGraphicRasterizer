#ifndef TextureEdge_class
#define TextureEdge_class

#include "../../../fa_texture/FaTexture.h"
#include "./../base_edge/BaseEdge.h"
#include <memory>

class TextureEdge : public BaseEdge
{
private:
  static constexpr bool DEBUG_MODE = false;

  Vec2DFloat edge1TexturePoint;
  Vec2DFloat edge2TexturePoint;
  Vec2DFloat edge3TexturePoint;
  std::unique_ptr<FaTexture>& textureReference;
public:
  TextureEdge(
    int edge1,
    int edge2,
    int edge3,
    std::unique_ptr<FaTexture>& textureReference,
    Vec2DFloat edge1TexturePoint,
    Vec2DFloat edge2TexturePoint,
    Vec2DFloat edge3TexturePoint
  );
  void render(
    Camera& cameraInstance,
    std::vector<MatrixFloat>* worldPoints,
    float transformCenterX,
    float transformCenterY,
    float transformCenterZ
  ) override;
  EdgeType getEdgeType() override;
};

#endif