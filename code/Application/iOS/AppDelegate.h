#ifndef __AppDelegate_h__
#define __AppDelegate_h__

#import <UIKit/UIKit.h>

@class ViewController;
@class CMMotionManager;

@interface AppDelegate : UIApplication <UIApplicationDelegate>
{
    UIWindow*			m_pWindow;
    ViewController*		m_pViewController;
	CMMotionManager*	m_pMotionManager;
}

@property (nonatomic, retain) ViewController*		m_pViewController;

@end


#endif
