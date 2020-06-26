#include "Application/Export.h"
#include "View.h"
#include "Application/Platform.h"
#include "Application/Game.h"
#include "imgui_impl_metal.h"

#import <QuartzCore/QuartzCore.h>


@implementation View

@synthesize m_pUpdating;

+ (Class) layerClass
{
    //if (ma::Game::GetInstance().GetSetting().eRenderType == R)
    //{
    //    return [CAEAGLLayer class];
    //}
    //else
    {
        return [CAMetalLayer class];
    }
}

- (void)insertText:(NSString *)text
{
}

- (void)deleteBackward
{
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
            //LogError("Invalid OS Version: %s\n", (currSysVer == NULL?"NULL":[currSysVer cStringUsingEncoding:NSASCIIStringEncoding]));
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
        CALayer* layer = (CALayer *)self.layer;
        layer.opaque = TRUE;
        //if ([layer isKindOfClass:[CAEAGLLayer class]])
        //{
        //    CAEAGLLayer* eaglLayer = (CAEAGLLayer*)layer;
        //    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
        //                                [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
        //                                kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        //}

        self.contentScaleFactor = scale;
        layer.contentsScale = scale;
    

        // Initialize Internal Defaults
        m_pDisplayLink = nil;
        m_bInited = FALSE;
        m_SwapInterval = 1;        
        m_pUpdating = FALSE;
        
        // Set the resource path and initalize the game
        NSString* bundlePath = [[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/"];
        std::string appPath = [bundlePath fileSystemRepresentation];
        ma::Platform::GetInstance().SetAppPath(appPath.c_str());
        ma::Platform::GetInstance().SetWindId(self.layer);
        
        std::string docPath = appPath + "Documents/";
        ma::GetArchiveMananger()->SetSaveDir(docPath.c_str());
        
        CGSize screen_size = [[UIScreen mainScreen] bounds].size;
        //CGFloat scale = [[UIScreen mainScreen].scale];
        CGSize resolution = CGSizeMake(screen_size.width * scale, screen_size.height * scale);
        
        NSUInteger width = resolution.width;
        NSUInteger height = resolution.height;
        
        //ImGuiIO &io = ImGui::GetIO();
        //io.DisplaySize.x = width;
        //io.DisplaySize.y = height;

    #if TARGET_OS_OSX
        CGFloat framebufferScale = view.window.screen.backingScaleFactor ?: NSScreen.mainScreen.backingScaleFactor;
    #else
        //CGFloat framebufferScale = scale;//view.window.screen.scale ?: UIScreen.mainScreen.scale;
    #endif
        //io.DisplayFramebufferScale = ImVec2(framebufferScale, framebufferScale);
                                      
        //io.DeltaTime = 1 / float(view.preferredFramesPerSecond ?: 60);
        
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


extern id<MTLDevice> GetMetalDevive();

- (void)update:(id)sender
{   
    if (!m_bInited)
    {
        m_bInited = TRUE;
        
        ma::Game::GetInstance().Init();
        
        //ImGui_ImplMetal_Init(GetMetalDevive());
    }
    
    //ImGui_ImplMetal_NewFrame();

    // Execute a single game frame
    ma::Game::GetInstance().Update();
        
    ma::Game::GetInstance().Render(); 
}


@end






