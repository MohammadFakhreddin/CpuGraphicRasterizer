//
//  ViewController.h
//  ThreeDimentionalCube
//
//  Created by mohammad.fakhreddin on 3/12/20.
//  Copyright © 2020 mohammad.fakhreddin. All rights reserved.
//
#ifndef ViewController_class
#define ViewController_class

#ifndef GLES_SILENCE_DEPRECATION
    #define GLES_SILENCE_DEPRECATION
#endif
#import <GLKit/GLKit.h>
#include "./../../../src/application/Application.h"
#include "./IPhoneHelperAbstraction.h"
#include "./IPhoneHelper.h"

@interface ViewController : GLKViewController

@property (strong, nonatomic) EAGLContext *context;
@property (strong) IPhoneHelper* iphoneHelper;
@property IPhoneHelperAbstraction* iphoneHelperAbstraction;
@property Application* application;
@property (strong) GLKView *view;

- (void)setupGL;

@end

#endif
