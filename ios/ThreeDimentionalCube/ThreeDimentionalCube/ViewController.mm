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

GLKView *view;
double lastTime = 0;
double currentTime = 0;
double deltaTime = 0;

- (void)viewDidLoad

{

    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    view = (GLKView *)self.view;
    
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

    delete self.iphoneHelperAbstraction;
        
}

- (void)setupGL

{

    [EAGLContext setCurrentContext:self.context];
    
    CGSize viewSize = self.view.frame.size;
    
    self.view.contentScaleFactor = 1.0f;
    
    unsigned int realScreenWidth = (unsigned int)viewSize.width;
    unsigned int realScreenHeight = (unsigned int)viewSize.height;
    unsigned int appScreenWidth = (unsigned int)(float(realScreenWidth)/1.5f);
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

- (double)currentTimeInMilliseconds {
    NSDate *date = [NSDate date];
    return [date timeIntervalSince1970]*1000;
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect

{
    currentTime = ([self currentTimeInMilliseconds]);
    deltaTime = (currentTime - lastTime);
    lastTime = currentTime;
    self.application->mainLoop(deltaTime);
}

@end
