//
//  ViewController.m
//  ThreeDimentionalCube
//
//  Created by mohammad.fakhreddin on 3/12/20.
//  Copyright © 2020 mohammad.fakhreddin. All rights reserved.
//

#import "ViewController.h"
#import <GLKit/GLKit.h>
#include "../../../src/application/Application.cpp"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad

{

    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {

        NSLog(@"Failed to create ES context");

    }

    GLKView *view = (GLKView *)self.view;

    view.context = self.context;

    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;

    [self setupGL];

}

- (void)dealloc

{

    if ([EAGLContext currentContext] == self.context) {

        [EAGLContext setCurrentContext:nil];

    }

}

- (void)setupGL

{

[EAGLContext setCurrentContext:self.context];

    on_surface_created();

    on_surface_changed();

}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect

{

    on_draw_frame();

}


@end
