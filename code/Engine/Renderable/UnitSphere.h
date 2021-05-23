#ifndef  _UnitSphere__H__
#define  _UnitSphere__H__

namespace ma
{
	class UnitSphere
	{
	public:
		static void Init();
		
		static void Shoutdown();
		
		static Renderable* GetRenderable();

		static RefPtr<Renderable> CreateRenderable();

		static void Render(Technique* pTech,const Vector3& vPos,float fRadius, RenderCommand* pComomnd, SceneContext* sc);
	};
}


#endif

