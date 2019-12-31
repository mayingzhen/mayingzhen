#include "UnitSphere.h"

namespace ma
{
	static Renderable* gpSphere = NULL;

	void UnitSphere::Render(Technique* pTech,const Vector3& vPos,float fRadius,RenderCommand* pComomnd)
	{
		Matrix4 matTras = Matrix4::IDENTITY;  
		matTras.setTrans(vPos);
		matTras.setScale(Vector3(fRadius ,fRadius ,fRadius));

		gpSphere->SetWorldMatrix(matTras);

		gpSphere->PreRender(pTech);
		
		gpSphere->Render(pTech, pComomnd);
	}

	void SphereTessR(Vector3& v0, Vector3& v1, Vector3& v2, int depth, vector<uint16_t>& indBuff, vector<Vector3>& vertBuff)
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

		vector<uint16_t> indBuff;
		vector<Vector3> vertBuff;

		for (int i = 19; i >= 0; i--) 
		{
			Vector3& v0 = verts[indices[i][2]];
			Vector3& v1 = verts[indices[i][1]];
			Vector3& v2 = verts[indices[i][0]];
			SphereTessR(v0, v1, v2, 2, indBuff, vertBuff);
		}

		gpSphere = new Renderable();

		gpSphere->m_pVertexBuffer = GetRenderSystem()->CreateVertexBuffer((uint8_t*)&vertBuff[0],vertBuff.size() * sizeof(Vector3),sizeof(Vector3));
		gpSphere->m_pIndexBuffer = GetRenderSystem()->CreateIndexBuffer((uint8_t*)&indBuff[0],vertBuff.size() * sizeof(uint16_t),sizeof(uint16_t));
		gpSphere->m_ePrimitiveType = PRIM_TRIANGLELIST;

		return;
	}

	void UnitSphere::Shoutdown()
	{
		SAFE_DELETE(gpSphere);		
	}
}

