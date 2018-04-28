#include "Export.h"
#include "Platform.h"
#include "AppDelegate.h"


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
        
        float fScale = [[UIScreen mainScreen] scale];
        float width = [[UIScreen mainScreen] bounds].size.width * fScale;
        float height = [[UIScreen mainScreen] bounds].size.height * fScale;
        bool bFlib = [pAppDelegate.m_pViewController interfaceOrientation] == UIInterfaceOrientationPortrait ||
            [pAppDelegate.m_pViewController interfaceOrientation] == UIInterfaceOrientationPortraitUpsideDown;
        
        if (bFlib)
        {
            w = width;
            h = height;
        }
        else
        {
            w = height;
            h = width;
        }
	}
}

