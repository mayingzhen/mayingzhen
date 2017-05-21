#include "GLESRender/stdafx.h"
#include "GLESDeviceContextIOS.h"


#include <OpenGLES/EAGL.h>
#include <OpenGLES/EAGLDrawable.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

namespace ma
{

	bool GLESDeviceContext::Init(HWND wndHandle)
	{
        EAGLContext* pEAGLContext = nil;
        
		// 创建OpenGL ES 渲染环境
		pEAGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
		if(pEAGLContext == nil)
		{
			NSLog(@"Fail to allocate EAGLContext");
			return false;
		}
        
        bool result = [EAGLContext setCurrentContext:pEAGLContext];
        if (!result)
        {
            NSLog(@"Failed to make context current.");
            return false;
        }
        
        GL_ASSERT( glPixelStorei(GL_PACK_ALIGNMENT, 1) );
        GL_ASSERT( glPixelStorei(GL_UNPACK_ALIGNMENT, 1) );
        
		// Create the default frame buffer
		GL_ASSERT( glGenFramebuffers(1, &m_hDefaultFramebuffer) );
		GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, m_hDefaultFramebuffer) );
	    
		// Create a color buffer to attach to the frame buffer
		GL_ASSERT( glGenRenderbuffers(1, &m_hColorRenderbuffer) );
		GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, m_hColorRenderbuffer) );
        
        GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_hColorRenderbuffer) );
	    
		id<EAGLDrawable> eaglDrawable = id<EAGLDrawable>(wndHandle);

		// Associate render buffer storage with CAEAGLLauyer so that the rendered content is display on our UI layer.
		result = [pEAGLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglDrawable];
        ASSERT(result);
        if (!result)
        {
            NSLog(@"Fail to rederbufferStorage");
            return false;
        }
	    
		// Attach the color buffer to our frame buffer
		//GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_hColorRenderbuffer) );
	    
		// Retrieve framebuffer size
		GL_ASSERT( glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &m_nFramebufferWidth) );
		GL_ASSERT( glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &m_nFramebufferHeight) );
	    
        GL_ASSERT( glViewport(0, 0, m_nFramebufferWidth, m_nFramebufferHeight) );
        
		NSLog(@"width: %d, height: %d", m_nFramebufferWidth, m_nFramebufferHeight);
	    
		GL_ASSERT( glGenRenderbuffers(1, &m_hDepthRenderbuffer) );
		GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, m_hDepthRenderbuffer) );
		GL_ASSERT( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, m_nFramebufferWidth, m_nFramebufferHeight) );
		GL_ASSERT( glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_hDepthRenderbuffer) );

	    
		// Sanity check, ensure that the framebuffer is valid
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			NSLog(@"ERROR: Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
		
			return false;
		}
        
        
        m_pDeviceGLRC = pEAGLContext;
	    
		return true;
	}
    
    void GLESDeviceContext::MakeCurrent()
    {
        EAGLContext* pEAGLContext = (EAGLContext*)m_pDeviceGLRC;
        bool result = [EAGLContext setCurrentContext:pEAGLContext];
        if (!result)
        {
            NSLog(@"Failed to make context current.");
        }
        
        GL_ASSERT( glBindFramebuffer(GL_FRAMEBUFFER, m_hDefaultFramebuffer) );
    }

	void GLESDeviceContext::SwapBuffers()
	{
        if (m_pDeviceGLRC == NULL)
            return;
            
        // Present the color buffer
        GL_ASSERT( glBindRenderbuffer(GL_RENDERBUFFER, m_hColorRenderbuffer) );
        BOOL result =  [m_pDeviceGLRC presentRenderbuffer:GL_RENDERBUFFER];
        //ASSERT(result);
        
	}





}
