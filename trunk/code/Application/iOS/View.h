#ifndef __View_h__
#define __View_h__

#import <UIKit/UIKit.h>

@interface View : UIView <UIKeyInput>
{
    CADisplayLink*				m_pDisplayLink;
    BOOL						m_bInited;
   
    NSInteger					m_SwapInterval;
    BOOL						m_pUpdating;
    BOOL						m_pOglDiscardSupported;
    
    UITapGestureRecognizer*		m_pTapRecognizer;
    UIPinchGestureRecognizer*	m_pPinchRecognizer;
    UISwipeGestureRecognizer*	m_pSwipeRecognizer;
}

@property (readonly, nonatomic, getter=isUpdating) BOOL m_pUpdating;

- (void)startUpdating;
- (void)stopUpdating;
- (void)update:(id)sender;
- (void)setSwapInterval:(NSInteger)interval;
- (int)swapInterval;
- (void)swapBuffers;
- (BOOL)showKeyboard;
- (BOOL)dismissKeyboard;
@end

@interface View (Private)
- (BOOL)createFramebuffer;
- (void)deleteFramebuffer;
@end


#endif



