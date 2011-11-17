#pragma once

#include<vector>
using namespace std;

/**
 * 保存骨头和顶点的绑定关系及其权重.
 * 一个顶点可以受多个骨头的影响，最终计算顶点坐标及法向的时候把多个骨头的影响加权平均。
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
	 * 获得共有几个顶点－骨头－权重对
	 */
	inline size_t GetNumberAssignments() const
	{ return mMap.size(); }


	/**
	 * 添加一个顶点－骨头－权重对
	 */
	inline void AddAssignment(unsigned short vertexIndex, unsigned short boneIndex, float weight)
	{
		mMap.push_back(Assignment(vertexIndex, BoneIndexWeight(boneIndex, weight)));
	}

	/**
	 * 获得第一个《顶点，骨头和索引》对。用于枚举所有的顶点－骨头－索引对
	 */
	inline const Assignment& GetFirstAssignment()
	{
		itr = mMap.begin();
		return *itr;
	}

	/**
	 * 判断是否已经枚举完所有的顶点－骨头－索引对
	 */
	inline bool IsEnd() const
	{
		return itr == mMap.end();
	}

	/**
	 * 获得下一个《顶点，骨头和索引》对。用于枚举所有的顶点－骨头－索引对
	 */
	inline const Assignment& GetNextAssignment()
	{
        ++itr;
		return *itr;
	}

protected:
	vector<Assignment> mMap;	///< 顶点的索引号及其对应的骨头和权重
	vector<Assignment>::const_iterator itr;
};
