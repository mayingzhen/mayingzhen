#include "LineRender.h"
#include "../Material/ParameterManager.h"

namespace ma
{
	struct LineVertex
	{
		Vector3			pos;
		UINT			col;
	};

	static RefPtr<MeshBatch> gpMeshBatch;

	static RefPtr<Technique> gpLinTech;

	static std::vector<LineVertex> garrLineVertex[2];

	void LineRender::Init()
	{
		gpLinTech = CreateTechnique("Line","line","line","");
		
		VertexElement element[2];
		element[0] = VertexElement(0,0,DT_FLOAT3,DU_POSITION,0);
		element[1] = VertexElement(0,12,DT_COLOR,DU_COLOR,0);
		RefPtr<VertexDeclaration> pVertexDec = GetRenderSystem()->CreateVertexDeclaration(element,2);

		gpMeshBatch = new MeshBatch(pVertexDec.get(), PRIM_LINELIST, true, 1024);
	}

	void LineRender::ShutDown()
	{
		gpLinTech = NULL;
		gpMeshBatch = NULL;
		garrLineVertex[0].clear();
		garrLineVertex[1].clear();
	}


	void LineRender::DrawLine(const Vector3& p0,const Vector3& p1,ColourValue dwColor)
	{
		LineVertex v[2];
		v[0].pos = p0;
		v[0].col = dwColor.getAsARGB();
		v[1].pos = p1;
		v[1].col = dwColor.getAsARGB();
	
		int index = GetRenderSystem()->CurThreadFill();
		std::vector<LineVertex>& arrLineVertex = garrLineVertex[index];

		arrLineVertex.push_back(v[0]);
		arrLineVertex.push_back(v[1]);
	}

	void LineRender::DrawCapsule(const Matrix4& wordMat,float fRadius, float Height, ColourValue color)
	{

	}

	void LineRender::DrawBox(const Matrix4& wordMat,const Vector3& size, ColourValue dwcolor)
	{
		float fHalfWidth = 0.5f  * size.x;
		float fHalfHeight = 0.5f  * size.y;
		float fHalfDepth = 0.5f * size.z;

		Vector3 arrVec[]=
		{
			Vector3(-fHalfWidth,-fHalfHeight,fHalfDepth),
			Vector3(fHalfWidth,-fHalfHeight,fHalfDepth),
			Vector3(fHalfWidth,fHalfHeight,fHalfDepth),
			Vector3(-fHalfWidth,fHalfHeight,fHalfDepth),
			Vector3(-fHalfWidth,-fHalfHeight,-fHalfDepth),
			Vector3(fHalfWidth,-fHalfHeight,-fHalfDepth),
			Vector3(fHalfWidth,fHalfHeight,-fHalfDepth),
			Vector3(-fHalfWidth,fHalfHeight,-fHalfDepth),
		};

		for (UINT nCnt = 0; nCnt < 8; ++nCnt)
		{
			arrVec[nCnt] = wordMat * arrVec[nCnt];
		}

		for (UINT nCnt = 0; nCnt < 4; ++nCnt)
		{
			UINT nID2 = (nCnt + 1) % 4;
			DrawLine(arrVec[nCnt],arrVec[nID2],dwcolor);
			DrawLine(arrVec[nCnt+4],arrVec[nID2+4],dwcolor);
			DrawLine(arrVec[nCnt],arrVec[nCnt+4],dwcolor);
		}
	}

	void LineRender::DrawCircle(UINT nbSegments, const Matrix4& world, 
		ColourValue dwColor, float radius, bool semicircle)
	{
		static const float NxTwoPiF32	= 6.28318530717958647692f;

		float step = NxTwoPiF32/float(nbSegments);
		float segs = float(nbSegments);
		if(semicircle)
		{
			segs /= 2;
		}

		for(int i = 0; i < segs; i++)
		{
			int j = i+1;
			if(j == nbSegments)	
				j=0;

			float angle0 = float(i) * step;
			float angle1 = float(j) * step;

			Vector3 p0 = world * Vector3(radius * sinf(angle0), radius * cosf(angle0), 0.0f);
			Vector3 p1 = world * Vector3(radius * sinf(angle1), radius * cosf(angle1), 0.0f);

			DrawLine(p0, p1, dwColor);
		}
	}

	void LineRender::DrawWireSphere(const Matrix4& world,float fRadius, ColourValue dwColor)
	{
		DrawCircle(20, world, dwColor, fRadius);

		Matrix4 matTemp = world.transpose();

		Vector3 c0 = Vector3(matTemp[0]);
		Vector3 c1 = Vector3(matTemp[1]);
		Vector3 c2 = Vector3(matTemp[2]);
		
		memcpy(matTemp[0],&c1,sizeof(Vector3));
		memcpy(matTemp[1],&c2,sizeof(Vector3));
		memcpy(matTemp[2],&c0,sizeof(Vector3));

		DrawCircle(20, matTemp.transpose(), dwColor, fRadius);

		memcpy(matTemp[0],&c2,sizeof(Vector3));
		memcpy(matTemp[1],&c0,sizeof(Vector3));
		memcpy(matTemp[2],&c1,sizeof(Vector3));

		DrawCircle(20, matTemp.transpose(), dwColor, fRadius);
	}

	void LineRender::DrawTransform(const Transform& tsf, float fCenterScale/* = 1.0f*/, float fAxisScale/* = 1.0f*/)
	{
		Vector3 vX,vY,vZ;
		tsf.m_qRot.ToAxes(vX,vY,vZ);

		vX = vX * fAxisScale;
		vY = vY * fAxisScale;
		vZ = vZ * fAxisScale;

		Vector3 vCenter = tsf.m_vPos * fCenterScale;

		DrawLine(tsf.m_vPos,tsf.m_vPos + vX,ColourValue::Red);
		DrawLine(tsf.m_vPos,tsf.m_vPos + vY,ColourValue::Green);
		DrawLine(tsf.m_vPos,tsf.m_vPos + vZ,ColourValue::Blue);
	}

	void LineRender::OnFlushFrame()
	{
		int index = GetRenderSystem()->CurThreadFill();
		garrLineVertex[index].clear();
	}

	void LineRender::Render()
	{
		if (gpMeshBatch == NULL)
			return;

		gpMeshBatch->Reset();

		int index = GetRenderSystem()->CurThreadProcess();
		std::vector<LineVertex>& arrLineVertex = garrLineVertex[index];

		if (arrLineVertex.empty())
			return;

		for (UINT i = 0; i < arrLineVertex.size(); i += 2)
		{
			LineVertex v[2];
			v[0] = arrLineVertex[i];
			v[1] = arrLineVertex[i + 1];
			uint16 index[2] = {0,1};

			gpMeshBatch->Add(v,2,index,2);
		}

		gpMeshBatch->Render(gpLinTech.get());	
	}
	
}

