#ifndef  _IRENDMESH__H__
#define  _IRENDMESH__H__

#include "Engine/Thread/DataThread.h"

namespace ma
{
	class MeshData;
	class Material;

	class ENGINE_API RenderMesh : public IDataObj
	{
	public:
		RenderMesh();

		bool		LoadToMemory();

		bool		LoadImp();

		bool		InitWithData(MeshData* pMeshData);

		bool		Load(const char* pMeshPath,const char* pDiffueTexture);
		
		void		Draw();

		void		SetSkinMatrix(const Matrix4x4* arrMatrixs,Uint nCount);

		void		SetWorldMatrix(const Matrix4x4& matWorld);

		void		SetMaterial(Material* pMaterial);

		Material*	GetMaterial() {return m_pMaterial;}

		UINT		GetRenderableNumber() {return m_arrRenderable.size();}

		Renderable*	GetRenderableByIndex(UINT index) {return m_arrRenderable[index];}

		AABB		GetBoundingAABB();

	public:
		std::string				m_sknPath;
		std::string				m_texPath;	
		
 		MeshData*				m_pMesData;

		TextureData*			m_pTextureData;

		Material*				m_pMaterial;

		std::vector<Renderable*>	m_arrRenderable;

		bool					m_bSkin;

	};
}

#endif
