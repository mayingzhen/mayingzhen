#ifndef  _IRENDWINDOW__H__
#define  _IRENDWINDOW__H__

namespace ma
{
	class COMMON_API IRendWindow
	{
	public:
		virtual void Init() = 0;

		virtual void Shutdown() = 0;

		virtual void BeginRender() = 0;
		
		virtual void EndRender() = 0;
	};
}



#endif