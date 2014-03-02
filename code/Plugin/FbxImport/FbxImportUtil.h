#ifndef  _FbxImportUtil__H__
#define  _FbxImportUtil__H__

//using namespace fbxsdk_2013_2;

namespace ma
{
	inline Vector3 ToMaUnit(const FbxVector4& vFbx)
	{
		return Vector3((float)vFbx[0],(float)vFbx[1],(float)vFbx[2]);
	}

	inline Vector3 ToMaUnit(const FbxDouble3 vFbx)
	{
		return Vector3((float)vFbx[0],(float)vFbx[1],(float)vFbx[2]);
	}

	inline Quaternion ToMaUnit(const FbxQuaternion& qFbx)
	{
		return Quaternion((float)qFbx[0],(float)qFbx[1],(float)qFbx[2],(float)qFbx[3]);
	}

	inline Vector4 ToMaUnit(const FbxColor& cFbx)
	{
		return Vector4((float)cFbx.mRed,(float)cFbx.mGreen,(float)cFbx.mBlue,(float)cFbx.mAlpha);
	}

	inline NodeTransform ToMaUnit(const FbxMatrix& vFbx)
	{
		KFbxXMatrix matrix4x4 = *(KFbxXMatrix*)(double*)&vFbx;

		FbxVector4 LocalLinkT = matrix4x4.GetT();
		FbxQuaternion LocalLinkQ = matrix4x4.GetQ();
		FbxVector4 LocalLinkS = matrix4x4.GetS();

		NodeTransform tsf;
		tsf.m_fScale = ToMaUnit(LocalLinkS).x;
		tsf.m_qRot = ToMaUnit(LocalLinkQ);
		tsf.m_vPos = ToMaUnit(LocalLinkT);

		return tsf;
	}

	inline void ToMaUnit(const FbxVector4& vFbx,Vector4& maVector4)
	{
		maVector4.x = (float)vFbx[0];
		maVector4.y = (float)vFbx[1];
		maVector4.z = (float)vFbx[2];
		maVector4.w = (float)vFbx[3];
	}
}


#endif
