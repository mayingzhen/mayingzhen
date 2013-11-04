#include "UnitSphere.h"

namespace ma
{
	static Renderable* gpSphere = NULL;

	void UnitSphere::Render(Technique* pTech,const Vector3& vPos,float fRadius)
	{
		Matrix4x4 matTras;
		MatrixTranslation(&matTras,vPos.x,vPos.y,vPos.z);
		Matrix4x4 matScale;
 		MatrixScaling(&matScale,fRadius ,fRadius ,fRadius );

		gpSphere->m_matWorld[0] = matScale * matTras;
		gpSphere->m_matWorld[1] = matScale * matTras;		
		//gpSphere->m_pMaterial = pMaterial;
		GetRenderSystem()->DrawRenderable(gpSphere,pTech);
	}


	void UnitSphere::Init()
	{
		MeshData* pMeshData = DeclareResource<MeshData>("FBX/shpere.skn");
		pMeshData->LoadSync();
	
		SubMeshData* pSubMesh = pMeshData->GetSubMeshByIndex(0);
		gpSphere = new Renderable();
		gpSphere->m_ePrimitiveType = PRIM_TRIANGLELIST;
		gpSphere->m_pDeclaration = pMeshData->GetVertexDeclar(); 
		gpSphere->m_pVertexBuffers = pMeshData->GetVertexBuffer(); 
		gpSphere->m_pIndexBuffer =  pMeshData->GetIndexBuffer();
		gpSphere->m_pSubMeshData = pSubMesh;

		return;
	}


	void UnitSphere::ShutDown()
	{

	}
}

