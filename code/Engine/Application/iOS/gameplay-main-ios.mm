#ifdef __APPLE__

#import <Foundation/Foundation.h>
#include "Common/Module.h"
#include "Engine/Module.h"
#include "GLESRender/Module.h"
#include "Animation/Module.h"

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
    
    Platform app;
	app.Init();
	app.Run();
	app.Shutdown();
    
    [p release];
    return 1;
}

#endif