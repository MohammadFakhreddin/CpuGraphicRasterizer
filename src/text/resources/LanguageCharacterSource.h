#ifndef LanguageCharacterSource_Class
#define LanguageCharacterSource_Class

#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ot.h>
#include <harfbuzz/hb-ft.h>

#include "../../utils/log/Logger.h"

class LanguageCharacterSource {

public:

  enum class Direction {
    ltr,
    rtl
  };

  virtual const std::vector<std::u32string>& getUnicodes() const {
    Logger::exception("LanguageCharacterSource::getUnicode is not implemented in child class");
    return  unicodes;
  };

  virtual const std::vector<char32_t>& getConnectionBreakers() const {
    Logger::exception("LanguageCharacterSource::getConnectionBreakers is not implemented in child class");
    return connectionBreakers;
  };

  virtual const std::vector<char32_t>& getNumbers() const {
    Logger::exception("LanguageCharacterSource::getPersianNumbers is not implemented in child class");
    return numbers;
  };

  virtual const LanguageCharacterSource::Direction& getDirection() const {
    Logger::exception("LanguageCharacterSource::getDirection is not implemented in child class");
    return direction;
  }

  virtual const hb_language_t& getLanguage() const {
    Logger::exception("LanguageCharacterSource::getLanguage is not implemented in child class");
    return language;
  }

  virtual const hb_script_t& getScript() const {
    Logger::exception("LanguageCharacterSource::getScript is not implemented in child class");
    return script;
  }

private:

  std::vector<std::u32string> unicodes;

  std::vector<char32_t> connectionBreakers;

  std::vector<char32_t> numbers;

  LanguageCharacterSource::Direction direction;

  hb_language_t language = hb_language_from_string("en",2);

  hb_script_t script = HB_SCRIPT_LATIN;

};

#endif // !
