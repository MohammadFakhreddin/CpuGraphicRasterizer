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
#import <QuartzCore/QuartzCore.h>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad

{

    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    self.view = (GLKView *)self.view;
    
    self.view.context = self.context;

    self.view.drawableDepthFormat = GLKViewDrawableDepthFormat24;

    [self setupGL];

}

- (void)dealloc

{

    if ([EAGLContext currentContext] == self.context) {

        [EAGLContext setCurrentContext:nil];

    }

    delete self.application;

    delete self.iphoneHelperAbstraction;
        
}

- (void)setupGL

{

    [EAGLContext setCurrentContext:self.context];
    
    CGSize viewSize = self.view.frame.size;
    
    self.view.contentScaleFactor = 1.0f;
    
    unsigned int realScreenWidth = (unsigned int)viewSize.width;
    unsigned int realScreenHeight = (unsigned int)viewSize.height;
    unsigned int appScreenWidth = (unsigned int)(float(realScreenWidth)/2.0f);
    unsigned int appScreenHeight = (unsigned int)(appScreenWidth * (float(realScreenHeight)/float(realScreenWidth)));
    
    self.iphoneHelper = [[IPhoneHelper alloc] init];
    
    self.iphoneHelperAbstraction = new IPhoneHelperAbstraction((__bridge void *)(self.iphoneHelper));

    self.application = new Application(
                                        Application::Platform::Iphone,
                                        appScreenWidth,
                                        appScreenHeight,
                                        realScreenWidth,
                                        realScreenHeight
                                    );
}

CFTimeInterval lastTime = CACurrentMediaTime();
CFTimeInterval currentTime = 0;

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect

{
    currentTime = CACurrentMediaTime();
    self.application->mainLoop(currentTime - lastTime);
    lastTime = currentTime;
}


@end
