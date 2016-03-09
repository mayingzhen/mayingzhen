#import <Foundation/Foundation.h>

#include "SampleBrowser.h"

using namespace ma;


/**
 * Main entry point.
 */
int main(int argc, char** argv)
{
    NSAutoreleasePool *p = [[NSAutoreleasePool alloc] init];
    
	bool bRenderThread = true;
	bool bDataThread = true;
	bool bJobScheduler = true;

	SampleBrowser sampleBrowser("SampleBrowser");

	sampleBrowser.Init(bRenderThread,bDataThread,bJobScheduler);
	sampleBrowser.Run();
	sampleBrowser.Shutdown();
    
    [p release];
    return 1;
}


