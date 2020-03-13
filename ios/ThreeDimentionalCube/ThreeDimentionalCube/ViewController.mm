//
//  ViewController.m
//  ThreeDimentionalCube
//
//  Created by mohammad.fakhreddin on 3/12/20.
//  Copyright © 2020 mohammad.fakhreddin. All rights reserved.
//

#import "ViewController.h"
#import <GLKit/GLKit.h>
#include <memory>

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
    
    delete self.application;

}

- (void)setupGL

{

    [EAGLContext setCurrentContext:self.context];
    
    CGSize viewSize = self.view.frame.size;
    unsigned int realScreenWidth = viewSize.width;
    unsigned int realScreenHeight = viewSize.height;
    unsigned int appScreenWidth = int(float(realScreenWidth)/1.0f);
    unsigned int appScreenHeight = int(float(appScreenWidth) * (float(realScreenHeight) / float(realScreenWidth)));
    
    self.application = new Application(
                                        Application::Platform::Iphone,
                                        appScreenWidth,
                                        appScreenHeight,
                                        realScreenWidth,
                                        realScreenHeight
                                    );

}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect

{
    //TODO Calculate delta time
    self.application->mainLoop(1);
}


@end
