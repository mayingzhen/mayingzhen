#include "OctreeNode.h"

namespace ma
{
	OctreeNode::OctreeNode(const AABB& box)
	{
		m_AABB = box;
		m_vHalfSize = m_AABB.getHalfSize();

		for (int i = 0;i< 2;++i)
		{
			for (int j = 0;j<2;++j)
			{
				for (int k = 0;k<2;++k)
				{
					m_rgChildren[i][j][k] = NULL;
				}
			}
		}
	}

	OctreeNode::~OctreeNode()
	{
		for (int i = 0;i< 2;++i)
		{
			for (int j = 0;j<2;++j)
			{
				for (int k = 0;k<2;++k)
				{
					SAFE_DELETE(m_rgChildren[i][j][k]);
				}
			}
		}
	}

	void OctreeNode::AddObject(RenderComponent* pObject)
	{
		ASSERT(std::find(m_vecObject.begin(), m_vecObject.end(), pObject) == m_vecObject.end());
		m_vecObject.push_back(pObject);
	}

	void OctreeNode::RemoveObject(RenderComponent* pObject)
	{
		VEC_OBJECT::iterator iter = std::find(m_vecObject.begin(), m_vecObject.end(), pObject);
		if (iter == m_vecObject.end())
		{
			ASSERT(false);
			return;
		}

		m_vecObject.erase(iter);
	}


	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------

	// 获取需要裁剪的包围盒，此包围盒比实际的包围盒更大
	void OctreeNode::GetCullBoundingBox(AABB& box) const
	{
		box.setExtents(m_AABB.getMinimum() - m_vHalfSize, m_AABB.getMaximum() + m_vHalfSize);
	}

	// 此node超过输入box的两倍大
	bool OctreeNode::IsTwiceSize(const AABB& box) const
	{
		if (box.isInfinite())
		{
			return false;
		}

		Vector3 vHalfBoxSize = m_AABB.getHalfSize();
		Vector3 vSize = box.getSize();
		return (vSize.x <= vHalfBoxSize.x) && (vSize.y <= vHalfBoxSize.y) && (vSize.z <= vHalfBoxSize.z);
	}

	// 获取适合box的child
	void OctreeNode::GetChildIndexByBox(const AABB& box, int& x, int& y, int& z)
	{
		Vector3 vNodeCenter = m_AABB.getMaximum().midPoint(m_AABB.getMinimum());
		Vector3 vBoxCenter = box.getMaximum().midPoint(box.getMinimum());
		if (vBoxCenter.x > vNodeCenter.x)
		{
			x = 1;
		}
		else
		{
			x = 0;
		}

		if (vBoxCenter.y > vNodeCenter.y)
		{
			y = 1;
		}
		else
		{
			y = 0;
		}

		if (vBoxCenter.z > vNodeCenter.z)
		{
			z = 1;
		}
		else
		{
			z = 0;
		}
	}

	// object operation
	UINT OctreeNode::GetObjectAmount() const
	{
		return m_vecObject.size();
	}

	RenderComponent* OctreeNode::GetObjectByIndex(UINT nIndex) const
	{
		return m_vecObject[nIndex];
	}

}