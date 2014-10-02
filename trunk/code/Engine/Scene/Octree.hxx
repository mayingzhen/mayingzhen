#include "Octree.h"

namespace ma
{

	Octree::Octree(void)
	{
		static const float g_fMaxExtent = 1000.0f;
		Vector3 vMin(-g_fMaxExtent, -g_fMaxExtent, -g_fMaxExtent);
		m_pRoot = new OctreeNode(AABB(vMin,-vMin));
		m_nMaxDepth = 6;
	}

	Octree::~Octree(void)
	{
		SAFE_DELETE(m_pRoot);
	}

	// ---------------------------------------------------------------------
	// Inherite from CCullTree
	// ---------------------------------------------------------------------
	// resize the culltree to the given size
	void Octree::Resize(const AABB& box)
	{
		ASSERT(false);
// 		typedef std::vector<GameObject*> VEC_OBJ;
// 		VEC_OBJ vecObj;
// 		this->_FindObjectsIn(m_pRoot, true, box, vecObj);
// 
// 		for (VEC_OBJ::iterator iter = vecObj.begin();iter != vecObj.end();++iter)
// 		{
// 			GameObject* pObj = *iter;
// 			pObj->SetCullNode(NULL);
// 		}
// 
// 		SAFE_DELETE(m_pRoot);
// 		m_pRoot = new OctreeNode(box);
// 		for (VEC_OBJ::iterator iter = vecObj.begin();iter != vecObj.end();++iter)
// 		{
// 			GameObject* pObj = *iter;
// 			this->AddObject(pObj);
// 		}
	}


	// object operation
	void Octree::AddObject(RenderComponent* pObject)
	{
		if (pObject->GetCullNode() != NULL)
		{
			ASSERT(false);
			return;
		}

		// ������󲻷���ü���
		//if (pObject->GetSceneNode()->GetAABBWS().IsNull())
		//{
		//	return;
		//}

		this->_AddObject(m_pRoot, 0, pObject);
	}

	void Octree::RemoveObject(RenderComponent* pObject)
	{
		CullNode* pCullNode = pObject->GetCullNode();
		if (pCullNode == NULL)
		{
			return;
		}

		pCullNode->RemoveObject(pObject);
		pObject->SetCullNode(NULL);
	}

	void Octree::UpdateObject(RenderComponent* pObject)
	{
		this->RemoveObject(pObject);
		this->AddObject(pObject);
	}

	void Octree::FindObjectsIn(CullTreeQuery& query) const
	{
		this->_FindObjectsIn(m_pRoot,false,query);
	}


	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------

	void Octree::_AddObject(OctreeNode* pNode, int nDepth, RenderComponent* pObject)
	{
		ASSERT(pNode != NULL && pObject != NULL);
		const AABB& box = pObject->GetAABBWS();

		// GetWorldBoundingBox���Ѿ��ӽ�ȥ��
		// ���ֹ�ظ����
		if (pObject->GetCullNode() != NULL)
		{
			return;
		}

		// if the octree is twice as big as the scene node
		// we will add it to a child
		if ((nDepth < m_nMaxDepth) && pNode->IsTwiceSize(box))
		{
			int x,y,z;
			pNode->GetChildIndexByBox(box, x, y, z);

			if (pNode->m_rgChildren[x][y][z] == NULL)
			{
				const Vector3& vNodeMin = pNode->GetBoundingBox().getMinimum();
				const Vector3& vNodeMax = pNode->GetBoundingBox().getMaximum();

				Vector3 vMin, vMax;
				if (x == 0)
				{
					vMin.x = vNodeMin.x;
					vMax.x = (vNodeMin.x + vNodeMax.x)/2;
				}
				else
				{
					vMin.x = (vNodeMin.x + vNodeMax.x)/2;
					vMax.x = vNodeMax.x;
				}

				if (y == 0)
				{
					vMin.y = vNodeMin.y;
					vMax.y = (vNodeMin.y + vNodeMax.y)/2;
				}
				else
				{
					vMin.y = (vNodeMin.y + vNodeMax.y)/2;
					vMax.y = vNodeMax.y;
				}

				if (z == 0)
				{
					vMin.z = vNodeMin.z;
					vMax.z = (vNodeMin.z + vNodeMax.z)/2;
				}
				else
				{
					vMin.z = (vNodeMin.z + vNodeMax.z)/2;
					vMax.z = vNodeMax.z;
				}

				pNode->m_rgChildren[x][y][z] = new OctreeNode(AABB(vMin, vMax));
			}

			this->_AddObject(pNode->m_rgChildren[x][y][z], ++nDepth, pObject);
		}
		else
		{
			pObject->SetCullNode(pNode);
			pNode->AddObject(pObject);
		}
	}


	void Octree::_FindObjectsIn(OctreeNode* pNode, bool bFull, CullTreeQuery& query) const
	{
		Frustum::Visibility visiblity = Frustum::Visibility_NONE;
		if (bFull)
		{
			visiblity = Frustum::Visibility_FULL;
		}
		else
		{
			visiblity = query.TestCullNode(pNode);
		}

		if (visiblity == Frustum::Visibility_NONE)
		{
			return;
		}

		query.VisitCullNode(pNode,visiblity);

		//
		bool bFindChildVisible = (visiblity == Frustum::Visibility_FULL);
		OctreeNode* pChild = NULL;
		if ((pChild = pNode->m_rgChildren[0][0][0]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, query);
		}

		if ((pChild = pNode->m_rgChildren[1][0][0]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, query);
		}

		if ((pChild = pNode->m_rgChildren[0][1][0]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, query);
		}

		if ((pChild = pNode->m_rgChildren[1][1][0]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, query);
		}

		if ((pChild = pNode->m_rgChildren[0][0][1]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, query);
		}

		if ((pChild = pNode->m_rgChildren[1][0][1]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, query);
		}

		if ((pChild = pNode->m_rgChildren[0][1][1]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, query);
		}

		if ((pChild = pNode->m_rgChildren[1][1][1]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, query);
		}
	}

}