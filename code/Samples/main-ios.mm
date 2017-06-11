#import <Foundation/Foundation.h>

#include "SampleBrowser.h"

using namespace ma;


/**
 * Main entry point.
 */
int main(int argc, char** argv)
{
    NSAutoreleasePool *p = [[NSAutoreleasePool alloc] init];
    
    Setting set;
	set.bForceGLES = false;
	set.bRenderThread = false;
	set.bDataThread = false;
	set.bJobScheduler = true;

	SampleBrowser sampleBrowser("SampleBrowser",set,"SampleMaterial");
    
	sampleBrowser.Run();
	sampleBrowser.Shutdown();
    
    [p release];
    return 1;
}


