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
		typedef std::vector<GameObject*> VEC_OBJ;
		VEC_OBJ vecObj;
		this->_FindObjectsIn(m_pRoot, true, box, vecObj);

		for (VEC_OBJ::iterator iter = vecObj.begin();iter != vecObj.end();++iter)
		{
			GameObject* pObj = *iter;
			pObj->SetCullNode(NULL);
		}

		SAFE_DELETE(m_pRoot);
		m_pRoot = new OctreeNode(box);
		for (VEC_OBJ::iterator iter = vecObj.begin();iter != vecObj.end();++iter)
		{
			GameObject* pObj = *iter;
			this->AddObject(pObj);
		}
	}


	// object operation
	void Octree::AddObject(GameObject* pObject)
	{
		if (pObject->GetCullNode() != NULL)
		{
			ASSERT(false);
			return;
		}

		// 虚拟对象不放入裁剪树
		//if (pObject->GetSceneNode()->GetAABBWS().IsNull())
		//{
		//	return;
		//}

		this->_AddObject(m_pRoot, 0, pObject);
	}

	void Octree::RemoveObject(GameObject* pObject)
	{
		CullNode* pCullNode = pObject->GetCullNode();
		if (pCullNode == NULL)
		{
			return;
		}

		pCullNode->RemoveObject(pObject);
		pObject->SetCullNode(NULL);
	}

	void Octree::UpdateObject(GameObject* pObject)
	{
		this->RemoveObject(pObject);
		this->AddObject(pObject);
	}

	// Recurses the tree, adding any obj that intersectig with the frustum/ray/box/sphere/volume into the given list
	void Octree::FindObjectsIn(const Frustum* pFrustum, std::vector<GameObject*>& vecObj) const
	{
		ASSERT(vecObj.empty());
		this->_FindObjectsIn(m_pRoot, false, pFrustum, vecObj);
	}

	void Octree::FindObjectsIn(const AABB& box, std::vector<GameObject*>& vecObj) const
	{
		ASSERT(vecObj.empty());
		this->_FindObjectsIn(m_pRoot, false, box, vecObj);
	}
 
// 	void Octree::FindObjectsIn(const Sphere& sphere, std::vector<GameObject*>& vecObj) const
// 	{
// 		ASSERT(vecObj.empty());
// 		this->_FindObjectsIn(m_pRoot, false, sphere, vecObj);
// 	}
// 
// 	void Octree::FindObjectsIn(const Ray& ray, std::vector<std::pair<GameObject*, float> >& vecObj) const
// 	{
// 		ASSERT(vecObj.empty());
// 		this->_FindObjectsIn(m_pRoot, false, ray, vecObj);
// 	}

	// ---------------------------------------------------------------------
	// Self
	// ---------------------------------------------------------------------

	void Octree::_AddObject(OctreeNode* pNode, int nDepth, GameObject* pObject)
	{
		ASSERT(pNode != NULL && pObject != NULL);
		const AABB& box = pObject->GetAABBWS();

		// GetWorldBoundingBox中已经加进去了
		// 或防止重复添加
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
				const Vector3& vNodeMin = pNode->GetBoundingBox().m_vMin;
				const Vector3& vNodeMax = pNode->GetBoundingBox().m_vMax;

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

	void Octree::_FindObjectsIn(OctreeNode* pNode, bool bFull, const Frustum* pFrustum, std::vector<GameObject*>& vecObj) const
	{
		Frustum::Visibility visiblity = Frustum::Visibility_NONE;
		if (bFull)
		{
			visiblity = Frustum::Visibility_FULL;
		}
		else
		{
			AABB box;
			pNode->GetCullBoundingBox(box);

			visiblity = pFrustum->Intersect(box);
		}

		if (visiblity == Frustum::Visibility_NONE)
		{
			return;
		}

		// 把对象放进去
		for (UINT i = 0;i< pNode->GetObjectAmount();++i)
		{
			GameObject* pObject = pNode->GetObjectByIndex(i);
			if (visiblity == Frustum::Visibility_FULL)
			{
				vecObj.push_back(pObject);
			}
			else if (pFrustum->Intersect(pObject->GetAABBWS ()) != Frustum::Visibility_NONE)
			{
				vecObj.push_back(pObject);
			}
		}

		//
		bool bFindChildVisible = (visiblity == Frustum::Visibility_FULL);
		OctreeNode* pChild = NULL;
		if ((pChild = pNode->m_rgChildren[0][0][0]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFindChildVisible, pFrustum, vecObj);
		}

		if ((pChild = pNode->m_rgChildren[1][0][0]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFindChildVisible, pFrustum, vecObj);
		}

		if ((pChild = pNode->m_rgChildren[0][1][0]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFindChildVisible, pFrustum, vecObj);
		}

		if ((pChild = pNode->m_rgChildren[1][1][0]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFindChildVisible, pFrustum, vecObj);
		}

		if ((pChild = pNode->m_rgChildren[0][0][1]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFindChildVisible, pFrustum, vecObj);
		}

		if ((pChild = pNode->m_rgChildren[1][0][1]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFindChildVisible, pFrustum, vecObj);
		}

		if ((pChild = pNode->m_rgChildren[0][1][1]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFindChildVisible, pFrustum, vecObj);
		}

		if ((pChild = pNode->m_rgChildren[1][1][1]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFindChildVisible, pFrustum, vecObj);
		}
	}

	void Octree::_FindObjectsIn(OctreeNode* pNode, bool bFull, const AABB& box, std::vector<GameObject*>& vecObj) const
	{
// 		if (!bFull)
// 		{
// 			AABB cullBox;
// 			pNode->GetCullBoundingBox(cullBox);
// 
// 			if (! intersects(box,cullBox) )
// 			{
// 				return;
// 			}
// 
// 			bFull = contains(box,cullBox);//box.contains(cullBox);
// 		}
// 
// 		// 把对象放进去
// 		for (UINT i = 0;i< pNode->GetObjectAmount();++i)
// 		{
// 			GameObject* pObject = pNode->GetObjectByIndex(i);
// 			if (bFull)
// 			{
// 				vecObj.push_back(pObject);
// 			}
// 			else if ( intersects( box, pObject->GetSceneNode()->GetAABBWS() ) )
// 			{
// 				vecObj.push_back(pObject);
// 			}
// 		}

		//
		OctreeNode* pChild = NULL;
		if ((pChild = pNode->m_rgChildren[0][0][0]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, box, vecObj);
		}

		if ((pChild = pNode->m_rgChildren[1][0][0]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, box, vecObj);
		}

		if ((pChild = pNode->m_rgChildren[0][1][0]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, box, vecObj);
		}

		if ((pChild = pNode->m_rgChildren[1][1][0]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, box, vecObj);
		}

		if ((pChild = pNode->m_rgChildren[0][0][1]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, box, vecObj);
		}

		if ((pChild = pNode->m_rgChildren[1][0][1]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, box, vecObj);
		}

		if ((pChild = pNode->m_rgChildren[0][1][1]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, box, vecObj);
		}

		if ((pChild = pNode->m_rgChildren[1][1][1]) != NULL)
		{
			this->_FindObjectsIn(pChild, bFull, box, vecObj);
		}
	}

// 	void Octree::_FindObjectsIn(OctreeNode* pNode, bool bFull, const Sphere& sphere, OUT vector<SceneNode*>& vecObj) const
// 	{
// 		if (!bFull)
// 		{
// 			AABB cullBox;
// 			pNode->GetCullBoundingBox(cullBox);
// 
// 			if (!sphere.intersects(cullBox))
// 			{
// 				return;
// 			}
// 
// 			bFull = CMathEx::Contains(sphere, cullBox);
// 		}
// 
// 
// 		// 把对象放进去
// 		for (UINT i = 0;i< pNode->GetObjectAmount();++i)
// 		{
// 			SceneNode* pObject = pNode->GetObjectByIndex(i);
// 			if (bFull)
// 			{
// 				vecObj.push_back(pObject);
// 			}
// 			else if (sphere.intersects(pObject->GetWorldBoundingBox()))
// 			{
// 				vecObj.push_back(pObject);
// 			}
// 		}
// 
// 		//
// 		OctreeNode* pChild = NULL;
// 		if ((pChild = pNode->m_rgChildren[0][0][0]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, sphere, vecObj);
// 		}
// 
// 		if ((pChild = pNode->m_rgChildren[1][0][0]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, sphere, vecObj);
// 		}
// 
// 		if ((pChild = pNode->m_rgChildren[0][1][0]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, sphere, vecObj);
// 		}
// 
// 		if ((pChild = pNode->m_rgChildren[1][1][0]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, sphere, vecObj);
// 		}
// 
// 		if ((pChild = pNode->m_rgChildren[0][0][1]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, sphere, vecObj);
// 		}
// 
// 		if ((pChild = pNode->m_rgChildren[1][0][1]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, sphere, vecObj);
// 		}
// 
// 		if ((pChild = pNode->m_rgChildren[0][1][1]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, sphere, vecObj);
// 		}
// 
// 		if ((pChild = pNode->m_rgChildren[1][1][1]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, sphere, vecObj);
// 		}
// 	}
// 
// 	void Octree::_FindObjectsIn(OctreeNode* pNode, bool bFull, const Ray& ray, OUT vector<std::pair<SceneNode*, float> >& vecObj) const
// 	{
// 		AABB cullBox;
// 		pNode->GetCullBoundingBox(cullBox);
// 
// 		std::pair<bool, float> isect = Math::intersects(ray, cullBox);
// 		if (!isect.first)
// 		{
// 			return;
// 		}
// 
// 		// 把对象放进去
// 		for (UINT i = 0;i< pNode->GetObjectAmount();++i)
// 		{
// 			SceneNode* pObject = pNode->GetObjectByIndex(i);
// 			std::pair<bool, float> isect = Math::intersects(ray, pObject->GetWorldBoundingBox());
// 			if (isect.first)
// 			{
// 				vecObj.push_back(std::make_pair<SceneNode*, float>(pObject, isect.second));
// 			}
// 		}
// 
// 		//
// 		OctreeNode* pChild = NULL;
// 		if ((pChild = pNode->m_rgChildren[0][0][0]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, ray, vecObj);
// 		}
// 
// 		if ((pChild = pNode->m_rgChildren[1][0][0]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, ray, vecObj);
// 		}
// 
// 		if ((pChild = pNode->m_rgChildren[0][1][0]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, ray, vecObj);
// 		}
// 
// 		if ((pChild = pNode->m_rgChildren[1][1][0]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, ray, vecObj);
// 		}
// 
// 		if ((pChild = pNode->m_rgChildren[0][0][1]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, ray, vecObj);
// 		}
// 
// 		if ((pChild = pNode->m_rgChildren[1][0][1]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, ray, vecObj);
// 		}
// 
// 		if ((pChild = pNode->m_rgChildren[0][1][1]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, ray, vecObj);
// 		}
// 
// 		if ((pChild = pNode->m_rgChildren[1][1][1]) != NULL)
// 		{
// 			this->_FindObjectsIn(pChild, bFull, ray, vecObj);
// 		}
// 	}

}