#include "./CharGlyph.h"

#include <cassert>

CharGlyph::CharGlyph(
  Shape3d* shape,
  ImageTexture* texture,
  const float& originalWidth,
  const float& originalHeight
)
  :
  shape(std::unique_ptr<Shape3d>(shape)),
  texture(std::unique_ptr<ImageTexture>(texture)),
  originalWidth(originalWidth),
  originalHeight(originalHeight)
{
  assert(shape != nullptr);
  assert(texture != nullptr);
}

std::unique_ptr<Shape3d>& CharGlyph::getShape()
{
  return shape;
}

std::unique_ptr<ImageTexture>& CharGlyph::getTexture()
{
  return texture;
}

const float& CharGlyph::getWidth() {
  return originalWidth;
}

const float& CharGlyph::getHeight() {
  return originalHeight;
}
