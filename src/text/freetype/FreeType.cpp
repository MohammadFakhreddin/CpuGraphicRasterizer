#include "./FreeType.h"

#include <cassert>
#include <cstring>

FreeType::FreeType()
{
  FT_Init_FreeType(&lib);
}

FreeType::~FreeType()
{
  FT_Done_FreeType(lib);
}

FT_Face* FreeType::loadFace(
  const std::string& fontName, 
  const int& ptSize, 
  const int& deviceHDPI, 
  const int& deviceVDPI
) const 
{
  FT_Face* face = new FT_Face;

  FT_Error error = FT_New_Face(lib, fontName.c_str(), 0, face);

  assert(error == 0 && "Cannot open font file");

  forceUcs2CharMap(*face);
  
  FT_Set_Char_Size(*face, 0, ptSize, deviceHDPI, deviceVDPI);

  return face;
}

void FreeType::freeFace(FT_Face* face) const
{
  FT_Done_Face(*face);
  
  delete face;
}

FreeType::Glyph* FreeType::rasterize(FT_Face* face, uint32_t glyphIndex) const
{
  Glyph* g = new Glyph;

  FT_Int32 flags = FT_LOAD_DEFAULT;

  FT_Load_Glyph(*face,
    glyphIndex, // the glyph_index in the font file
    flags
  );

  auto& slot = (*face)->glyph;
  
  FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);

  const auto& ftBitmap = slot->bitmap;

  g->buffer = ftBitmap.buffer;
  
  g->width = ftBitmap.width;
  
  g->height = ftBitmap.rows;
  
  g->bearingX = float(slot->bitmap_left);

  g->bearingY = float(slot->bitmap_top);

  return g;
}

void FreeType::freeGlyph(Glyph* glyph) const
{
  delete glyph;
}

int FreeType::forceUcs2CharMap(FT_Face ftf) const
{
  for (auto i = 0; i < ftf->num_charmaps; i++) {
    auto& currentCharMap = ftf->charmaps[i];
    if (
      (
        currentCharMap->platform_id == 0
        && 
        currentCharMap->encoding_id == 3
      )
      || 
      (
        currentCharMap->platform_id == 3
        && 
        currentCharMap->encoding_id == 1
      )
    ) {
      return FT_Set_Charmap(ftf, ftf->charmaps[i]);
    }
  }
  return -1;
}
