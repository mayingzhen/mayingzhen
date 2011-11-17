#pragma once

#include<vector>
using namespace std;

/**
 * �����ͷ�Ͷ���İ󶨹�ϵ����Ȩ��.
 * һ����������ܶ����ͷ��Ӱ�죬���ռ��㶥�����꼰�����ʱ��Ѷ����ͷ��Ӱ���Ȩƽ����
 */
class GEBoneAssignment
{
public:
	friend class GEMesh;

	typedef pair<unsigned short, pair<unsigned short, float> > Assignment;
	typedef pair<unsigned short, float> BoneIndexWeight;

	GEBoneAssignment(void);
	virtual ~GEBoneAssignment(void);

	/**
	 * ��ù��м������㣭��ͷ��Ȩ�ض�
	 */
	inline size_t GetNumberAssignments() const
	{ return mMap.size(); }


	/**
	 * ���һ�����㣭��ͷ��Ȩ�ض�
	 */
	inline void AddAssignment(unsigned short vertexIndex, unsigned short boneIndex, float weight)
	{
		mMap.push_back(Assignment(vertexIndex, BoneIndexWeight(boneIndex, weight)));
	}

	/**
	 * ��õ�һ�������㣬��ͷ���������ԡ�����ö�����еĶ��㣭��ͷ��������
	 */
	inline const Assignment& GetFirstAssignment()
	{
		itr = mMap.begin();
		return *itr;
	}

	/**
	 * �ж��Ƿ��Ѿ�ö�������еĶ��㣭��ͷ��������
	 */
	inline bool IsEnd() const
	{
		return itr == mMap.end();
	}

	/**
	 * �����һ�������㣬��ͷ���������ԡ�����ö�����еĶ��㣭��ͷ��������
	 */
	inline const Assignment& GetNextAssignment()
	{
        ++itr;
		return *itr;
	}

protected:
	vector<Assignment> mMap;	///< ����������ż����Ӧ�Ĺ�ͷ��Ȩ��
	vector<Assignment>::const_iterator itr;
};
