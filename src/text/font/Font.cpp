#include "./Font.h"

#include <codecvt>

#include "../../texture/ImageTexture/ImageTexture.h"
#include "../../3d/surface/Surface.h"
#include "../../utils/operators/Operators.h"
#include "../../pipeline/Pipeline.h"

//This class is written based on harfbuzz library example
Font::Font(const std::string& fontAddress,
  const int& fontSize,
  const FreeType* freeType,
  const hb_language_t& language,
  const hb_script_t& script,
  const hb_direction_t& direction,
  const LanguageCharacterSource* charactersSource
)
  :
  size(fontSize),
  margin(fontSize * 0.5f),
  space(fontSize * 0.25f),
  freeType(freeType),
  //Plus 2 for better accuracy
  face(freeType->loadFace(fontAddress, size * 64, 72, 72)),
  font(hb_ft_font_create(*face,nullptr)),
  buffer(hb_buffer_create()),
  language(language),
  script(script),
  direction(direction),
  singleCharsMap(),
  startCharsMap(),
  middleCharsMap(),
  endCharsMap()
{
  hb_buffer_allocation_successful(buffer);

  if (charactersSource->getUnicodes().empty() == false) {
    for (const auto& unicodeChars : charactersSource->getUnicodes()) {
      std::vector<std::u32string> rawChars = Operators::split(Operators::trim(unicodeChars), U' ');
      assert(rawChars.size() == 4);
      {
        assert(rawChars[0].size() == 1);
        CharGlyph* charGlyph = generateCharacter(rawChars[0][0]);
        assert(charGlyph != nullptr);
        singleCharsMap.insert({ rawChars[0][0],std::unique_ptr<CharGlyph>(charGlyph) });
      }
      {
        assert(rawChars[1].size() == 1);
        CharGlyph* charGlyph = generateCharacter(rawChars[1][0]);
        assert(charGlyph != nullptr);
        startCharsMap.insert({ rawChars[0][0],std::unique_ptr<CharGlyph>(charGlyph) });
      }
      {
        assert(rawChars[2].size() == 1);
        CharGlyph* charGlyph = generateCharacter(rawChars[2][0]);
        assert(charGlyph != nullptr);
        middleCharsMap.insert({ rawChars[0][0],std::unique_ptr<CharGlyph>(charGlyph) });
      }
      {
        assert(rawChars[3].size() == 1);
        CharGlyph* charGlyph = generateCharacter(rawChars[3][0]);
        assert(charGlyph != nullptr);
        endCharsMap.insert({ rawChars[0][0],std::unique_ptr<CharGlyph>(charGlyph) });
      }
    }
  }

  if (charactersSource->getConnectionBreakers().empty() == false) {
    for (const auto& connectionBreakder : charactersSource->getConnectionBreakers()) {
      connectionBreakers[connectionBreakder] = true;
    }
  }

  if (charactersSource->getNumbers().empty() == false) {
    for (const auto& number : charactersSource->getNumbers()) {
      CharGlyph* charGlyph = generateCharacter(number);
      assert(charGlyph != nullptr);
      numbersCharMap[number] = std::unique_ptr<CharGlyph>(charGlyph);
    }
  }

}

Font::~Font() {
  freeType->freeFace(face);
  hb_buffer_destroy(buffer);
  hb_font_destroy(font);
}

void Font::drawText(
  PipeLine& pip,
  const std::u32string& text,
  const int& positionX,
  const int& positionY,
  const Font::PositionMode& mode,
  ColorTexture* colorTexture
) {
  assert(colorTexture != nullptr);
  this->currentColorTexture = colorTexture;
  //TODO We need scale too
  //TODO We need size too
  float currentPositionX = float(positionX);
  float currentPositionY = float(positionY);
  if (text.size() > 0) {
    //std::vector<std::u32string> words;
    //bool isPersian = false;
    //bool isPreviousPersian = false;
    //bool isPreviousEnglish = false;
    //for (auto character : text) {
    //  //TODO Handle punctuations
    //  /*if (
    //    character._char == "\n" ||
    //    (
    //      character.isASCII() == true &&
    //      std::ispunct(character._char[0])
    //      )
    //    ) {
    //    auto newWord = std::vector<cocos2d::StringUtils::StringUTF8::CharUTF8>();
    //    newWord.emplace_back(character);
    //    words.emplace_back(newWord);
    //    isPreviousPersian = false;
    //    isPreviousEnglish = false;
    //  }
    //  else {*/
    //  isPersian = singleCharsMap[character] != nullptr;
    //  if (isPersian) {
    //    isPreviousEnglish = false;
    //    if (isPreviousPersian == true) {
    //      words.at(words.size() - 1)+=(character);
    //    }
    //    else {
    //      isPreviousPersian = true;
    //      std::u32string newWord = U"";
    //      newWord += character;
    //      words.emplace_back(newWord);
    //    }
    //  }
    //  else {
    //    isPreviousPersian = false;
    //    if (isPreviousEnglish == true) {
    //      words.at(words.size() - 1) += character;
    //    }
    //    else {
    //      isPreviousEnglish = true;
    //      auto newWord = U"";
    //      newWord += character;
    //      words.emplace_back(newWord);
    //    }
    //  }
    //  //}
    //}
    for (auto& character : text) {
      auto& glyph = singleCharsMap[character];
      if (glyph == nullptr) {
        currentPositionX += space;
        continue;
      }
      assert(glyph.get());
      assert(glyph.get()->getShape());
      auto& shape = glyph.get()->getShape();
      shape->resetTransform();
      shape->transformXYZ(currentPositionX, currentPositionY, 1.0f);
      currentPositionX += glyph->getWidth(); 
      updateTextNodes(shape.get());
      updateTextSurfaces(pip, shape.get());
    }
  }
}

void Font::updateTextNodes(Shape3d* shape) {
  for (
    unsigned int nodeIndex = 0;
    nodeIndex < shape->nodes.size();
    nodeIndex ++
    ) {
    //TODO Move this part to new function and merge all other update methods with this
    shape->worldPoints[nodeIndex].assign(shape->nodes[nodeIndex]);

    shape->worldPoints[nodeIndex].multiply(shape->rotationXYZMatrix);

    shape->worldPoints[nodeIndex].multiply(shape->scaleMatrix);

    shape->worldPoints[nodeIndex].multiply(shape->transformMatrix);

  }
}

void Font::updateTextSurfaces(PipeLine& pip,Shape3d* shape) {
  for (
    unsigned int surfaceIndex = 0;
    surfaceIndex < shape->surfaces.size();
    surfaceIndex ++
    ) {
    pip.updateSurface(shape, shape->surfaces[surfaceIndex].get());
  }
}

//We need to create all words using this method once and then combine them separately
CharGlyph* Font::generateCharacter(
  const char32_t& characterSymbol
)
{
  hb_buffer_reset(buffer);

  hb_buffer_set_direction(buffer, direction);
  
  hb_buffer_set_script(buffer, script);
  
  hb_buffer_set_language(buffer, language);

  //TO convert between utf32 characters and string
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
  
  std::string characterString = converter.to_bytes(characterSymbol);

  int length = int(characterString.size());
 
  hb_buffer_add_utf8(buffer, characterString.c_str(), length, 0, length);

  // harfbuzz shaping
  hb_shape(font, buffer, nullptr, 0);

  unsigned int glyphCount;

  hb_glyph_info_t* glyphInfo = hb_buffer_get_glyph_infos(buffer, &glyphCount);

  assert(glyphCount == 1);
  
  hb_glyph_position_t* glyphPos = hb_buffer_get_glyph_positions(buffer, &glyphCount);
  
  assert(glyphCount == 1);

  std::vector<Matrix3X1Float> vertices;

  std::vector<std::unique_ptr<Surface>> indices; 

  unsigned int vertexStartIndex;

  FreeType::Glyph* glyph = freeType->rasterize(face, glyphInfo[0].codepoint);
  
  int twidth = int(pow(2.0, ceil(log(glyph->width) / log(2))));
    
  int theight = int(pow(2.0, ceil(log(glyph->height) / log(2))));

  unsigned char * tdata = new unsigned char[twidth * theight]();

  for (unsigned int iy = 0; iy < glyph->height; ++iy) {
    //TODO I think 1 memcpy is enough
    memcpy(tdata + iy * twidth, glyph->buffer + iy * glyph->width, glyph->width);
    
  }

  float s0 = 1.0f;
    
  float t0 = 1.0f;
    
  float s1 = (twidth / 3.0f) - 1.0f;
    
  float t1 = (theight / 3.0f) - 1.0f;

  float x = 0;

  float y = 0;
    
  float xa = (float)glyphPos[0].x_advance / 64.0f;
    
  float ya = (float)glyphPos[0].y_advance / 64.0f;
    
  float xo = (float)glyphPos[0].x_offset / 64.0f;
    
  float yo = (float)glyphPos[0].y_offset / 64.0f;
    
  float x0 = x + xo + glyph->bearingX;
    
  float y0 = floor(y + yo + glyph->bearingY);
    
  float x1 = x0 + glyph->width;
    
  float y1 = floor(y0 - glyph->height);

  vertexStartIndex = int(vertices.size());

  float scaleFactor = 1.0f;

  vertices.emplace_back(x0, y0, 0.0f);

  vertices.emplace_back(x0, y1 / scaleFactor, 0.0f);

  vertices.emplace_back(x1 / scaleFactor, y1 / scaleFactor, 0.0f);

  vertices.emplace_back(x1 / scaleFactor, y0, 0.0f);

  ImageTexture* texture = new ImageTexture(
    tdata, 
    4, 
    twidth, 
    theight, 
    float(twidth), 
    float(theight)
  );
 
  texture->assignFilter(&colorFilterRefrence);

  indices.emplace_back(std::make_unique<Surface>(
    Constants::LightPrecision::none,
    texture,
    vertexStartIndex + 0,
    vertexStartIndex + 1,
    vertexStartIndex + 2
  ));

  indices.back()->setNormalIndex(0, 0);
  indices.back()->setNormalIndex(1, 0);
  indices.back()->setNormalIndex(2, 0);

  indices.back()->setTextureCoordinates(0, s0, t0);
  indices.back()->setTextureCoordinates(1, s0, t1);
  indices.back()->setTextureCoordinates(2, s1, t1);

  indices.emplace_back(std::make_unique<Surface>(
    Constants::LightPrecision::none,
    texture,
    vertexStartIndex + 0,
    vertexStartIndex + 2,
    vertexStartIndex + 3
  ));

  indices.back()->setNormalIndex(0, 0);
  indices.back()->setNormalIndex(1, 0);
  indices.back()->setNormalIndex(2, 0);

  indices.back()->setTextureCoordinates(0, s0, t0);
  indices.back()->setTextureCoordinates(1, s1, t1);
  indices.back()->setTextureCoordinates(2, s1, t0);
  
  x += xa;

  y += ya;

  freeType->freeGlyph(glyph);

  std::vector<Matrix3X1Float> normals;

  normals.emplace_back(0.0f, 0.0f, -1.0f);

  Shape3d* shape = new Shape3d(vertices, indices, normals);
  
  return new CharGlyph(
    shape,
    texture,
    x1,
    y1
  );

}

void Font::colorFilterMethod(float* red, float* green, float* blue) {
  if (currentColorTexture != nullptr) {
 
    float averageColor = (*red + *green + *blue) / 3.0f;

    *red = averageColor;
    *green = averageColor;
    *blue = averageColor;

    *red *= currentColorTexture->getRed();
    *green *= currentColorTexture->getGreen();
    *blue *= currentColorTexture->getBlue();

  }
}