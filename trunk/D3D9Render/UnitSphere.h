#ifndef  _UnitSphere__H__
#define  _UnitSphere__H__

namespace ma
{
	class UnitSphere
	{
	public:
		void Init(LPDIRECT3DDEVICE9 pDxDevice);

		void Render(ID3DXEffect* pEffect);

	private:
		void CreateUnitSphere( int rec, std::vector<xmUint16>& indBuff, std::vector<D3DXVECTOR3>& vertBuff );

		void SphereTess(D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, 
			std::vector<xmUint16>& indBuff, std::vector<D3DXVECTOR3>& vertBuff);

		void SphereTessR(D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, int depth, 
			std::vector<xmUint16>& indBuff, std::vector<D3DXVECTOR3>& vertBuff);

	private:

// 		struct Vertex
// 		{
// 			D3DXVECTOR3 position;
// 		};

		//Vertex							m_quadVerts[4];
		

		IDirect3DVertexBuffer9*			m_pVB;
		IDirect3DIndexBuffer9*			m_pIB;
		int								m_nVBSise;
		int								m_nIBSize;

		IDirect3DVertexDeclaration9*	m_pVertexDeclaration;

		LPDIRECT3DDEVICE9				m_pd3dDevice;
	};
}


#endif

