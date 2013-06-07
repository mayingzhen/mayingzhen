#ifndef __ApplicationAndroid_h__
#define __ApplicationAndroid_h__


#include "../ApplicationBase.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <jni.h>
#include <android_native_app_glue.h>

namespace ma
{
	class Input;

	class ENGINE_API Platform :  public ApplicationBase
	{
	public:
		Platform(const char* appID = NULL);

		virtual void		Init();

		virtual void		Run();

		virtual void		GetWindowSize(int& w, int& h);

		void				StartFrame() {m_bRunning = true;}

		void				StopFrame() {m_bRunning = false;}

		bool				ProcessFrame() {return m_bRunning;}

		static void			MsgProc(android_app* app, int cmd);
		
		static int32_t		InputProc(android_app* app, AInputEvent* event);

	protected:
		//ANativeWindow*		m_pWindow;

		bool				m_bRunning;
	};
}


#endif //__S3AUTApplicationAndroid_h__

