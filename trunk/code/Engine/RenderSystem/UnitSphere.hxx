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
					w0 = (i * v0 + j * v1 + k * v2) / (float)depth;
					w1 = ((i + 1) * v0 + j * v1 + (k - 1) * v2)
						/ (float)depth;
					w2 = (i * v0 + (j + 1) * v1 + (k - 1) * v2)
						/ (float)depth;
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


	void UnitSphere::Render(Material* pMaterial)
	{
		m_pRenderable->m_pMaterial;

		GetRenderDevice()->DrawRenderable(m_pRenderable);
	}


	void UnitSphere::Init()
	{
		VertexDeclaration* pVertexDec = GetRenderDevice()->CreateVertexDeclaration(); 
		pVertexDec->AddElement(0,0,DT_FLOAT3,DU_POSITION,0);
		pVertexDec->Active();

		std::vector<Uint16> arrIndex;
		std::vector<Vector3> arrVertex;
		CreateUnitSphere(1,arrIndex,arrVertex);

		// Init Renderable
		m_pRenderable = new Renderable();

		VertexBuffer* pVertexs = GetRenderDevice()->CreateVertexBuffer(&arrVertex[0],sizeof(Vector3) * arrVertex.size(),sizeof(Vector3));
		pVertexs->Active();

		IndexBuffer* pIndexs = GetRenderDevice()->CreateIndexBuffer(&arrIndex[0],sizeof(Uint16) * arrIndex.size());
		pIndexs->Active();

		m_pRenderable->m_pDeclaration = pVertexDec;
		m_pRenderable->m_ePrimitiveType = PRIM_TRIANGLESTRIP;
		m_pRenderable->m_pIndexBuffer = pIndexs;
		m_pRenderable->m_pVertexBuffers = pVertexs;
		m_pRenderable->m_nVertexStart = 0;
		m_pRenderable->m_nVertexCount = 4;
		m_pRenderable->m_nIndexStart = 0;
		m_pRenderable->m_nIndexCount = 4;


		return;
	}

}

