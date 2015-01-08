#include "UnitSphere.h"

namespace ma
{
	static Renderable* gpSphere = NULL;

	void UnitSphere::Render(Technique* pTech,const Vector3& vPos,float fRadius)
	{
		Matrix4 matTras;
		matTras.setTrans(vPos);
		matTras.setScale(Vector3(fRadius ,fRadius ,fRadius));

		gpSphere->m_matWorld[0] = matTras;
		gpSphere->m_matWorld[1] = matTras;		

		GetRenderSystem()->DrawRenderable(gpSphere,pTech);
	}


	void UnitSphere::Init()
	{
		RefPtr<MeshData> pMeshData = LoadResourceSync<MeshData>("FBX/shpere.skn");

		gpSphere = new Renderable();
		gpSphere->m_ePrimitiveType = PRIM_TRIANGLELIST;
		gpSphere->m_pDeclaration = pMeshData->GetVertexDeclar(); 
		gpSphere->m_pVertexBuffers = pMeshData->GetVertexBuffer(); 
		gpSphere->m_pIndexBuffer =  pMeshData->GetIndexBuffer();
		//gpSphere->m_pSubMeshData = pMeshData->GetSubMeshByIndex(0);

		return;
	}


	void UnitSphere::ShutDown()
	{
		SAFE_DELETE(gpSphere);		
	}
}

