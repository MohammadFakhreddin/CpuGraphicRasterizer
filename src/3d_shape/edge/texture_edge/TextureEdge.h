#ifndef TextureEdge_class
#define TextureEdge_class

#include "../../../fa_texture/FaTexture.h"
#include "./../base_edge/BaseEdge.h"
#include <memory>

class TextureEdge : public BaseEdge
{
private:
  Vec2DFloat edge1TexturePoint;
  Vec2DFloat edge2TexturePoint;
  Vec2DFloat edge3TexturePoint;
  std::unique_ptr<FaTexture>& textureReference;
  float textureVirtualWidth;
  float textureVirtualHeight;
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
  void render(std::vector<MatrixFloat>* worldPoints) override;
  EdgeType getEdgeType() override;
};

#endif