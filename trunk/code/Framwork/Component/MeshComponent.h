#ifndef  _MeshComponent__H__
#define  _MeshComponent__H__

#include "Component.h"


namespace ma
{
	class RenderMesh;
	class Material;
	
	class FRAMWORK_API MeshComponent : public Component
	{
		DECL_OBJECT(MeshComponent)

	public:
		MeshComponent(GameObject* pGameObj); 

		~MeshComponent();

		void			Render();

		void			Update();

		void			Start();

		void			Stop();

		void			Load(const char* pszMeshPath,const char* pszTexPath);

		RenderMesh*		GetRendMesh() {return m_pRendMesh;}

		Material*		GetMaterial() {return m_pMaterial;}

		void			SetMaterial(Material* pMaterial) {m_pMaterial = pMaterial;}

		void			GetBoundingAABB(Vector3& vMin,Vector3& vMax);

		virtual void	Serialize(Serializer& sl, const char* pszLable = "MeshComponent");

	private:
		RenderMesh*		m_pRendMesh;

		Material*		m_pMaterial;

		std::string		m_strMeshPath;
		std::string		m_strMaterialPath;
	};
}

#endif
