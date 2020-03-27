#ifndef Constants_class
#define Constants_class
//
// Created by mohammad.fakhreddin on 1/3/20.
//
#include <math.h>
#include <unordered_map>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    //define something for Windows (32-bit and 64-bit, this part is common)
#define __PLATFORM_WIN__
#define __DESKTOP__

#ifdef _WIN64
   //define something for Windows (64-bit only)
#else
   //define something for Windows (32-bit only)
#endif

#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
    // iOS Simulator
    // iOS device
#define __MOBILE__
#ifndef __IOS__
#define __IOS__
#endif
#elif TARGET_OS_MAC
    // Other kinds of Mac OS
#define __DESKTOP__
#define __PLATFORM_MAC__
#else
#   error "Unknown Apple platform"
#endif
#elif __ANDROID__
#define __MOBILE__
#elif __linux__
    // linux
#define __PLATFORM_LINUX__
#if !defined(__DESKTOP__)
#define __DESKTOP__
#endif
#elif __unix__ // all unices not caught above
    // Unix
#define __PLATFORM_UNIX__
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#   error "Unknown compiler"
#endif

class Constants {

public:

  class Window {
  
  public:
	
    static constexpr char appName[] = "3D CUBE";
	
  };
  
  enum Platform {
    Windows,
    Mac,
    Iphone,
    Android
  };

#ifdef __DESKTOP__
  
  enum Buttons
  {
    keyA,
    keyD,
    keyW,
    keyS,
    keyE,
    keyQ,
    keyR,
    keyT,
    keyF,
    keyG,
    keyX,
    keyC,
    keyV,
    keyB,
    keyU,
    keyH,
    keyJ,
    keyK,
    keyY,
    keyI,
    keyL,
    keyM,
    keyN,
    keyO,
    keyP,
    keyZ
  };

  inline static std::unordered_map<char, Buttons> keyMap = std::unordered_map<char, Buttons>({
    {'a',Buttons::keyA},
    {'b',Buttons::keyB},
    {'c',Buttons::keyC},
    {'d',Buttons::keyD},
    {'e',Buttons::keyE},
    {'f',Buttons::keyF},
    {'g',Buttons::keyG},
    {'h',Buttons::keyH},
    {'i',Buttons::keyI},
    {'j',Buttons::keyJ},
    {'k',Buttons::keyK},
    {'l',Buttons::keyL},
    {'m',Buttons::keyM},
    {'n',Buttons::keyN},
    {'o',Buttons::keyO},
    {'p',Buttons::keyP},
    {'q',Buttons::keyQ},
    {'r',Buttons::keyR},
    {'s',Buttons::keyS},
    {'t',Buttons::keyT},
    {'u',Buttons::keyU},
    {'v',Buttons::keyV},
    {'w',Buttons::keyW},
    {'x',Buttons::keyX},
    {'y',Buttons::keyY},
    {'z',Buttons::keyZ}
  });
#endif // __DESKTOP__
};
#endif //!Constants_class
