#include "LineRender.h"
#include "../Material/ParameterManager.h"

namespace ma
{
	static LineRender* gpLineRender = NULL;

	LineRender*	GetLineRender()
	{
		return gpLineRender;
	}

	void SetLineRender(LineRender* pLineRender)
	{
		gpLineRender = pLineRender;
	}

	void LineRender::Init()
	{
		m_pLinTech =  new Technique("Line","default","default","COLOR");

		MaterialParameter* pParam = m_pLinTech->GetParameter("u_worldViewProjectionMatrix");
		GetParameterManager()->SetParameterAutoBinding(pParam,VIEW_PROJECTION_MATRIX);
		
		VertexDeclaration* pVertexDec = GetRenderDevice()->CreateVertexDeclaration();
		pVertexDec->AddElement(0,0,DT_FLOAT3,DU_POSITION,0);
		pVertexDec->AddElement(0,12,DT_FLOAT4,DU_COLOR,0);

		m_pMeshBatch = new MeshBatch(pVertexDec, PRIM_LINELIST, /*pEffect, */true, 1024);
	}

	void LineRender::ShutDown()
	{
		SAFE_DELETE(m_pMeshBatch);
		SAFE_DELETE(m_pLinTech);
	}


	void LineRender::DrawLine(const Vector3& p0,const Vector3& p1,ColourValue dwColor)
	{
		LineVertex v[2];
		v[0].pos = p0;
		v[0].col = dwColor;
		v[1].pos = p1;
		v[1].col = dwColor;
	
		int index = GetRenderSystem()->GetThreadList();
		std::vector<LineVertex>& arrLineVertex = m_arrLineVertex[index];

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
		m_arrLineVertex[index].clear();
	}

	void LineRender::Render()
	{
		m_pMeshBatch->Reset();

		int index = GetRenderSystem()->CurThreadProcess();
		std::vector<LineVertex>& arrLineVertex = m_arrLineVertex[index];

		for (UINT i = 0; i < arrLineVertex.size(); i += 2)
		{
			LineVertex v[2];
			v[0] = arrLineVertex[i];
			v[1] = arrLineVertex[i + 1];
			Uint16 index[2] = {0,1};

			m_pMeshBatch->Add(v,2,index,2);
		}

		m_pMeshBatch->Render(m_pLinTech);	
	}
	
}

