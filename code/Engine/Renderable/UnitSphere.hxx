#include "UnitSphere.h"

namespace ma
{
	static MeshRenderable* gpSphere = NULL;

	void UnitSphere::Render(Technique* pTech,const Vector3& vPos,float fRadius)
	{
		Matrix4 matTras = Matrix4::IDENTITY;  
		matTras.setTrans(vPos);
		matTras.setScale(Vector3(fRadius ,fRadius ,fRadius));

		gpSphere->SetWorldMatrix(matTras);
		
		gpSphere->Render(pTech);
	}


	void UnitSphere::Init()
	{
		RefPtr<MeshData> pMeshData = CreateMeshData("FBX/shpere.skn");

		gpSphere = new MeshRenderable();
		gpSphere->m_ePrimitiveType = PRIM_TRIANGLELIST;
		gpSphere->m_pDeclaration = pMeshData->GetVertexDeclar(); 
		gpSphere->m_pVertexBuffer = pMeshData->GetVertexBuffer(); 
		gpSphere->m_pIndexBuffer =  pMeshData->GetIndexBuffer();
		gpSphere->m_posAABB = pMeshData->GetBoundingAABB();
		gpSphere->m_tcAABB = pMeshData->GetUVBoundingAABB();

		return;
	}


	void UnitSphere::ShutDown()
	{
		SAFE_DELETE(gpSphere);		
	}
}

