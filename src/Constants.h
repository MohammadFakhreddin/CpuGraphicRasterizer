#pragma once
//
// Created by mohammad.fakhreddin on 1/3/20.
//
#include <math.h>

template<typename T>
T constexpr _piTemplate = (T)3.14159265358979323846264338327950288419;

namespace Constants {
	namespace Math {
		static constexpr float piFloat = _piTemplate<float>;
		static constexpr double piDouble = _piTemplate<double>;
	};
	namespace Window {
		static constexpr char appName[] = "3D CUBE";
		static int screenWidth = 800;
		static int screenHeight = 600;
	};
	namespace System {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
		//define something for Windows (32-bit and 64-bit, this part is common)
		#define __PLATFORM_WIN__
		#if !defined(__DESKTOP__)
			#define __DESKTOP__
		#endif

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
	#if !defined(__MOBILE__)
	#define __MOBILE__
	#endif
	#if !defined(__IOS__)
	#define __IOS__
	#endif
#elif TARGET_OS_MAC
		// Other kinds of Mac OS
	#if !defined(__DESKTOP__)
	#define __DESKTOP__
	#endif
	#define __PLATFORM_MAC__
#else
#   error "Unknown Apple platform"
#endif
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
#elif __ANDROID__
	#if !defined(__MOBILE__)
	#define __MOBILE__
	#endif
#else
#   error "Unknown compiler"
#endif
	};
};