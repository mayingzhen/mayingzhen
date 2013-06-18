#ifndef _SAMPLES_STDAFX_HH__
#define _SAMPLES_STDAFX_HH__



#include "Common/Module.h"
#include "Engine/Module.h"
#include "Animation/Module.h"
#include "UI/Module.h"
#include "GLESRender/Module.h"

#if PLATFORM_WIN == 1
#include "D3D9Render/Module.h"
#endif


// #import <Availability.h>
// 
// #ifndef __IPHONE_5_0
// #warning "This project uses features only available in iOS SDK 5.0 and later."
// #endif
// 
// #ifdef __OBJC__
// #import <UIKit/UIKit.h>
// #import <Foundation/Foundation.h>
// #endif

#endif
