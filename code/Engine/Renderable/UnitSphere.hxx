#include "UnitSphere.h"

namespace ma
{
	static Renderable* gpSphere = NULL;

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
		RefPtr<MeshData> pMeshData = LoadResourceSync<MeshData>("FBX/shpere.skn");

		gpSphere = new Renderable();
		gpSphere->m_ePrimitiveType = PRIM_TRIANGLELIST;
		gpSphere->m_pDeclaration = pMeshData->GetVertexDeclar(); 
		gpSphere->m_pVertexBuffers = pMeshData->GetVertexBuffer(); 
		gpSphere->m_pIndexBuffer =  pMeshData->GetIndexBuffer();

		return;
	}


	void UnitSphere::ShutDown()
	{
		SAFE_DELETE(gpSphere);		
	}
}

