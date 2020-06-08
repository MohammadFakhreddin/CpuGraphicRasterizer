#ifndef Font_Class
#define Font_Class

#include "../../open_gl/OpenGl.h"
#ifdef  __OPENGL__
#include <codecvt>
#include <memory>
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ot.h>
#include <harfbuzz/hb-ft.h>
#include <unordered_map>

#include "../freetype/FreeType.h"
#include "../char_glyph/CharGlyph.h"
#include "../resources/FaCharacterSource.h"
#include "../../pipeline/Pipeline.h"
#include "./FontMemoryPool.h"

class Font {

private:

  static constexpr char32_t asciiStart = 0;
  static constexpr char32_t asciiEnd = 256;
  static constexpr char32_t punctuationStart = 8192;
  static constexpr char32_t punctuationEnd = 8303;

public:

  enum class PositionMode {
    leftTop
  };

  Font(
    const std::string& fontAddress,
    const int& fontSize,
    const FreeType* freeType,
    const std::vector<LanguageCharacterSource*> specialCharactersSource
  );

  ~Font();

  void drawText(
    PipeLine& pip,
    const std::string& text,
    const int& positionX,
    const int& positionY,
    const Font::PositionMode& mode,
    ColorTexture* colorTexture
  );

  void drawText(
    PipeLine& pip,
    const std::u32string& text, 
    const int& positionX, 
    const int& positionY, 
    const Font::PositionMode& mode,
    ColorTexture* colorTexture
  );

  void updateTextNodes(Shape3d* shape);

  void updateTextSurfaces(PipeLine& pip, Shape3d* shape);

private:

  bool isPunctOrSpace(const char32_t& character);

  bool isPunct(const char32_t& character);

  bool isSpace(const char32_t& character);

  void renderAndEmptyRTLWordBuffer(PipeLine& pip);

  void renderWord(PipeLine& pip,const std::u32string& word);

  void renderGlyph(PipeLine& pip, std::unique_ptr<CharGlyph>& glyph);

  void isRtl(const std::u32string& word, bool* result);

  void renderSpace();

  const FreeType* freeType;

  const int size;

  const float margin;

  const float space;
#ifdef __OPENGL__
  FT_Face* face;

  hb_font_t* font;

  hb_buffer_t* buffer;
#endif

  std::unordered_map<char32_t, LanguageCharacterSource::Direction> unicodeSingleCharDirection;

  std::unordered_map<char32_t, std::unique_ptr<CharGlyph>> unicodeSingleCharsMap;
  
  std::unordered_map<char32_t, std::unique_ptr<CharGlyph>> unicodeStartCharsMap;

  std::unordered_map<char32_t, std::unique_ptr<CharGlyph>> unicodeMiddleCharsMap;

  std::unordered_map<char32_t, std::unique_ptr<CharGlyph>> unicodeEndCharsMap;

  std::unordered_map<char32_t, bool> unicodeConnectionBreakers;

  CharGlyph* generateCharacter(
    const char32_t& characterSymbol,
    const hb_language_t& language,
    const hb_script_t& script,
    const hb_direction_t& direction
  );

  void colorFilterMethod(float* red, float* green, float* blue);

  std::function<void(float* red, float* green, float* blue)> colorFilterReference = std::bind(
    &Font::colorFilterMethod,
    this,
    std::placeholders::_1,
    std::placeholders::_2,
    std::placeholders::_3
  );

  void findSmallestRectForGlyphTexture(
    ImageTexture* texture,
    float* textureStartX,
    float* textureStartY,
    float* textureEndX,
    float* textureEndY
  ) const;

  ColorTexture* currentColorTexture = nullptr;

  FontMemoryPool mp;

};
#endif
#endif // !Font_Class
