#ifndef _GE_SUBMESH_H_
#define _GE_SUBMESH_H_

//#include "GEVertexBuffer.h"
#include <d3dx9.h>

class GESkeleton;
class GEMesh;
class GEIndexBuffer;
class GEBoneAssignment;
class GEMorphAnimationKey;

#include<string>
using namespace std;

//#include "GEGraphicLayer.h"

//#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
//#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

/**
 * mesh���ݲ��ʵĲ�ͬ��Ϊsubmesh��ÿ��submesh���Լ��Ĳ��ʶ����������������������ѡ��
 * ʹ���Լ��Ķ��㻺������mesh�Ĺ������㻺������
 */
class GESubmesh
{
public:
	friend class GEMesh;

	inline GESubmesh(void) : mVB(NULL),mIB(NULL),mBoneAssignment(NULL)
	{
	}
	
	virtual ~GESubmesh(void);
	
	/// ���ö��㻺����
	//inline void setVertexBuffer(GEVertexBuffer* p) { mVB=p; }
	/// ��ö��㻺����
	inline IDirect3DVertexBuffer9* getVertexBuffer() { return mVB; }
	inline IDirect3DIndexBuffer9*  getIndexBuffer()	 { return mIB; }

	/// ���Ƹ�submesh
	void Render();

	/// ���ݹǼܵ���̬���¶��㻺����
	void updateFromSkeleton(GESkeleton* skeleton);
	void updateFromMorphAnimation(GEMorphAnimationKey* currentPose);
	
	void InvalidateDeviceObjects();
	void RestoreDeviceObjects();
	void DeleteDeviceObjects();


	//inline float GetXMin() const	{ return mVB->GetXMin(); }
	//inline float GetXMax() const	{ return mVB->GetXMax(); }
	//inline float GetYMin() const	{ return mVB->GetYMin(); }
	//inline float GetYMax() const	{ return mVB->GetYMax(); }
	//inline float GetZMin() const	{ return mVB->GetZMin(); }
	//inline float GetZMax() const	{ return mVB->GetZMax(); }


protected:
	IDirect3DVertexBuffer9* mVB;				///< ���㻺����
	IDirect3DIndexBuffer9*  mIB;				///< ����������
	GEBoneAssignment* mBoneAssignment;	///< �����ͷӳ��
	string         mMaterial;			///< ���ʶ���
	GEMesh*		   mParent;				///< ������mesh
};

#endif