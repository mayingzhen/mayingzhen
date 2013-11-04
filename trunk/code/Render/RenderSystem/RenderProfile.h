#ifndef  _RenderProfile__H__
#define  _RenderProfile__H__


namespace ma
{
	struct RENDER_API RenderProfile
	{
		RenderProfile(const char * str)
		{
			GetRenderDevice()->BeginProfile(str);
		}

		~RenderProfile()
		{
			GetRenderDevice()->EndProfile();
		}
	};


	#define RENDER_PROFILE(name) RenderProfile __re_##name(#name)

}


#endif