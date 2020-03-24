//
//  IPhoneHelper.h
//  ThreeDimentionalCube
//
//  Created by mohammad.fakhreddin on 3/13/20.
//  Copyright © 2020 mohammad.fakhreddin. All rights reserved.
//
#ifndef IPhoneHelper_class
#define IPhoneHelper_class

#import <Foundation/Foundation.h>
#include <string>
#include <UIKit/UIKit.h>
#include "./IPhoneHelper-C-Interface.h"

@interface IPhoneHelper : NSObject

- (ImageData) loadImage: (std::string) imageName;

- (void) log:(std::string) logName;

- (unsigned char *) loadTextFile: (std::string) textFileName;

- (std::string) getPathNameForResource: (std::string) fileName extension:(std::string) extension;

@end

#endif
