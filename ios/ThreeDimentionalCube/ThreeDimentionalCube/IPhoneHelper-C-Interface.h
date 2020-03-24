//
//  IPhoneHelper-C-Interface.h
//  ThreeDimensionalCube
//
//  Created by mohammad.fakhreddin on 3/13/20.
//  Copyright © 2020 mohammad.fakhreddin. All rights reserved.
//

#ifndef IPhoneHelper_C_Interface_h
#define IPhoneHelper_C_Interface_h

#include <string>

struct ImageData{
    unsigned long width;
    unsigned long height;
    unsigned long numberOfChannels;
    unsigned char * pixels;
};

ImageData loadImage(void * ObjectiveCIPhoneHelperInstance,std::string imageName);

unsigned char * loadTextFile(void * ObjectiveCIPhoneHelperInstance,std::string fileName);

void log(void * ObjectiveCIPhoneHelperInstance,std::string logName);

std::string getPathNameForResource(void * ObjectiveCIPhoneHelperInstance,std::string fileName,std::string extension);

#endif /* IPhoneHelper_C_Interface_h */
