#ifndef  _SCREENQUAD__H__
#define  _SCREENQUAD__H__


namespace ma
{
	class ScreenQuad
	{
	public:
		static void Init();

		//static void Reset(uint32 nWidth,uint32 nHeight);
		
		static void Shoutdown();

		static void Render(Technique* pTechnique);
	};

}

#endif


