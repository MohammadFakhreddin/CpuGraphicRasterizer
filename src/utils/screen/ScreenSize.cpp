#include "ScreenSize.h"

#if defined(__PLATFORM_WIN__)
	#include <windows.h>
#elif defined(__PLATFORM_LINUX__)
	//TODO Define here
#elif defined(__PLATFORM_MAC__)
	#include <CoreGraphics/CGDisplayConfiguration.h>
#endif

void ScreenSize::getScreenResolution(unsigned int& width, unsigned int& height) {
#if defined(__PLATFORM_WIN__)
	width = (int)GetSystemMetrics(SM_CXSCREEN);
	height = (int)GetSystemMetrics(SM_CYSCREEN);
#elif defined(__PLATFORM_MAC__)
	auto mainDisplayId = CGMainDisplayID();
	width = CGDisplayPixelsWide(mainDisplayId);
	height = CGDisplayPixelsHigh(mainDisplayId);
#elif defined(__PLATFORM_LINUX__)
//TODO
	width = 0;
	height = 0;
#else
//Means that it is an unknown platform
	width = 0;
	height = 0;
#endif
}