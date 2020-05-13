#ifndef FontMemoryPool_Class
#define FontMemoryPool_Class

#include <vector>
#include <stack>
#include <stack>
#include <string>
//We can use this mechanism to have memory pool to avoid being forced to create these classes
class FontMemoryPool {
public:
  
  float currentPositionX = 0.0f;
  
  float currentPositionY = 0.0f;
  
  float maximumGlyphWidth = 0.0f;
  
  float maximumGlyphHeight = 0.0f;
  
  std::vector<std::u32string> words;
  
  std::stack<std::u32string> rtlWordBuffer;
  
  bool isRtl = false;
  
  bool isPreviousRtl = false;
  
  bool isPreviousLtr = false;

  bool isPreviouseCharacterABreakerChar = false;

};

#endif // !FontMemoryPool_Class
