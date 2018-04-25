#ifndef  _OctreeNode__H__
#define  _OctreeNode__H__

#include "CullNode.h"

namespace ma
{

	class OctreeNode : public CullNode
	{
	public:
		OctreeNode(const AABB& box);

		~OctreeNode();

		void AddObject(RenderComponent* pObject);
		
		void RemoveObject(RenderComponent* pObject);

		// ---------------------------------------------------------------------
		// Self
		// ---------------------------------------------------------------------
	public:
		// 获取需要裁剪的包围盒，此包围盒比实际的包围盒更大
		void GetCullBoundingBox(AABB& box) const;

		// 实际包围盒
		const AABB& GetBoundingBox() const{return m_AABB;}

		// 此node超过输入box的两倍大
		bool IsTwiceSize(const AABB& box) const;

		// 获取适合box的child
		void GetChildIndexByBox(const AABB& box, int& x, int& y, int& z);

		// object operation
		uint32_t GetObjectAmount() const;

		RenderComponent* GetObjectByIndex(uint32_t nIndex) const;

	private:
		AABB		m_AABB;

		Vector3		m_vHalfSize;

		typedef std::vector<RenderComponent*> VEC_OBJECT;
		VEC_OBJECT	m_vecObject;

	public:
		OctreeNode* m_rgChildren[2][2][2];
	};

}

#endif