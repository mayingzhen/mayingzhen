#ifndef  C_SCREENQUAD__H__
#define  C_SCREENQUAD__H__


namespace ma
{
	class ScreenQuad
	{
	public:
		void Init(LPDIRECT3DDEVICE9 pDxDevice);
		
		void Render(ID3DXEffect* pEffect);

	private:
		struct Vertex
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 texCoords;
		};

		IDirect3DVertexDeclaration9*	m_pVertexDeclaration;

		Vertex							m_quadVerts[4];

		LPDIRECT3DDEVICE9				m_pDxDevice;
	};

}

#endif


