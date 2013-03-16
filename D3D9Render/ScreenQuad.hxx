#include "ScreenQuad.h"

namespace ma
{
	void ScreenQuad::Init(LPDIRECT3DDEVICE9 pDxDevice)
	{
		m_pDxDevice = pDxDevice;

		D3DVERTEXELEMENT9 vertexElements[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, 
			{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		m_pDxDevice->CreateVertexDeclaration(vertexElements, &m_pVertexDeclaration);


		// Construct full screen quad
		int width, Heigh;
		GetRenderDevice()->GetRenderWndSize(width,Heigh);
		D3DXVECTOR2 offset = D3DXVECTOR2(0.5f / width, 0.5f / Heigh);

		//   1    0 
		//   +----+
		//   | \  |  
		//   |  \ |
		//   +----+
		//   3    2
		/// view space near plan  ccw index  
		m_quadVerts[0].position = D3DXVECTOR3(1, 1, 0);
		m_quadVerts[1].position = D3DXVECTOR3(-1, +1, 0);
		m_quadVerts[2].position = D3DXVECTOR3(1, -1, 0);
		m_quadVerts[3].position = D3DXVECTOR3(-1, -1, 0);

		m_quadVerts[0].texCoords = D3DXVECTOR2(0.0f, 1.0f) + offset;
 		m_quadVerts[1].texCoords = D3DXVECTOR2(0.0f, 0.0f) + offset;
 		m_quadVerts[2].texCoords = D3DXVECTOR2(1.0f, 1.0f) + offset;
 		m_quadVerts[3].texCoords = D3DXVECTOR2(0.0f, 1.0f) + offset;
	}

	void ScreenQuad::Render(ID3DXEffect* pEffect)
	{
		if (pEffect == NULL)
			return;

		HRESULT hr = D3D_OK;

		Camera* pCamera = GetRenderDevice()->GetCamera();
		if (pCamera == NULL)
			return;

		hr = m_pDxDevice->SetFVF(NULL);
		hr = m_pDxDevice->SetVertexDeclaration(m_pVertexDeclaration);


		D3DXMATRIX mWordViewProj,mWOrldView;
		D3DXMatrixInverse(&mWOrldView, NULL, &pCamera->GetProjMatrix());
		D3DXMatrixIdentity(&mWordViewProj);

		hr = pEffect->SetMatrix("worldview",&mWOrldView);
		hr = pEffect->SetMatrix("worldviewprojection",&mWordViewProj);
		hr = pEffect->CommitChanges();

		hr = m_pDxDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_quadVerts, sizeof(Vertex));
	}
}


