#ifndef  _Octree__H__
#define  _Octree__H__

#include "CullTree.h"

namespace ma
{
	class OctreeNode;

	class Octree : public CullTree
	{
	public:
		Octree();

		~Octree();

		// ---------------------------------------------------------------------
		// Inherite from CCullTree
		// ---------------------------------------------------------------------
		// resize the culltree to the given size
		void Resize(const AABB& box);

		// object operation
		void AddObject(GameObject* pObject);
		
		void RemoveObject(GameObject* pObject);
		
		void UpdateObject(GameObject* pobject);

		// Recurses the tree, adding any obj that intersectig with the frustum/ray/box/sphere/volume into the given list
		void FindObjectsIn(const Frustum* pFrustum, std::vector<GameObject*>& vecObj) const;
		
		void FindObjectsIn(const AABB& box, std::vector<GameObject*>& vecObj) const;
		
		//void FindObjectsIn(const Sphere& sphere, OUT vector<CNode*>& vecObj) const;
		
		//void FindObjectsIn(const Ray& ray, OUT vector<std::pair<CNode*, float> >& vecObj) const;

		// ---------------------------------------------------------------------
		// Self
		// ---------------------------------------------------------------------
	private:
		void _AddObject(OctreeNode* pNode, int nDepth, GameObject* pObject);
		
		void _FindObjectsIn(OctreeNode* pNode, bool bFull, const Frustum* pFrustum, std::vector<GameObject*>& vecObj) const;
		
		void _FindObjectsIn(OctreeNode* pNode, bool bFull, const AABB& box, std::vector<GameObject*>& vecObj) const;
		
		//void _FindObjectsIn(OctreeNode* pNode, bool bFull, const Sphere& sphere, OUT vector<CNode*>& vecObj) const;
		
		//void _FindObjectsIn(OctreeNode* pNode, bool bFull, const Ray& ray, OUT vector<std::pair<CNode*, float> >& vecObj) const;


	private:
		OctreeNode*		m_pRoot;
		
		int				m_nMaxDepth;
	};

}

#endif