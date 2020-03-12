//
//  ViewController.h
//  ThreeDimentionalCube
//
//  Created by mohammad.fakhreddin on 3/12/20.
//  Copyright © 2020 mohammad.fakhreddin. All rights reserved.
//
#define GLES_SILENCE_DEPRECATION 
#import <GLKit/GLKit.h>

@interface ViewController : GLKViewController

@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;

@end

