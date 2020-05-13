#ifndef FaCharacterSource_Class
#define FaCharacterSource_Class

#include <vector>

#include "./LanguageCharacterSource.h"

class FaCharacterSource : public LanguageCharacterSource {

private:

  std::vector<std::u32string> persianUnicodes = std::vector<std::u32string>({
      U"آ ﺁ ﺂ ﺂ",
      U"ﺃ ﺃ ﺄ ﺄ",
      U"ﺇ ﺇ ﺈ ﺈ",
      U"ا ﺍ ﺎ ﺎ",
      U"ب ﺑ ﺒ ﺐ",
      U"ت ﺗ ﺘ ﺖ",
      U"ث ﺛ ﺜ ﺚ",
      U"ج ﺟ ﺠ ﺞ",
      U"ح ﺣ ﺤ ﺢ",
      U"خ ﺧ ﺨ ﺦ",
      U"د ﺩ ﺪ ﺪ",
      U"ﺫ ﺫ ﺬ ﺬ",
      U"ر ﺭ ﺮ ﺮ",
      U"ز ﺯ ﺰ ﺰ",
      U"س ﺳ ﺴ ﺲ",
      U"ش ﺷ ﺸ ﺶ",
      U"ص ﺻ ﺼ ﺺ",
      U"ض ﺿ ﻀ ﺾ",
      U"ط ﻃ ﻄ ﻂ",
      U"ظ ﻇ ﻈ ﻆ",
      U"ع ﻋ ﻌ ﻊ",
      U"غ ﻏ ﻐ ﻎ",
      U"ف ﻓ ﻔ ﻒ",
      U"ق ﻗ ﻘ ﻖ",
      U"ک ﻛ ﻜ ﻚ",
      U"ل ﻟ ﻠ ﻞ",
      U"م ﻣ ﻤ ﻢ",
      U"ن ﻧ ﻨ ﻦ",
      U"ه ﻫ ﻬ ﻪ",
      U"و ﻭ ﻮ ﻮ",
      U"ث ﭨ ﭩ ﭧ",
      U"ﮦ ﮨ ﮩ ﮧ",
      U"ﮮ ﮰ ﮱ ﮯ",
      U"ی ﯾ ﯿ ﯽ",
      U"ﮈ ﮈ ﮉ ﮉ",
      U"ژ ﮌ ﮍ ﮍ",
      U"ﯗ ﯗ ﯘ ﯘ",
      U"ﯙ ﯙ ﯚ ﯚ",
      U"ﯛ ﯛ ﯜ ﯜ",
      U"ک ﮐ ﮑ ﮏ",
      U"ﻵ ﻵ ﻶ ﻶ",
      U"ﻷ ﻷ ﻸ ﻸ",
      U"ﻹ ﻹ ﻺ ﻺ",
      U"ﻻ ﻻ ﻼ ﻼ",
      U"پ ﭘ ﭙ ﭗ",
      U"چ ﭼ ﭽ ﭻ",
      U"ژ ﮊ ﮋ ﮋ",
      U"ﭪ ﭬ ﭭ ﭫ",
      U"گ ﮔ ﮕ ﮓ "
  });

  std::vector<char32_t> connectionBreakers = std::vector<char32_t>({
      U'آ',
      U'ﺁ',
      U'ﺂ',
      U'ﺂ',
      U'ﺃ',
      U'ﺃ',
      U'ﺄ',
      U'ﺄ',
      U'ﺇ',
      U'ﺇ',
      U'ﺈ',
      U'ﺈ',
      U'ا',
      U'ﺍ',
      U'ﺎ',
      U'ﺎ',
      U'د',
      U'و',
      U'ر',
      U'ز'
  });

  std::vector<char32_t> persianNumbers = std::vector<char32_t>({
      U'۰',
      U'۱',
      U'۲',
      U'۳',
      U'۴',
      U'۵',
      U'۶',
      U'۷',
      U'۸',
      U'۹'
  });

  hb_language_t language = hb_language_from_string("fa", 2);

  hb_script_t script = HB_SCRIPT_ARABIC;

public:

  const std::vector<std::u32string>& getUnicodes() const override {
    return persianUnicodes;
  };

  const std::vector<char32_t>& getConnectionBreakers() const override {
    return connectionBreakers;
  };

  const std::vector<char32_t>& getNumbers() const override {
    return persianNumbers;
  };

  const hb_language_t& getLanguage() const override {
    return language;
  }

  const hb_script_t& getScript() const override {
    return script;
  }

};

#endif // !FaCharacterSource_Class