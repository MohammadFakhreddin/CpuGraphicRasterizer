#ifndef Font_Class
#define Font_Class

#include <string>
#include <memory>
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ot.h>
#include <harfbuzz/hb-ft.h>
#include <unordered_map>

#include "../freetype/FreeType.h"
#include "../char_glyph/CharGlyph.h"
#include "../resources/FaCharacterSource.h"
#include "../../pipeline/Pipeline.h"

class Font {
public:

  enum class PositionMode {
    leftTop
  };

  Font(
    const std::string& fontAddress,
    const int& fontSize,
    const FreeType* freeType,
    const hb_language_t& language,
    const hb_script_t& script,
    const hb_direction_t& direction,
    const LanguageCharacterSource* charactersSource
  );

  ~Font();

  void drawText(
    PipeLine& pip,
    const std::u32string& text, 
    const int& positionX, 
    const int& positionY, 
    const Font::PositionMode& mode,
    ColorTexture* colorTexture
  );

  void Font::updateTextNodes(Shape3d* shape);

  void Font::updateTextSurfaces(PipeLine& pip, Shape3d* shape);

private:

  const FreeType* freeType;

  const int size;

  const float margin;

  const float space;

  const hb_language_t& language;

  const hb_script_t& script;

  const hb_direction_t& direction;

  FT_Face* face;

  hb_font_t* font;

  hb_buffer_t* buffer;

  std::unordered_map<char32_t, std::unique_ptr<CharGlyph>> singleCharsMap;
  
  std::unordered_map<char32_t, std::unique_ptr<CharGlyph>> startCharsMap;

  std::unordered_map<char32_t, std::unique_ptr<CharGlyph>> middleCharsMap;

  std::unordered_map<char32_t, std::unique_ptr<CharGlyph>> endCharsMap;

  std::unordered_map<char32_t, bool> connectionBreakers;

  std::unordered_map<char32_t, std::unique_ptr<CharGlyph>> numbersCharMap;
  
  CharGlyph* generateCharacter(
    const char32_t& characterSymbol
  );

  void colorFilterMethod(float* red, float* green, float* blue);

  std::function<void(float* red, float* green, float* blue)> colorFilterRefrence = std::bind(
    &Font::colorFilterMethod,
    this,
    std::placeholders::_1,
    std::placeholders::_2,
    std::placeholders::_3
  );

  ColorTexture* currentColorTexture = nullptr;

};

#endif // !Font_Class
