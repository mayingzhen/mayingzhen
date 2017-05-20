#import <Foundation/Foundation.h>

#include "SampleBrowser.h"

using namespace ma;


/**
 * Main entry point.
 */
int main(int argc, char** argv)
{
    NSAutoreleasePool *p = [[NSAutoreleasePool alloc] init];
    
	bool bForceGLES = false;
	bool bRenderThread = false;
	bool bDataThread = false;
	bool bJobScheduler = true;

	SampleBrowser sampleBrowser("SampleBrowser");

	sampleBrowser.Init(bForceGLES,bRenderThread,bDataThread,bJobScheduler,"SampleMaterial");
	sampleBrowser.Run();
	sampleBrowser.Shutdown();
    
    [p release];
    return 1;
}


