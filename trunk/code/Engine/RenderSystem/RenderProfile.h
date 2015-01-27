#ifndef  _RenderProfile__H__
#define  _RenderProfile__H__


namespace ma
{
	struct  RenderProfile
	{
		RenderProfile(const char * str)
		{
			GetRenderSystem()->BeginProfile(str);
		}

		~RenderProfile()
		{
			GetRenderSystem()->EndProfile();
		}
	};


	#define RENDER_PROFILE(name) RenderProfile __re_##name(#name)

}


#endif