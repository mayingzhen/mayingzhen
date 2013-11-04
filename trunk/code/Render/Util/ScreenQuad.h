#ifndef  _SCREENQUAD__H__
#define  _SCREENQUAD__H__


namespace ma
{
	class RENDER_API ScreenQuad
	{
	public:
		static void Init();
		
		static void ShoutDown();

		static void Render(Technique* pTechnique);
	};

}

#endif


