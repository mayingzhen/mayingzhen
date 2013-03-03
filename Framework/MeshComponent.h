#ifndef  _MESHCOMPONENT__H__
#define  _MESHCOMPONENT__H__

#include "Framework/Component.h"


namespace ma
{
	
	class FRAMEWORK_API MeshComponent : public Component
	{
		DECL_OBJECT(MeshComponent)
	public:
		MeshComponent(); 

		~MeshComponent();

		void Render();

		void Update();

		void Start();

		void Stop();

		void Load(const char* pszMeshPath,const char* pszTexPath);

		IRendMesh* GetRendMesh() {return m_pRendMesh;}

		//void SetMeshRes(IRendMesh* pMeshRes) {m_pMeshRes = pMeshRes;}

		IRendTexture* GetTexture() {return m_pTexture;}

		void SetTexture(IRendTexture* pTexture) {m_pTexture = pTexture;}

		void GetBoundingAABB(D3DXVECTOR3& vMin,D3DXVECTOR3& vMax);

		virtual void Serialize(SerializeListener& sl, const char* pszLable = "MeshComponent");

	private:
		IRendMesh*		m_pRendMesh;
		IRendTexture*   m_pTexture;
	};
}

#endif
