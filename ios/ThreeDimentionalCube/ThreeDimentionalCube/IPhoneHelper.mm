//
//  IPhoneHelper.m
//  ThreeDimentionalCube
//
//  Created by mohammad.fakhreddin on 3/13/20.
//  Copyright © 2020 mohammad.fakhreddin. All rights reserved.
//
#include "IPhoneHelper.h"
#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <UIKit/UIKit.h>
#include <string>
#include <AssetsLibrary/AssetsLibrary.h>

#define Clamp255(a) (a>255 ? 255 : a)

ImageData loadImage( void* self, std::string imageName ){
    id callerObject = (__bridge id)(self);
    assert(callerObject);
    log(self, "Trying to load image from xcode assets:"+imageName);
    auto imageData = [callerObject loadImage:imageName];
    log(self, "Loading image is complete");
    return imageData;
};

void log( void * self, std::string logName ){
    id callerObject = (__bridge id)(self);
    assert(callerObject);
    [callerObject log:logName];
};

@implementation IPhoneHelper

- (ImageData) loadImage:(std::string)rawImageName{
    NSString *imageName = [NSString stringWithCString:rawImageName.c_str() encoding:[NSString defaultCStringEncoding]];
    assert(imageName);

    auto uiImage = [UIImage imageNamed:imageName];
    assert(uiImage);
    auto cgImage = uiImage.CGImage;
    assert(cgImage);
    ImageData data;
    
    data.width = CGImageGetWidth(cgImage);
    data.height = CGImageGetHeight(cgImage);
    data.numberOfChannels = 4;

    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    int bytesPerPixel = 4;
    int bytesPerRow = (int)(data.width * bytesPerPixel);
    NSUInteger bitsPerComponent = 8;
    auto pixelCount = data.width * data.height;
    auto pixelMapSize = data.numberOfChannels * pixelCount;
    
    data.pixels = new unsigned char[pixelMapSize];
    CGContextRef context = CGBitmapContextCreate(data.pixels,
                                                 data.width,
                                                 data.height,
                                                 bitsPerComponent,
                                                 bytesPerRow,
                                                 colorSpace,
                                                 (CGBitmapInfo)kCGImageAlphaNoneSkipLast);
    CGContextDrawImage(context, CGRectMake(0.0f, 0.0f, data.width, data.height), cgImage);
    CGContextRelease(context);
    
    return data;
}

- (void) log:(std::string)rawLog{
    NSString *parsedLog = [NSString stringWithCString:rawLog.c_str() encoding:[NSString defaultCStringEncoding]];
    NSLog(@"%@", parsedLog);
}

@end
