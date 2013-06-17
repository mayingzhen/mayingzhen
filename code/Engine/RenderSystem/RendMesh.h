#ifndef  _IRENDMESH__H__
#define  _IRENDMESH__H__

//#include "Engine/RenderSystem/IRenderItem.h"
//#include "Renderable.h"

namespace ma
{
	class MeshData;
	class Material;

	class ENGINE_API RenderMesh 
	{
	public:
		bool InitWithData(MeshData* pMeshData);
		
		void Draw();

		void SetSkinMatrix(const Matrix4x4* arrMatrixs,Uint nCount);

		void SetMaterial(Material* pMaterial);

	public:
		
 		MeshData*				m_pMesData;

		Material*				m_pMaterial;

		std::vector<Renderable*>	m_arrRenderable;

	};
}

#endif
