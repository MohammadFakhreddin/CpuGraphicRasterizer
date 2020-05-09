#ifndef CharGlyph_Class
#define CharGlyph_Class

#include <memory>

#include "../../texture/ImageTexture/ImageTexture.h"
#include "../../3d/shape/Shape3d.h"

class CharGlyph {
public:

  CharGlyph() = delete;

  CharGlyph(
    Shape3d* shape, 
    ImageTexture* texture, 
    const float& originalWidth, 
    const float& originalHeight
  );

  CharGlyph(CharGlyph& other) = delete;

  CharGlyph(CharGlyph&& other) = delete;

  CharGlyph& operator=(const CharGlyph& other) = delete;// copy assignment

  CharGlyph& operator=(CharGlyph&& other) = delete; // move assignment
  
  std::unique_ptr<Shape3d>& getShape();

  std::unique_ptr<ImageTexture>& getTexture();

  const float& getWidth();

  const float& getHeight();

private:

  float originalWidth;

  float originalHeight;

  std::unique_ptr<Shape3d> shape;

  std::unique_ptr<ImageTexture> texture;

};

#endif // !Char_Class
