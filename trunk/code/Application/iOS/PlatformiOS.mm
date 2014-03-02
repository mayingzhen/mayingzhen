#ifdef __APPLE__

#include "Common/Module.h"
#include "Engine/Module.h"
#include <unistd.h>
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <CoreMotion/CoreMotion.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <mach/mach_time.h>

#define UIInterfaceOrientationEnum(x) ([x isEqualToString:@"UIInterfaceOrientationPortrait"]?UIInterfaceOrientationPortrait:                        \
                                      ([x isEqualToString:@"UIInterfaceOrientationPortraitUpsideDown"]?UIInterfaceOrientationPortraitUpsideDown:    \
                                      ([x isEqualToString:@"UIInterfaceOrientationLandscapeLeft"]?UIInterfaceOrientationLandscapeLeft:              \
                                        UIInterfaceOrientationLandscapeRight)))
#define DeviceOrientedSize(o)         ((o == UIInterfaceOrientationPortrait || o == UIInterfaceOrientationPortraitUpsideDown)?                      \
                                            CGSizeMake([[UIScreen mainScreen] bounds].size.width * [[UIScreen mainScreen] scale], [[UIScreen mainScreen] bounds].size.height * [[UIScreen mainScreen] scale]):  \
                                            CGSizeMake([[UIScreen mainScreen] bounds].size.height * [[UIScreen mainScreen] scale], [[UIScreen mainScreen] bounds].size.width * [[UIScreen mainScreen] scale]))

using namespace std;
using namespace ma;

// UIScreen bounds are provided as if device was in portrait mode  defaults to landscape
extern const int WINDOW_WIDTH  = [[UIScreen mainScreen] bounds].size.height * [[UIScreen mainScreen] scale];
extern const int WINDOW_HEIGHT = [[UIScreen mainScreen] bounds].size.width * [[UIScreen mainScreen] scale];
extern const int WINDOW_SCALE = [[UIScreen mainScreen] scale];

int __argc = 0;
char** __argv = 0;

@class AppDelegate;
@class View;

static Platform* __platform = NULL;
static AppDelegate *__appDelegate = NULL;
static View* __view = NULL;


@interface View : UIView <UIKeyInput>
{
    //EAGLContext* context;
    CADisplayLink* displayLink;
    BOOL    m_bInited;
   
    NSInteger swapInterval;
    BOOL updating;
    BOOL oglDiscardSupported;
    
    UITapGestureRecognizer *_tapRecognizer;
    UIPinchGestureRecognizer *_pinchRecognizer;
    UISwipeGestureRecognizer *_swipeRecognizer;
}

@property (readonly, nonatomic, getter=isUpdating) BOOL updating;
@property (readonly, nonatomic, getter=getContext) EAGLContext* context;

- (void)startUpdating;
- (void)stopUpdating;
- (void)update:(id)sender;
- (void)setSwapInterval:(NSInteger)interval;
- (int)swapInterval;
- (void)swapBuffers;
- (BOOL)showKeyboard;
- (BOOL)dismissKeyboard;
@end

@interface View (Private)
- (BOOL)createFramebuffer;
- (void)deleteFramebuffer;
@end

@implementation View

@synthesize updating;
@synthesize context;

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id) initWithFrame:(CGRect)frame
{
    if ((self = [super initWithFrame:frame]))
    {
        // A system version of 3.1 or greater is required to use CADisplayLink. 
        NSString *reqSysVer = @"3.1";
        NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
        if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
        {
            // Log the system version
            NSLog(@"System Version: %@", currSysVer);
        }
        else
        {
            //GP_ERROR("Invalid OS Version: %s\n", (currSysVer == NULL?"NULL":[currSysVer cStringUsingEncoding:NSASCIIStringEncoding]));
            [self release];
            return nil;
        }
        
        // Check for OS 4.0+ features
        if ([currSysVer compare:@"4.0" options:NSNumericSearch] != NSOrderedAscending)
        {
            oglDiscardSupported = YES;
        }
        else
        {
            oglDiscardSupported = NO;
        }
        
        // Configure the CAEAGLLayer and setup out the rendering context
        CGFloat scale = [[UIScreen mainScreen] scale];
        CAEAGLLayer* layer = (CAEAGLLayer *)self.layer;
        layer.opaque = TRUE;
        layer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                   [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, 
                                    kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        self.contentScaleFactor = scale;
        layer.contentsScale = scale;
        
        //context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        //if (!context || ![EAGLContext setCurrentContext:context])
        //{
        //    //GP_ERROR("Failed to make context current.");
        //    [self release];
        //    return nil;
        //}

        // Initialize Internal Defaults
        displayLink = nil;
        m_bInited = FALSE;
        swapInterval = 1;        
        updating = FALSE;
        
        // Set the resource path and initalize the game
        NSString* bundlePath = [[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/"];
        __platform->SetAppPath([bundlePath fileSystemRepresentation]);
        __platform->SetWindId(self.layer);
    }
    return self;
}

- (void) dealloc
{
    //Game::GetInstance().Shutdown();
    
    [super dealloc];
}

- (BOOL)canBecomeFirstResponder 
{
    // Override so we can control the keyboard
    return YES;
}

- (void) layoutSubviews
{
    // Called on 'resize'.
    // Mark that framebuffer needs to be updated.
    // NOTE: Current disabled since we need to have a way to reset the default frame buffer handle
    // in FrameBuffer.cpp (for FrameBuffer:bindDefault). This means that changing orientation at
    // runtime is currently not supported until we fix this.
    //updateFramebuffer = YES;
}


- (void)setSwapInterval:(NSInteger)interval
{
    if (interval >= 1)
    {
        swapInterval = interval;        
        if (updating)
        {
            [self stopUpdating];
            [self startUpdating];
        }
    }
}

- (int)swapInterval 
{
    return swapInterval;
}



- (void)startUpdating
{
    if (!updating)
    {
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update:)];
        [displayLink setFrameInterval:swapInterval];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        //if (game)
        //    game->resume();
        updating = TRUE;
    }
}

- (void)stopUpdating
{
    if (updating)
    {
        //if (game)
        //    game->pause();
        [displayLink invalidate];
        displayLink = nil;
        updating = FALSE;
    }
}

- (void)update:(id)sender
{   
 
    if (!m_bInited)
    {
        m_bInited = TRUE;
        
        Game::GetInstance().Init();
        
        //return;
    }

    // Execute a single game frame
    Game::GetInstance().Update();
        
    Game::GetInstance().Render();
    
}





@end


@interface ViewController : UIViewController
- (void)startUpdating;
- (void)stopUpdating;
@end


@implementation ViewController 

- (id)init 
{
    if((self = [super init])) 
    {
    }
    return self;
}

- (void)dealloc 
{
    __view = nil;
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

#pragma mark - View lifecycle
- (void)loadView
{
    self.view = [[[View alloc] init] autorelease];
    if(__view == nil) 
    {
        __view = (View*)self.view;
    }
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Fetch the supported orientations array
    NSArray *supportedOrientations = NULL;
    if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) 
    {
        supportedOrientations = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"UISupportedInterfaceOrientations~ipad"];    
    }
    else if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) 
    { 
        supportedOrientations = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"UISupportedInterfaceOrientations~iphone"];
    }
    
    if(supportedOrientations == NULL) 
    {
       supportedOrientations = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"UISupportedInterfaceOrientations"]; 
    }

    // If no supported orientations default to v1.0 handling (landscape only)
    if(supportedOrientations == nil) {
        return UIInterfaceOrientationIsLandscape(interfaceOrientation);
    }
    for(NSString *s in supportedOrientations) {
        if(interfaceOrientation == UIInterfaceOrientationEnum(s)) return YES;
    }    
    return NO;
}

- (void)startUpdating 
{
    [(View*)self.view startUpdating];
}

- (void)stopUpdating 
{
    [(View*)self.view stopUpdating];
}

@end


@interface AppDelegate : UIApplication <UIApplicationDelegate>
{
    UIWindow* window;
    ViewController* viewController;
    CMMotionManager *motionManager;
}
@property (nonatomic, retain) ViewController *viewController;
@end


@implementation AppDelegate

@synthesize viewController;

- (BOOL)application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    __appDelegate = self;
    [UIApplication sharedApplication].statusBarHidden = YES;
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];

    motionManager = [[CMMotionManager alloc] init];
    if([motionManager isAccelerometerAvailable] == YES) 
    {
        motionManager.accelerometerUpdateInterval = 1 / 40.0;    // 40Hz
        [motionManager startAccelerometerUpdates];
    }
    if([motionManager isGyroAvailable] == YES)
    {
        motionManager.gyroUpdateInterval = 1 / 40.0;    // 40Hz
        [motionManager startGyroUpdates];
    }
    
    window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    viewController = [[ViewController alloc] init];
    [window setRootViewController:viewController];
    [window makeKeyAndVisible];
    return YES;
}



- (void)applicationWillResignActive:(UIApplication*)application
{    
    [viewController stopUpdating];
}

- (void)applicationDidEnterBackground:(UIApplication*)application 
{
    [viewController stopUpdating];
}

- (void)applicationWillEnterForeground:(UIApplication*)application 
{    
    [viewController startUpdating];
}

- (void)applicationDidBecomeActive:(UIApplication*)application 
{    
    [viewController startUpdating];
}

- (void)applicationWillTerminate:(UIApplication*)application 
{    
    [viewController stopUpdating];
}

- (void)dealloc 
{
    [window setRootViewController:nil];
    [viewController release];
    [window release];
    [motionManager release];
    [super dealloc];
}

@end



namespace ma
{
    

    
Platform&	Platform::GetInstance()
{
    return *__platform;
}

Platform::Platform()
{
    __platform = this;
}
    
void Platform::Init()
{
    //NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    //[pool release];
}

void Platform::Shutdown()
{
    // Cannot 'exit' an iOS Application
    //assert(false);
    //[__view stopUpdating];
    //exit(0);
}
    
void Platform::Run()
{
    [AppDelegate load];
    UIApplicationMain(0, nil, NSStringFromClass([AppDelegate class]), NSStringFromClass([AppDelegate class]));
    
    //NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    //[AppDelegate load];
    //UIApplicationMain(0, nil, NSStringFromClass([AppDelegate class]), NSStringFromClass([AppDelegate class]));
    //[pool release];
    //return EXIT_SUCCESS;        
}
    

void Platform::GetWindowSize(int& w, int& h) const
{
    CGSize size = DeviceOrientedSize([__appDelegate.viewController interfaceOrientation]);
    w = size.width;
    h = size.height;
}


void Platform::swapBuffers()
{
    //if (__view)
    //    [__view swapBuffers];
}
    
void Platform::sleep(long ms)
{
    usleep(ms * 1000);
}
 
}

#endif
