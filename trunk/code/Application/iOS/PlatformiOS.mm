#include "Export.h"
#include "Platform.h"
#include "AppDelegate.h"


#define DeviceOrientedSize(o)         ((o == UIInterfaceOrientationPortrait || o == UIInterfaceOrientationPortraitUpsideDown)?                      \
                                            CGSizeMake([[UIScreen mainScreen] bounds].size.width * [[UIScreen mainScreen] scale], [[UIScreen mainScreen] bounds].size.height * [[UIScreen mainScreen] scale]):  \
                                            CGSizeMake([[UIScreen mainScreen] bounds].size.height * [[UIScreen mainScreen] scale], [[UIScreen mainScreen] bounds].size.width * [[UIScreen mainScreen] scale]))



namespace ma
{
	static Platform* gplatform = NULL;
	
	Platform&	Platform::GetInstance()
	{
		return *gplatform;
	}

	Platform::Platform()
	{
		gplatform = this;
	}
	    
	void Platform::Init()
	{

	}

	void Platform::Shutdown()
	{
	}
	    
	void Platform::Run()
	{
		[AppDelegate load];
		UIApplicationMain(0, nil, NSStringFromClass([AppDelegate class]), NSStringFromClass([AppDelegate class]));   
	}

	void Platform::GetWindowSize(int& w, int& h) const
	{
        AppDelegate* pAppDelegate = (AppDelegate*)[UIApplication sharedApplication];
        
		CGSize size = DeviceOrientedSize([pAppDelegate.m_pViewController interfaceOrientation]);
		w = size.width;
		h = size.height;
	}
 
}

