#ifndef LanguageCharacterSource_Class
#define LanguageCharacterSource_Class

#include "../../utils/log/Logger.h"

class LanguageCharacterSource {
public:

  virtual const std::vector<std::u32string>& getUnicodes() const {
    Logger::exception("LanguageCharacterSource::getUnicode is not implemented in parent class");
    return  unicodes;
  };

  virtual const std::vector<char32_t>& getConnectionBreakers() const {
    Logger::exception("LanguageCharacterSource::getConnectionBreakers is not implemented in parent class");
    return connectionBreakers;
  };

  virtual const std::vector<char32_t>& getNumbers() const {
    Logger::exception("LanguageCharacterSource::getPersianNumbers is not implemented in parent class");
    return numbers;
  };

private:

  std::vector<std::u32string> unicodes;

  std::vector<char32_t> connectionBreakers;

  std::vector<char32_t> numbers;

};

#endif // !
