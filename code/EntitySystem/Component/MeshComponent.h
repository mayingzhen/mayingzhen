#ifndef  _MeshComponent__H__
#define  _MeshComponent__H__

#include "Component.h"


namespace ma
{
	class RenderMesh;
	class Material;
	
	class ENTITYSYSTEM_API MeshComponent : public Component
	{
		DECL_OBJECT(MeshComponent)

	public:
		MeshComponent(GameObject* pGameObj); 

		~MeshComponent();

		virtual	void	BuildRenderItem();

		void			Load(const char* pszSknPath,const char* pszMatPath);

		RenderMesh*		GetRendMesh() {return m_pRendMesh;}

		AABB			GetBoundingAABB();

		virtual void	Serialize(Serializer& sl, const char* pszLable = "MeshComponent");

	private:
		RenderMesh*		m_pRendMesh;

	};
}

#endif // _MeshComponent__H__
