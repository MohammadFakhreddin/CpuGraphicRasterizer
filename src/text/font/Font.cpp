#include "./Font.h"

#include <codecvt>

#include "../../3d/surface/Surface.h"
#include "../../utils/operators/Operators.h"
#include "../../pipeline/Pipeline.h"

//This class is written based on harfbuzz library example
Font::Font(
  const std::string& fontAddress,
  const int& fontSize,
  const FreeType* freeType,
  const std::vector<LanguageCharacterSource*> specialCharactersSource
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
  unicodeSingleCharsMap(),
  unicodeStartCharsMap(),
  unicodeMiddleCharsMap(),
  unicodeEndCharsMap()
{
  hb_buffer_allocation_successful(buffer);
  {
    //TODO We need a fallback font that supports all unicode characters in case that current font does not support that font
    auto languageCode = hb_language_from_string("en", 2);
    auto direction = HB_DIRECTION_LTR;
    auto script = HB_SCRIPT_LATIN;
    //Ascii chars
    for (char32_t asciiCharacter = asciiStart; asciiCharacter < asciiEnd; asciiCharacter++) {
      CharGlyph* charGlyph = generateCharacter(asciiCharacter, languageCode, script, direction);
      if (charGlyph == nullptr) {
        Logger::log(U"Creating glyph for following chracter failed:" + asciiCharacter);
      }
      else {
        unicodeSingleCharsMap.insert({ asciiCharacter,std::unique_ptr<CharGlyph>(charGlyph) });
        unicodeSingleCharDirection.insert({ asciiCharacter,LanguageCharacterSource::Direction::ltr });
      }
    }
    //Punctuations
    for (char32_t puncCharacter = punchuationStart; puncCharacter < punchuationEnd; puncCharacter++) {
      CharGlyph* charGlyph = generateCharacter(puncCharacter, languageCode, script, direction);
      if (charGlyph == nullptr) {
        Logger::log(U"Creating glyph for following chracter failed:" + puncCharacter);
      }
      else {
        unicodeSingleCharsMap.insert({ puncCharacter,std::unique_ptr<CharGlyph>(charGlyph) });
        unicodeSingleCharDirection.insert({ puncCharacter,LanguageCharacterSource::Direction::ltr });
      }
    }
  }
  //Special unicode chars
  if (specialCharactersSource.empty() == false) {
    for (const auto& charactersSource : specialCharactersSource) {
      auto languageCode = charactersSource->getLanguage();
      auto direction = charactersSource->getDirection() == LanguageCharacterSource::Direction::ltr 
        ? HB_DIRECTION_LTR 
        : HB_DIRECTION_RTL;
      auto script = charactersSource->getScript();
      if (charactersSource->getUnicodes().empty() == false) {
        for (const auto& unicodeChars : charactersSource->getUnicodes()) {
          std::vector<std::u32string> rawChars = Operators::split(Operators::trim(unicodeChars), U' ');
          assert(rawChars.size() == 4);
          {
            assert(rawChars[0].size() == 1);
            CharGlyph* charGlyph = generateCharacter(rawChars[0][0], languageCode, script, direction);
            if (charGlyph == nullptr) {
              Logger::log(U"Creating glyph for following chracter failed:" + rawChars[0][0]);
            }
            else {
              unicodeSingleCharsMap.insert({ rawChars[0][0],std::unique_ptr<CharGlyph>(charGlyph) });
              unicodeSingleCharDirection.insert({ rawChars[0][0],charactersSource->getDirection() });
            }
          }
          {
            assert(rawChars[1].size() == 1);
            CharGlyph* charGlyph = generateCharacter(rawChars[1][0], languageCode, script, direction);
            if (charGlyph == nullptr) {
              Logger::log(U"Creating glyph for following chracter failed:" + rawChars[1][0]);
            }
            else {
              unicodeStartCharsMap.insert({ rawChars[0][0],std::unique_ptr<CharGlyph>(charGlyph) });
            }
          }
          {
            assert(rawChars[2].size() == 1);
            CharGlyph* charGlyph = generateCharacter(rawChars[2][0], languageCode, script, direction);
            if (charGlyph == nullptr) {
              Logger::log(U"Creating glyph for following chracter failed:" + rawChars[2][0]);
            }
            else {
              unicodeMiddleCharsMap.insert({ rawChars[0][0],std::unique_ptr<CharGlyph>(charGlyph) });
            }
          }
          {
            assert(rawChars[3].size() == 1);
            CharGlyph* charGlyph = generateCharacter(rawChars[3][0], languageCode, script, direction);
            if (charGlyph == nullptr) {
              Logger::log(U"Creating glyph for following chracter failed:" + rawChars[3][0]);
            }
            else {
              unicodeEndCharsMap.insert({ rawChars[0][0],std::unique_ptr<CharGlyph>(charGlyph) });
            }
          }
        }
      }
      if (charactersSource->getConnectionBreakers().empty() == false) {
        for (const auto& connectionBreakder : charactersSource->getConnectionBreakers()) {
          unicodeConnectionBreakers[connectionBreakder] = true;
        }
      }
      if (charactersSource->getNumbers().empty() == false) {
        for (const auto& number : charactersSource->getNumbers()) {
          CharGlyph* charGlyph = generateCharacter(number, languageCode, script, direction);
          assert(charGlyph != nullptr);
          unicodeStartCharsMap.insert({ number,std::unique_ptr<CharGlyph>(charGlyph) });
        }
      }
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
  if (text.size() <= 0) {
    return;
  }
  mp.currentPositionX = 0.0f;
  mp.currentPositionY = 0.0f;
  mp.maximumGlyphWidth = 0.0f;
  mp.maximumGlyphHeight = 0.0f;
  mp.words.clear();
  assert(mp.words.size() == 0);
  assert(mp.rtlWordBuffer.size() == 0);
  assert(colorTexture != nullptr);
  mp.isPreviousRtl = false;
  mp.isPreviousLtr = false;
  mp.isPreviouseCharacterABreakerChar = false;
  this->currentColorTexture = colorTexture;
  for (auto character : text) {
    //TODO Handle punctuations
    if (isPunctOrSpace(character)) {
      auto newWord = U"";
      newWord += character;
      mp.words.emplace_back(newWord);
      mp.isPreviousRtl = false;
      mp.isPreviousLtr = false;
    }
    else {
      isRtl(text,&mp.isRtl);
      if (mp.isRtl) {
        mp.isPreviousLtr = false;
        if (mp.isPreviousRtl == true) {
          mp.words.at(mp.words.size() - 1) += character;
        }
        else {
          mp.isPreviousRtl = true;
          std::u32string newWord = U"";
          newWord += character;
          mp.words.emplace_back(newWord);
        }
      }
      else {
        mp.isPreviousRtl = false;
        if (mp.isPreviousLtr == true) {
          mp.words.at(mp.words.size() - 1) += character;
        }
        else {
          mp.isPreviousLtr = true;
          auto newWord = U"";
          newWord += character;
          mp.words.emplace_back(newWord);
        }
      }
    }
  }
  assert(mp.words.size() > 0);
  //TODO We need scale too
  //TODO We need size too
  mp.currentPositionX = float(positionX);
  mp.currentPositionY = float(positionY);
  //Print glyphs one by one
  {//Normalizing and connecting words to create normalized text
    for (const auto& word : mp.words) {
      if (word[0] == '\n')
      {
        assert(word.length() == 1);
        renderAndEmptyRTLWordBuffer(pip);
        mp.currentPositionY += mp.maximumGlyphHeight;
        mp.currentPositionX = float(positionX);
      }
      else if (isPunct(word[0])) {
        assert(word.length() == 1);
        if (mp.rtlWordBuffer.empty()) {
          renderSpace();
        }
        else {
          mp.rtlWordBuffer.push(word);
        }
      }
      else if (isSpace(word[0])) {
        assert(word.length() == 1);
        if (mp.rtlWordBuffer.size() == 0) {
          renderWord(pip, word);
        }
        else {
          mp.rtlWordBuffer.push(word);
        }
      }
      else {
        isRtl(word, &mp.isRtl);
        if (mp.isRtl) {
          mp.rtlWordBuffer.push(word);
        }
        else {
          renderAndEmptyRTLWordBuffer(pip);
          renderWord(pip, word);
        }
      }
    }
    renderAndEmptyRTLWordBuffer(pip);
  }
}


void Font::renderAndEmptyRTLWordBuffer(PipeLine& pip)
{
  while (mp.rtlWordBuffer.empty() == false) {
    renderWord(pip, mp.rtlWordBuffer.top());
    mp.rtlWordBuffer.pop();
  }
}

void Font::renderWord(PipeLine& pip, const std::u32string& word)
{
  //TODO To many duplicate computation
  assert(word.length() > 0);
  if (isSpace(word[0])) {
    assert(word.length() == 1);
    renderSpace();
  }
  else if (isPunct(word[0])) {
    assert(word.length() == 1);
    auto& glyph = unicodeSingleCharsMap[word[0]];
    renderGlyph(pip, glyph);
  }
  else {
    isRtl(word, &mp.isRtl);
    int wordLoopStart = mp.isRtl ? word.length() - 1 : 0;
    int wordLoopEnd = mp.isRtl ? -1 : word.length();
    int wordLoopStepValue = mp.isRtl ? -1 : +1;
    for (int i = wordLoopStart; i != wordLoopEnd; i += wordLoopStepValue) {
      assert(i >= 0 && i < word.length());
      mp.isPreviouseCharacterABreakerChar = false;
      const auto& currentCharacter = word[i];
      if (i == 0) {
        auto& existingGlyph = unicodeStartCharsMap[currentCharacter];
        if (existingGlyph != nullptr) {
          renderGlyph(pip, existingGlyph);
        }
        else
        {
          auto& basicGlyph = unicodeSingleCharsMap[currentCharacter];
          if (basicGlyph != nullptr) {
            renderGlyph(pip, basicGlyph);
          }
          else {
            renderSpace();
          }
        }
      }
      else
      {
        mp.isPreviouseCharacterABreakerChar = unicodeConnectionBreakers[word[i - 1]] == true;
        if (mp.isPreviouseCharacterABreakerChar) {
          if (i == word.length() - 1) {
            auto& basicGlyph = unicodeSingleCharsMap[currentCharacter];
            if (basicGlyph != nullptr) {
              renderGlyph(pip, basicGlyph);
            }
            else {
              renderSpace();
            }
          }
          else {
            auto& existingGlyph = unicodeStartCharsMap[currentCharacter];
            if (existingGlyph != nullptr) {
              renderGlyph(pip, existingGlyph);
            }
            else
            {
              auto& basicGlyph = unicodeSingleCharsMap[currentCharacter];
              if (basicGlyph != nullptr) {
                renderGlyph(pip, basicGlyph);
              }
              else {
                renderSpace();
              }
            }
          }
        }
        else
        {
          if (i == word.length() - 1)
          {
            auto& existingGlyph = unicodeEndCharsMap[currentCharacter];
            if (existingGlyph != nullptr) {
              renderGlyph(pip, existingGlyph);
            }
            else
            {
              auto& basicGlyph = unicodeSingleCharsMap[currentCharacter];
              if (basicGlyph != nullptr) {
                renderGlyph(pip, basicGlyph);
              }
              else {
                renderSpace();
              }
            }
          }
          else {
            auto& existingGlyph = unicodeMiddleCharsMap[currentCharacter];
            if (existingGlyph != nullptr) {
              renderGlyph(pip, existingGlyph);
            }
            else
            {
              auto& basicGlyph = unicodeSingleCharsMap[currentCharacter];
              if (basicGlyph != nullptr) {
                renderGlyph(pip, basicGlyph);
              }
              else {
                renderSpace();
              }
            }
          }
        }
      }
    }
  }
}

void Font::renderGlyph(PipeLine& pip,std::unique_ptr<CharGlyph>& glyph) {
  auto& shape = glyph.get()->getShape();
  shape->resetTransform();
  shape->transformXYZ(mp.currentPositionX, mp.currentPositionY, 1.0f);
  mp.currentPositionX += glyph->getWidth() * 0.9f;
  updateTextNodes(shape.get());
  updateTextSurfaces(pip, shape.get());
}

void Font::renderSpace() {
  mp.currentPositionX += space;
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

bool Font::isPunctOrSpace(const char32_t& character)
{
  if (character >= 256) {
    return false;
  }
  return std::ispunct(character) ||
    std::isspace(character);
}

bool Font::isPunct(const char32_t& character)
{
  if (character >= 256) {
    return false;
  }
  return std::ispunct(character);
}

bool Font::isSpace(const char32_t& character)
{
  if (character >= 256) {
    return false;
  }
  return std::isspace(character);
}

void Font::isRtl(const std::u32string& word, bool* result)
{
  assert(word.length() >= 1);
  *result = unicodeSingleCharDirection[word[0]] == LanguageCharacterSource::Direction::rtl;
}

//We need to create all words using this method once and then combine them separately
CharGlyph* Font::generateCharacter(
  const char32_t& characterSymbol,
  const hb_language_t& language,
  const hb_script_t& script,
  const hb_direction_t& direction
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

  if (glyph->width == 0 || glyph->height == 0) {
    return nullptr;
  }
  
  int twidth = int(pow(2.0, ceil(log(glyph->width) / log(2))));
    
  int theight = int(pow(2.0, ceil(log(glyph->height) / log(2))));

  unsigned char * tdata = new unsigned char[twidth * theight]();

  for (unsigned int iy = 0; iy < glyph->height; ++iy) {
    //TODO I think 1 memcpy is enough
    memcpy(tdata + iy * twidth, glyph->buffer + iy * glyph->width, glyph->width);
    
  }

  float textureMargin = 1.0f;

  float textureStartX = textureMargin + (twidth / 3.0f);
    
  float textureStartY = textureMargin;
 
  float textureEndX = (twidth * 2.0f)/3.0f - textureMargin;

  float textureEndY = (theight/3.0f) - textureMargin;

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

  findSmallestRectForGlyphTexture(texture, &textureStartX, &textureStartY, &textureEndX, &textureEndY);

  indices.back()->setTextureCoordinates(0, textureStartX, textureStartY);
  indices.back()->setTextureCoordinates(1, textureStartX, textureEndY);
  indices.back()->setTextureCoordinates(2, textureEndX, textureEndY);

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

  indices.back()->setTextureCoordinates(0, textureStartX, textureStartY);
  indices.back()->setTextureCoordinates(1, textureEndX, textureEndY);
  indices.back()->setTextureCoordinates(2, textureEndX, textureStartY);
  
  x += xa;

  y += ya;

  freeType->freeGlyph(glyph);

  std::vector<Matrix3X1Float> normals;

  normals.emplace_back(0.0f, 0.0f, -1.0f);

  Shape3d* shape = new Shape3d(vertices, indices, normals);

  shape->setTransparencyColorStatus(true);

  shape->setTransparencyColor(0.0f, 0.0f, 0.0f);
  
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

void Font::findSmallestRectForGlyphTexture(
  ImageTexture* texture, 
  float* textureStartX, 
  float* textureStartY, 
  float* textureEndX, 
  float* textureEndY
) const
{

  int i = 0;
  bool targetIndexCanBeMinimized = true;
  float red;
  float green;
  float blue;
  
  const auto doesColorReachesThreshold = [&red, &green, &blue]() {
    return red >= 0.4f || green >= 0.4f || blue >= 0.4f;
  };

  do {
    for (i = int(*textureStartX); i < *textureEndX; i++) {
      assert(*textureStartY < texture->getVirtualImageHeight());
      texture->getPixelForPosition(i, *textureStartY, &red, &green, &blue);
      if (doesColorReachesThreshold()) {
        targetIndexCanBeMinimized = false;
        break;
      }
    }
    if (targetIndexCanBeMinimized == true) {
      *textureStartY += 1.0f;
    }
  } while (targetIndexCanBeMinimized && *textureStartY < texture->getVirtualImageHeight());

  targetIndexCanBeMinimized = true;
  do {
    for (i = int(*textureStartX); i < *textureEndX; i++) {
      assert(*textureEndY >= 0);
      texture->getPixelForPosition(i, *textureEndY, &red, &green, &blue);
      if (doesColorReachesThreshold()) {
        targetIndexCanBeMinimized = false;
        break;
      }
    }
    if (targetIndexCanBeMinimized == true) {
      *textureEndY -= 1.0f;
    }
  } while (targetIndexCanBeMinimized && *textureEndY >= 0);

  targetIndexCanBeMinimized = true;
  do {
    for (i = int(*textureStartY); i < *textureEndY; i++) {
      assert(*textureStartX < texture->getVirtualImageWidth());
      texture->getPixelForPosition(*textureStartX, i, &red, &green, &blue);
      if (doesColorReachesThreshold()) {
        targetIndexCanBeMinimized = false;
        break;
      }
    }
    if (targetIndexCanBeMinimized == true) {
      *textureStartX += 1.0f;
    }
  } while (targetIndexCanBeMinimized && *textureStartX < texture->getVirtualImageWidth());

  targetIndexCanBeMinimized = true;
  do {
    for (i = int(*textureStartY); i < *textureEndY; i++) {
      assert(*textureEndX >= 0);
      texture->getPixelForPosition(*textureEndX, i, &red, &green, &blue);
      if (doesColorReachesThreshold()) {
        targetIndexCanBeMinimized = false;
        break;
      }
    }
    if (targetIndexCanBeMinimized == true) {
      *textureEndX -= 1.0f;
    }
  } while (targetIndexCanBeMinimized && *textureEndX >= 0);

}
