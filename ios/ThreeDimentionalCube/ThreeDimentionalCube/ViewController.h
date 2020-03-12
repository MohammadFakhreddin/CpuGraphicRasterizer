//
//  ViewController.h
//  ThreeDimentionalCube
//
//  Created by mohammad.fakhreddin on 3/12/20.
//  Copyright © 2020 mohammad.fakhreddin. All rights reserved.
//
#ifndef GLES_SILENCE_DEPRECATION
    #define GLES_SILENCE_DEPRECATION
#endif
#import <GLKit/GLKit.h>
#include "./../../../src/application/Application.h"

@interface ViewController : GLKViewController

@property (strong, nonatomic) EAGLContext *context;
@property Application *application;

- (void)setupGL;

@end

