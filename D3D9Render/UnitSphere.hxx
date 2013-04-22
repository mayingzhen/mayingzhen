#include "UnitSphere.h"

namespace ma
{

	void UnitSphere::SphereTessR(Vector3& v0, Vector3& v1, Vector3& v2, int depth, 
		std::vector<Uint16>& indBuff, std::vector<Vector3>& vertBuff)
	{
		if (depth == 0)
		{
			int nVertCount = vertBuff.size();

			vertBuff.push_back(v0);
			indBuff.push_back(nVertCount++);

			vertBuff.push_back(v1);
			indBuff.push_back(nVertCount++);

			vertBuff.push_back(v2);
			indBuff.push_back(nVertCount++);

		}
		else
		{
			Vector3 v01, v12, v02;

			Vec3Normalize(&v01,&(v0 + v1));
			Vec3Normalize(&v12,&(v1 + v2));
			Vec3Normalize(&v02,&(v0 + v2));

			SphereTessR(v0,  v01, v02, depth-1, indBuff, vertBuff);
			SphereTessR(v01,  v1, v12, depth-1, indBuff, vertBuff);
			SphereTessR(v12, v02, v01, depth-1, indBuff, vertBuff);
			SphereTessR(v12,  v2, v02, depth-1, indBuff, vertBuff);
		}
	}


	void UnitSphere::SphereTess(Vector3& v0, Vector3& v1, Vector3& v2, 
		std::vector<Uint16>& indBuff, std::vector<Vector3>& vertBuff)
	{
		int depth;
		Vector3 w0, w1, w2;
		int i, j, k;

		int nVertCount = vertBuff.size();

		depth = 2;
		for (i = 0; i < depth; i++) 
		{
			for (j = 0; i+j < depth; j++) 
			{
				k = depth - i - j;

				{
					w0 = (i * v0 + j * v1 + k * v2) / depth;
					w1 = ((i + 1) * v0 + j * v1 + (k - 1) * v2)
						/ depth;
					w2 = (i * v0 + (j + 1) * v1 + (k - 1) * v2)
						/ depth;
				}

				Vec3Normalize(&w0,&w0);
				Vec3Normalize(&w1,&w1);
				Vec3Normalize(&w2,&w2);

				vertBuff.push_back(w1);
				indBuff.push_back(nVertCount++);

				vertBuff.push_back(w0);
				indBuff.push_back(nVertCount++);

				vertBuff.push_back(w2);
				indBuff.push_back(nVertCount++);
			}
		}
	}

	void UnitSphere::CreateUnitSphere( int rec, std::vector<Uint16>& indBuff, std::vector<Vector3>& vertBuff )
	{
		// vb Ib
	#define X .525731112119133606f
	#define Z .850650808352039932f

	  static Vector3 verts[12] =
	  {
		Vector3(-X, 0, Z),
		Vector3(X, 0, Z),
		Vector3(-X, 0, -Z),
		Vector3(X, 0, -Z),
		Vector3(0, Z, X),
		Vector3(0, Z, -X),
		Vector3(0, -Z, X),
		Vector3(0, -Z, -X),
		Vector3(Z, X, 0),
		Vector3(-Z, X, 0),
		Vector3(Z, -X, 0),
		Vector3(-Z, -X, 0)
	  };

	  static int indices[20][3] =
	  {
		{0, 4, 1},
		{0, 9, 4},
		{9, 5, 4},
		{4, 5, 8},
		{4, 8, 1},
		{8, 10, 1},
		{8, 3, 10},
		{5, 3, 8},
		{5, 2, 3},
		{2, 7, 3},
		{7, 10, 3},
		{7, 6, 10},
		{7, 11, 6},
		{11, 0, 6},
		{0, 1, 6},
		{6, 1, 10},
		{9, 0, 11},
		{9, 11, 2},
		{9, 2, 5},
		{7, 2, 11},
	  };

	  indBuff.clear();
	  vertBuff.clear();

	  for (int i = 19; i >= 0; i--) 
	  {
		Vector3& v0 = verts[indices[i][2]];
		Vector3& v1 = verts[indices[i][1]];
		Vector3& v2 = verts[indices[i][0]];
		SphereTessR(v0, v1, v2, rec, indBuff, vertBuff);
	  }

	}


	void UnitSphere::Render(ID3DXEffect* pEffect)
	{
		if (pEffect == NULL)
			return;

		HRESULT hr = D3D_OK;

// 		Camera* pCamera = GetRenderDevice()->GetCamera();
// 		if (pCamera == NULL)
// 			return;

		hr = m_pd3dDevice->SetVertexDeclaration(m_pVertexDeclaration);
		hr = m_pd3dDevice->SetIndices(m_pIB);
		hr = m_pd3dDevice->SetStreamSource(0,m_pVB,0,sizeof(Vector3));
		hr = m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nVBSise, 0, m_nIBSize);
	}


	void UnitSphere::Init(LPDIRECT3DDEVICE9 pDxDevice)
	{
		m_pd3dDevice = pDxDevice;

		D3DVERTEXELEMENT9 vertexElements[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, 
			D3DDECL_END()
		};

		m_pd3dDevice->CreateVertexDeclaration(vertexElements, &m_pVertexDeclaration);

		std::vector<Uint16> indBuff;
		std::vector<Vector3> vertBuff;
		CreateUnitSphere(1,indBuff,vertBuff);

		m_nVBSise = vertBuff.size();
		m_nIBSize = indBuff.size();


		HRESULT hr = S_OK;

		hr = m_pd3dDevice->CreateVertexBuffer( vertBuff.size() * sizeof( Vector3 ), D3DUSAGE_WRITEONLY,
			0, D3DPOOL_DEFAULT, &m_pVB, NULL );
		assert(SUCCEEDED(hr));
		if(FAILED(hr))
		{
			return /*false*/;
		}

		hr = m_pd3dDevice->CreateIndexBuffer( indBuff.size() * sizeof( indBuff ), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB, NULL );
		assert(SUCCEEDED(hr));
		if(FAILED(hr))
		{
			return /*false*/;
		}

		Vector3* pVerts = NULL;
		UINT* pInds = NULL;

		//allocate vertices
		hr = m_pVB->Lock(0, vertBuff.size() * sizeof( Vector3 ), (void **) &pVerts, 0);
		assert(SUCCEEDED(hr));

		memcpy( pVerts, &vertBuff[0], vertBuff.size()*sizeof(Vector3 ) );

		hr = m_pVB->Unlock();
		assert(SUCCEEDED(hr));

		//allocate indices
		hr = m_pIB->Lock(0, indBuff.size() * sizeof( Uint16 ), (void **) &pInds, 0);
		assert(SUCCEEDED(hr));

		memcpy( pInds, &indBuff[0], sizeof(Uint16)*indBuff.size() );

		hr = m_pIB->Unlock();
		assert(SUCCEEDED(hr));


		return /*SUCCEEDED(hr)*/;
	}

}

