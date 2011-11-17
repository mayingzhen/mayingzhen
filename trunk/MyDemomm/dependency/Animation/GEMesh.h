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
 * ��װ��̬ģ�͵�����
 */
class GEMesh
{
public:	
	friend class GEMeshManager;

	/// ��������
	virtual ~GEMesh(void);
	
	bool CopyDataFrom(const GEMesh* mesh);

	vector<GESubmesh*>& GetSubMeshes();

	/// ���ƴ�mesh
	HRESULT Render();	
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();


	/// ��ȡģ�Ϳ�ȣ����ڰ�Χ��
	inline float GetRightDist() { return mRight; }
	inline float GetLeftDist() { return mLeft; }
	/// ��ȡģ����ȣ����ڰ�Χ��
	inline float GetFrontDist() { return mFront; }
	inline float GetBackDist() { return mRear; }
	/// ��ȡģ�͸߶ȣ����ڰ�Χ��
	inline float GetTopDist(){ return mTop; }
	inline float GetBottomDist(){ return mBottom; }

	/// ����Ӱ�Ӷ��㻺���������ڹ�������
// 	inline void createShadowVB()
// 	{
// 		for(size_t i=0;i<mSubmeshes.size();++i)
// 		{
// 			mSubmeshes[i]->createShadowVB();
// 		}
// 	}

	/** ����GESkeleton��ǰ֡����ͷ�ı任����mesh��Ķ��㻺���� */
	void updateFromSkeleton(GESkeleton* skeleton);

	/** ��Morph Animation�и���Mesh */
	void updateFromMorphAnimation(GEMorphAnimationKey* currentPose);

	/** �� .GEM.TXT �ļ�ת��� .GEM �ļ� */
	//static bool MakeGEMTXT2GEM(const string& file);
	/** ���ı���ʽ�� .X �ļ�ת��� .GEM �ļ� */
	//static bool MakeXTXT2GEM(const string& file);

	//bool parseX();

protected:
	vector<GESubmesh*> mSubmeshes;				///< ��mesh������submesh

	float mLeft, mRight, mTop, mBottom, mFront, mRear;

	string mSkeletonFileName;	///< meshʹ�õĹǼ��ļ�������й��������Ļ���

// 	bool parseXML();			///< ����OGRE��xmlģ�͸�ʽ����InitDeviceObjects()��ʹ��
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
	/// ���캯����ʵ�������������InitDeviceObjects()��ʱ��
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