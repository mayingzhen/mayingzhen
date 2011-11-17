#ifndef _GE_MESH_H_
#define _GE_MESH_H_

#include"GESubmesh.h"
//#include"GEVertexBuffer.h"
#include"GEBoneAssignment.h"
#include <cstdlib>
#include <vector>
using namespace std;

class TiXmlElement;
class GESkeleton;
class GEMeshManager;
class GEMorphAnimationKey;

/**
 * 封装静态模型的数据
 */
class GEMesh
{
public:	
	friend class GEMeshManager;

	/// 析构函数
	virtual ~GEMesh(void);
	
	bool CopyDataFrom(const GEMesh* mesh);

	vector<GESubmesh*>& GetSubMeshes();

	/// 绘制此mesh
	HRESULT Render();	
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();


	/// 获取模型宽度，用于包围体
	inline float GetRightDist() { return mRight; }
	inline float GetLeftDist() { return mLeft; }
	/// 获取模型深度，用于包围体
	inline float GetFrontDist() { return mFront; }
	inline float GetBackDist() { return mRear; }
	/// 获取模型高度，用于包围体
	inline float GetTopDist(){ return mTop; }
	inline float GetBottomDist(){ return mBottom; }

	/// 创建影子顶点缓冲区，用于骨骼动画
// 	inline void createShadowVB()
// 	{
// 		for(size_t i=0;i<mSubmeshes.size();++i)
// 		{
// 			mSubmeshes[i]->createShadowVB();
// 		}
// 	}

	/** 根据GESkeleton当前帧各骨头的变换更新mesh里的顶点缓冲区 */
	void updateFromSkeleton(GESkeleton* skeleton);

	/** 从Morph Animation中更新Mesh */
	void updateFromMorphAnimation(GEMorphAnimationKey* currentPose);

	/** 将 .GEM.TXT 文件转变成 .GEM 文件 */
	//static bool MakeGEMTXT2GEM(const string& file);
	/** 将文本格式的 .X 文件转变成 .GEM 文件 */
	//static bool MakeXTXT2GEM(const string& file);

	//bool parseX();

protected:
	vector<GESubmesh*> mSubmeshes;				///< 该mesh的所有submesh

	float mLeft, mRight, mTop, mBottom, mFront, mRear;

	string mSkeletonFileName;	///< mesh使用的骨架文件（如果有骨骼动画的话）

// 	bool parseXML();			///< 解析OGRE的xml模型格式，在InitDeviceObjects()中使用
// 	bool loadSubMeshes(TiXmlElement* element);
// 	GESubmesh* loadSubmesh(TiXmlElement* element);
// 
// 	bool parseFromData(std::string sFileName);
// 	bool loadSubMeshesFromData(BYTE* pData);
// 	GESubmesh* loadSubmeshFromData(BYTE* pData);

	
	//bool parseGEM();

private:
	string mMeshFile;
	bool mIsInited;
	bool mIsRestored;

public:
	/// 构造函数，实际填充数据是在InitDeviceObjects()的时候
	inline GEMesh(const string& meshFile) 
		: mMeshFile(meshFile),
		mLeft(FLT_MAX),
		mRight(-FLT_MAX),
		mFront(-FLT_MAX),
		mRear(FLT_MAX),
		mBottom(FLT_MAX),
		mTop(-FLT_MAX),
		mIsInited(false),
		mIsRestored(false)
	{
	}
};

inline vector<GESubmesh*>& GEMesh::GetSubMeshes()
{
	return mSubmeshes;
}

#endif