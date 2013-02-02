#ifndef  _FbxImportUtil__H__
#define  _FbxImportUtil__H__

namespace ma
{
	inline D3DXVECTOR3 ToMaUnit(FbxVector4 vFbx)
	{
		return D3DXVECTOR3((float)vFbx[0],(float)vFbx[1],(float)vFbx[2]);
	}
}


#endif
