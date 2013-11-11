#import <Foundation/Foundation.h>

#include "SampleBrowser.h"

using namespace ma;

extern int __argc;
extern char** __argv;

/**
 * Main entry point.
 */
int main(int argc, char** argv)
{
    __argc = argc;
    __argv = argv;
    NSAutoreleasePool *p = [[NSAutoreleasePool alloc] init];
    
    SampleBrowser sampleBrowser("SampleBrowser");
    
    Platform app;
    
	app.Init();
	app.Run();
	app.Shutdown();
    
    [p release];
    return 1;
}


