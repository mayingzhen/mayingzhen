#ifndef  _IRENDMESH__H__
#define  _IRENDMESH__H__

#include "Engine/RenderSystem/RenderObject.h"

namespace ma
{
	class MeshData;
	class Material;

	class ENGINE_API RenderMesh : public RenderObject
	{
	public:
		RenderMesh();

		virtual void	Render();

		bool			InitWithData(MeshData* pMeshData);

		bool			Load(const char* pszSknPath,const char* pszMatPath);

		void			SetSkinMatrix(const Matrix4x4* arrMatrixs,Uint nCount);
	
		void			SetWorldMatrix(const Matrix4x4& matWorld);

		void			SetMaterial(Material* pMaterial);

		Material*		GetMaterial() {return m_pMaterial;}

		MeshData*		GetMeshData() {return m_pMesData;}

		UINT			GetRenderableNumber() {return m_arrRenderable.size();}

		Renderable*		GetRenderableByIndex(UINT index) {return m_arrRenderable[index];}

		AABB			GetBoundingAABB();

// 		const char*		GetSknPath() {return m_pMesData ? m_pMesData->GetResPath() : NULL;}
// 
// 		const char*     GetMatPath() {return m_pMaterial ? m_pMaterial->GetResPath() : NULL;}

	public:
		//std::string				m_sknPath;
		//std::string				m_matPath;	
		
 		MeshData*				m_pMesData;

		//Texture*				m_pTexture;

		Material*				m_pMaterial;

		std::vector<Renderable*>	m_arrRenderable;

		bool					m_bSkin;
	};
}

#endif
