#include "Export.h"
#include "View.h"
#include "Platform.h"
#include "Game.h"
#import <QuartzCore/QuartzCore.h>



@implementation View

@synthesize m_pUpdating;

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
            m_pOglDiscardSupported = YES;
        }
        else
        {
            m_pOglDiscardSupported = NO;
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
    

        // Initialize Internal Defaults
        m_pDisplayLink = nil;
        m_bInited = FALSE;
        m_SwapInterval = 1;        
        m_pUpdating = FALSE;
        
        // Set the resource path and initalize the game
        NSString* bundlePath = [[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/"];
        ma::Platform::GetInstance().SetAppPath([bundlePath fileSystemRepresentation]);
        ma::Platform::GetInstance().SetWindId(self.layer);
    }
    return self;
}

- (void) dealloc
{
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
        m_SwapInterval = interval;        
        if (m_pUpdating)
        {
            [self stopUpdating];
            [self startUpdating];
        }
    }
}

- (int)swapInterval 
{
    return m_SwapInterval;
}


- (void)startUpdating
{
    if (!m_pUpdating)
    {
        m_pDisplayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update:)];
        [m_pDisplayLink setFrameInterval:m_SwapInterval];
        [m_pDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        m_pUpdating = TRUE;
    }
}

- (void)stopUpdating
{
    if (m_pUpdating)
    {
        [m_pDisplayLink invalidate];
        m_pDisplayLink = nil;
        m_pUpdating = FALSE;
    }
}

- (void)update:(id)sender
{   
    if (!m_bInited)
    {
        m_bInited = TRUE;
        
        ma::Game::GetInstance().Init();  
    }

    // Execute a single game frame
    ma::Game::GetInstance().Update();
        
    ma::Game::GetInstance().Render(); 
}


@end






