#import <Foundation/Foundation.h>

#include "SampleBrowser.h"

using namespace ma;


/**
 * Main entry point.
 */
int main(int argc, char** argv)
{
    NSAutoreleasePool *p = [[NSAutoreleasePool alloc] init];
    
    SampleBrowser sampleBrowser("SampleBrowser");
    
    Platform app;
    
	app.Init();
	app.Run();
	app.Shutdown();
    
    [p release];
    return 1;
}


