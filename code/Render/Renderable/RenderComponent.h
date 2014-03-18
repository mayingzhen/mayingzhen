#ifndef  _RenderComponent__H__
#define  _RenderComponent__H__


namespace ma
{
	class Material;


	class RENDER_API RenderComponent : public Component
	{
		DECL_OBJECT(RenderComponent)

	public:
		RenderComponent(GameObject* pGameObj);
	
		virtual UINT		GetRenderableNumber() = 0;
		 	
		virtual	Renderable*	GetRenderableByIndex(UINT index) = 0;
	};
}

#endif // _RenderComponent__H__
