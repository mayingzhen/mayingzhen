#import <CoreMotion/CoreMotion.h>
#include "AppDelegate.h"
#include "ViewController.h"


@implementation AppDelegate

@synthesize m_pViewController;

- (BOOL)application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    [UIApplication sharedApplication].statusBarHidden = YES;
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];

    m_pMotionManager = [[CMMotionManager alloc] init];
    if([m_pMotionManager isAccelerometerAvailable] == YES) 
    {
        m_pMotionManager.accelerometerUpdateInterval = 1 / 40.0;    // 40Hz
        [m_pMotionManager startAccelerometerUpdates];
    }
    if([m_pMotionManager isGyroAvailable] == YES)
    {
        m_pMotionManager.gyroUpdateInterval = 1 / 40.0;    // 40Hz
        [m_pMotionManager startGyroUpdates];
    }
    
    m_pWindow = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    m_pViewController = [[ViewController alloc] init];
    [m_pWindow setRootViewController:m_pViewController];
    [m_pWindow makeKeyAndVisible];
    return YES;
}



- (void)applicationWillResignActive:(UIApplication*)application
{    
    [m_pViewController stopUpdating];
}

- (void)applicationDidEnterBackground:(UIApplication*)application 
{
    [m_pViewController stopUpdating];
}

- (void)applicationWillEnterForeground:(UIApplication*)application 
{    
    [m_pViewController startUpdating];
}

- (void)applicationDidBecomeActive:(UIApplication*)application 
{    
    [m_pViewController startUpdating];
}

- (void)applicationWillTerminate:(UIApplication*)application 
{    
    [m_pViewController stopUpdating];
}

- (void)dealloc 
{
    [m_pWindow setRootViewController:nil];
    [m_pViewController release];
    [m_pWindow release];
    [m_pMotionManager release];
    [super dealloc];
}

@end


