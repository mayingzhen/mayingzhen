#ifndef  _MESHCOMPONENT__H__
#define  _MESHCOMPONENT__H__

#include "Framework/Component.h"


namespace ma
{
	class MeshRes;
	class Texture;

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

		MeshRes* GetMeshRes() {return m_pMeshRes;}

		void SetMeshRes(MeshRes* pMeshRes) {m_pMeshRes = pMeshRes;}

		Texture* GetTexture() {return m_pTexture;}

		void SetTexture(Texture* pTexture) {m_pTexture = pTexture;}

		void GetBoundingAABB(D3DXVECTOR3& vMin,D3DXVECTOR3& vMax);

		virtual void Serialize(SerializeListener& sl, const char* pszLable = "MeshComponent");

	private:
		MeshRes*		m_pMeshRes;
		Texture*	m_pTexture;
		

	};



}




#endif