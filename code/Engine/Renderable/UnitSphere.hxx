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

	void SphereTessR(Vector3& v0, Vector3& v1, Vector3& v2, int depth, vector<uint16>& indBuff, vector<Vector3>& vertBuff)
	{
		if (depth == 0)
		{
			Vector3 vert;

			int nVertCount = vertBuff.size();

			vert = v0;
			vertBuff.push_back(vert);
			indBuff.push_back(nVertCount++);

			vert = v1;
			vertBuff.push_back(vert);
			indBuff.push_back(nVertCount++);

			vert = v2;
			vertBuff.push_back(vert);
			indBuff.push_back(nVertCount++);

		}
		else
		{
			Vector3 v01, v12, v02;

			v01 = (v0 + v1).normalisedCopy();
			v12 = (v1 + v2).normalisedCopy();
			v02 = (v0 + v2).normalisedCopy();

			SphereTessR(v0,  v01, v02, depth-1, indBuff, vertBuff);
			SphereTessR(v01,  v1, v12, depth-1, indBuff, vertBuff);
			SphereTessR(v12, v02, v01, depth-1, indBuff, vertBuff);
			SphereTessR(v12,  v2, v02, depth-1, indBuff, vertBuff);
		}
	}

	void SphereTess(Vector3& v0, Vector3& v1, Vector3& v2, vector<uint16>& indBuff, vector<Vector3>& vertBuff)
	{
		int depth;
		Vector3 w0, w1, w2;
		int i, j, k;

		Vector3 vert;

		int nVertCount = vertBuff.size();

		depth = 2;
		for (i = 0; i < depth; i++) 
		{
			for (j = 0; i+j < depth; j++) 
			{
				k = depth - i - j;

				{
					w0 = ((float)i * v0 + (float)j * v1 + (float)k * v2) / (float)depth;
					w1 = ((float)(i + 1) * v0 + (float)j * v1 + ((float)k - 1) * v2)
						/ (float)depth;
					w2 = ((float)i * v0 + ((float)j + 1) * v1 + ((float)k - 1) * v2)
						/ (float)depth;
				}

				w0.normalise();
				w1.normalise();
				w2.normalise();

				vert = w1;
				vertBuff.push_back(vert);
				indBuff.push_back(nVertCount++);

				vert = w0;
				vertBuff.push_back(vert);
				indBuff.push_back(nVertCount++);

				vert = w2;
				vertBuff.push_back(vert);
				indBuff.push_back(nVertCount++);
			}
		}
	}

	void UnitSphere::Init()
	{
		const float X = .525731112119133606f;
		const float Z = .850650808352039932f;

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

		vector<uint16> indBuff;
		vector<Vector3> vertBuff;

		for (int i = 19; i >= 0; i--) 
		{
			Vector3& v0 = verts[indices[i][2]];
			Vector3& v1 = verts[indices[i][1]];
			Vector3& v2 = verts[indices[i][0]];
			SphereTessR(v0, v1, v2, 2, indBuff, vertBuff);
		}

		gpSphere = new Renderable();
		
		VertexElement element[1];
		element[0] = VertexElement(0,0,DT_FLOAT3,DU_POSITION,0);
		//gpSphere->m_pDeclaration = GetRenderSystem()->CreateVertexDeclaration(element,1);
		gpSphere->m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer((uint8*)&vertBuff[0],vertBuff.size() * sizeof(Vector3),sizeof(Vector3));
		gpSphere->m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer((uint8*)&indBuff[0],vertBuff.size() * sizeof(uint16),sizeof(uint16));
		gpSphere->m_ePrimitiveType = PRIM_TRIANGLELIST;

		gpSphere->m_pSubMeshData = CreateSubMeshData();
		gpSphere->m_pSubMeshData->m_nIndexStart = 0;
		gpSphere->m_pSubMeshData->m_nIndexCount = indBuff.size();
		gpSphere->m_pSubMeshData->m_nVertexStart = 0;
		gpSphere->m_pSubMeshData->m_nVertexCount = vertBuff.size();

		return;
	}

	void UnitSphere::Shoutdown()
	{
		SAFE_DELETE(gpSphere);		
	}
}

