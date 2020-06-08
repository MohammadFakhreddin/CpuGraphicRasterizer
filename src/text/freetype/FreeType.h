#ifndef FontType_Class
#define FontType_Class

#include "./../../open_gl/OpenGl.h"
#ifdef __OPENGL__
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

class FreeType {
public:
  
  struct Glyph {
    unsigned char* buffer;
    unsigned int width;
    unsigned int height;
    float bearingX;
    float bearingY;
  };

  FreeType();
  
  ~FreeType();

  FT_Face* loadFace(
    const std::string& fontName, 
    const int& ptSize, 
    const int& deviceHDPI, 
    const int& deviceVDPI
  ) const;

  void freeFace(FT_Face* face) const;

  Glyph* rasterize(FT_Face* face, uint32_t glyphIndex) const;

  void freeGlyph(Glyph* glyph) const;

private:

  FT_Library lib;

  int forceUcs2CharMap(FT_Face ftf) const;
};
#endif
#endif // !FontType_Class
