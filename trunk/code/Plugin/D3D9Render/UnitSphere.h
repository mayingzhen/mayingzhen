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
		void CreateUnitSphere( int rec, std::vector<Uint16>& indBuff, std::vector<Vector3>& vertBuff );

		void SphereTess(Vector3& v0, Vector3& v1, Vector3& v2, 
			std::vector<Uint16>& indBuff, std::vector<Vector3>& vertBuff);

		void SphereTessR(Vector3& v0, Vector3& v1, Vector3& v2, int depth, 
			std::vector<Uint16>& indBuff, std::vector<Vector3>& vertBuff);

	private:

// 		struct Vertex
// 		{
// 			Vector3 position;
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

