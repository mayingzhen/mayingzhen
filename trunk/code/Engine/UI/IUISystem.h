#ifndef  _IUISystem__H__
#define  _IUISystem__H__

namespace ma
{
	class ENGINE_API IUISystem 
	{
	public:

		virtual void Init() = 0;

		virtual void Shutdown() = 0;
		
		virtual void Update() = 0;

		virtual void Render() = 0;

		virtual void OnFlushFrame() = 0;
	};

	ENGINE_API void SetUISystem(IUISystem* pUISystem);

	ENGINE_API IUISystem* GetUISystem();
}


#endif // _IUISystem__H__