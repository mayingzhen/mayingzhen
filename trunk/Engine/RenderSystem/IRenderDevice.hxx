#include "IRenderDevice.h"

namespace ma
{
	IRenderDevice* g_pRenderDevice = NULL;

	void SetRenderDevice(IRenderDevice* pRenderDevice)
	{
		g_pRenderDevice = pRenderDevice;
	}

	IRenderDevice* GetRenderDevice()
	{
		return g_pRenderDevice;
	}

	void IRenderDevice::DrawBox(const Matrix4x4& wordMat,const Vector3& size, Uint32 color)
	{
		Vector3 vScale;
		Quaternion qRot;
		Vector3 vPos;
		MatrixDecompose(&vScale,&qRot,&vPos,&wordMat);

		float fHalfWidth = 0.5f * vScale.x * size.x;
		float fHalfHeight = 0.5f * vScale.y * size.y;
		float fHalfDepth = 0.5f * vScale.z * size.z;

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
			QuaternionTransformVector(&arrVec[nCnt],&arrVec[nCnt],&qRot);
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

	void IRenderDevice::DrawCircle(UINT nbSegments, const Matrix4x4& world, 
		Uint32 dwColor, float radius, bool semicircle)
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

			Vector3 p0,p1;
			Vector3 pTemp0(radius * sinf(angle0), radius * cosf(angle0), 0.0f);
			Vector3 pTemp1(radius * sinf(angle1), radius * cosf(angle1), 0.0f);
			Vec3TransformCoord(&p0, &pTemp0, &world);
			Vec3TransformCoord(&p1, &pTemp1, &world);

			DrawLine(p0, p1, dwColor);
		}
	}

	void IRenderDevice::DrawWireSphere(const Matrix4x4& world,float fRadius, Uint32 dwColor)
	{
		Vector3 c0 = *(Vector3*)&world._11;
		Vector3 c1 = *(Vector3*)&world._21;
		Vector3 c2 = *(Vector3*)&world._31;	

		DrawCircle(20, world, dwColor, fRadius);

		Matrix4x4 matrTemp = world;
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