#include "IRender.h"

namespace ma
{
	IRender* g_pRender = NULL;

	void SetRender(IRender* pRender)
	{
		g_pRender = pRender;
	}

	IRender* GetRender()
	{
		return g_pRender;
	}

	void IRender::BeginRender()
	{
		//m_vGeneral.clear();
		//m_vTransparent.clear();
	}

	void IRender::EndRender()
	{
		
	}


	void IRender::DrawMesh(const D3DXMATRIX* pWordMat,const IRendMesh* pMesh,const IRendTexture* pTexture)
	{
		IRenderItem renderItem;
		renderItem.m_pMatWorld = pWordMat;
		//renderItem.m_pMesh = pMesh;
		//renderItem.m_pTex = pTexture;
		//m_vGeneral.push_back(renderItem);
	}

	void IRender::DrawBox(const D3DXMATRIX& wordMat,const D3DXVECTOR3& size, xmUint32 color)
	{
		D3DXVECTOR3 vScale;
		D3DXQUATERNION qRot;
		D3DXVECTOR3 vPos;
		D3DXMatrixDecompose(&vScale,&qRot,&vPos,&wordMat);

		float fHalfWidth = 0.5f * vScale.x * size.x;
		float fHalfHeight = 0.5f * vScale.y * size.y;
		float fHalfDepth = 0.5f * vScale.z * size.z;

		D3DXVECTOR3 arrVec[]=
		{
			D3DXVECTOR3(-fHalfWidth,-fHalfHeight,fHalfDepth),
			D3DXVECTOR3(fHalfWidth,-fHalfHeight,fHalfDepth),
			D3DXVECTOR3(fHalfWidth,fHalfHeight,fHalfDepth),
			D3DXVECTOR3(-fHalfWidth,fHalfHeight,fHalfDepth),
			D3DXVECTOR3(-fHalfWidth,-fHalfHeight,-fHalfDepth),
			D3DXVECTOR3(fHalfWidth,-fHalfHeight,-fHalfDepth),
			D3DXVECTOR3(fHalfWidth,fHalfHeight,-fHalfDepth),
			D3DXVECTOR3(-fHalfWidth,fHalfHeight,-fHalfDepth),
		};

		for (UINT nCnt = 0; nCnt < 8; ++nCnt)
		{
			maQuaternionTransformVector(&arrVec[nCnt],&arrVec[nCnt],&qRot);
			arrVec[nCnt] += vPos;
		}

		for (UINT nCnt = 0; nCnt < 4; ++nCnt)
		{
			UINT nID2 = (nCnt + 1) % 4;
			DrawLine(arrVec[nCnt],arrVec[nID2],color);
			DrawLine(arrVec[nCnt+4],arrVec[nID2+4],color);
			DrawLine(arrVec[nCnt],arrVec[nCnt+4],color);
		}
	}

	void IRender::DrawCircle(UINT nbSegments, const D3DXMATRIX& world, 
		xmUint32 dwColor, float radius, bool semicircle)
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

			D3DXVECTOR3 p0,p1;
			D3DXVECTOR3 pTemp0(radius * sinf(angle0), radius * cosf(angle0), 0.0f);
			D3DXVECTOR3 pTemp1(radius * sinf(angle1), radius * cosf(angle1), 0.0f);
			D3DXVec3TransformCoord(&p0, &pTemp0, &world);
			D3DXVec3TransformCoord(&p1, &pTemp1, &world);

			DrawLine(p0, p1, dwColor);
		}
	}

	void IRender::DrawWireSphere(const D3DXMATRIX& world,float fRadius, xmUint32 dwColor)
	{
		D3DXVECTOR3 c0 = *(D3DXVECTOR3*)&world._11;
		D3DXVECTOR3 c1 = *(D3DXVECTOR3*)&world._21;
		D3DXVECTOR3 c2 = *(D3DXVECTOR3*)&world._31;	

		DrawCircle(20, world, dwColor, fRadius);

		D3DXMATRIX matrTemp = world;
		matrTemp._11 = c1.x; matrTemp._12 = c1.y; matrTemp._13 = c1.z;
		matrTemp._21 = c2.x; matrTemp._22 = c2.y; matrTemp._23 = c2.z;
		matrTemp._31 = c0.x; matrTemp._32 = c0.y; matrTemp._33 = c0.z;
		DrawCircle(20, matrTemp, dwColor, fRadius);

		matrTemp._11 = c2.x; matrTemp._12 = c2.y; matrTemp._13 = c2.z;
		matrTemp._21 = c0.x; matrTemp._22 = c0.y; matrTemp._23 = c0.z;
		matrTemp._31 = c1.x; matrTemp._32 = c1.y; matrTemp._33 = c1.z;
		DrawCircle(20, matrTemp, dwColor, fRadius);
	}

}
