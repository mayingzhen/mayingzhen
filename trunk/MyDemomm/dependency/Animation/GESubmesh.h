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
 * mesh根据材质的不同分为submesh，每个submesh有自己的材质对象和索引缓冲区，并可以选择
 * 使用自己的顶点缓冲区或mesh的公共顶点缓冲区。
 */
class GESubmesh
{
public:
	friend class GEMesh;

	inline GESubmesh(void) : mVB(NULL),mIB(NULL),mBoneAssignment(NULL)
	{
	}
	
	virtual ~GESubmesh(void);
	
	/// 设置顶点缓冲区
	//inline void setVertexBuffer(GEVertexBuffer* p) { mVB=p; }
	/// 获得顶点缓冲区
	inline IDirect3DVertexBuffer9* getVertexBuffer() { return mVB; }
	inline IDirect3DIndexBuffer9*  getIndexBuffer()	 { return mIB; }

	/// 绘制该submesh
	void Render();

	/// 根据骨架的姿态更新顶点缓冲区
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
	IDirect3DVertexBuffer9* mVB;				///< 顶点缓冲区
	IDirect3DIndexBuffer9*  mIB;				///< 索引缓冲区
	GEBoneAssignment* mBoneAssignment;	///< 顶点骨头映射
	string         mMaterial;			///< 材质对象
	GEMesh*		   mParent;				///< 所属的mesh
};

#endif