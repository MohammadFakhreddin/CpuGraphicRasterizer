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

ImageData loadImage( void * self, std::string imageName ){
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

unsigned char * loadTextFile( void * self , std::string textFileName ){
    id callerObject = (__bridge id)(self);
    assert(callerObject);
    return [callerObject loadTextFile:textFileName];
};

std::string getPathNameForResource(void * self,std::string fileName,std::string extension){
    id callerObject = (__bridge id)(self);
    assert(callerObject);
    return [callerObject getPathNameForResource:fileName extension:extension];
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

- (unsigned char *) loadTextFile:(std::string)rawTextFileName{
    NSString * textFileName = [NSString stringWithCString:rawTextFileName.c_str() encoding:[NSString defaultCStringEncoding]];
    assert(textFileName);
    
    NSString * path = [NSBundle.mainBundle pathForResource:textFileName ofType:@"obj"];

    NSError* error;
    NSString *fileContent = [[NSString alloc]initWithContentsOfFile:path encoding:NSUTF8StringEncoding error:&error];
    if(error){
        NSLog(@"%@",[error localizedDescription]);
    }
    assert(!error);
    assert(fileContent);
   
    return (unsigned char *)[fileContent UTF8String];
}

- (std::string) getPathNameForResource: (std::string) rawFileName extension:(std::string) rawExtensionName{
    NSString * fileName = [NSString stringWithCString:rawFileName.c_str() encoding:[NSString defaultCStringEncoding]];
    assert(fileName);
    
    NSString * extensionName = [NSString stringWithCString:rawExtensionName.c_str() encoding:[NSString defaultCStringEncoding]];
    assert(extensionName);
    
    NSString * path = [NSBundle.mainBundle pathForResource:fileName ofType:extensionName];
    return (std::string)[path UTF8String];
}

@end
