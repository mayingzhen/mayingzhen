#pragma once

#include "CullTree.h"

namespace ma
{
	class OctreeNode;

	class Octree : public CullTree
	{
	public:
		Octree();

		~Octree();

		// resize the culltree to the given size
		void Resize(const AABB& box);

		// object operation
		void AddObject(RenderProxy* pObject);
		
		void RemoveObject(RenderProxy* pObject);
		
		void UpdateObject(RenderProxy* pobject);

		void FindObjectsIn(const Frustum* pFrustum,uint32_t mask, OUT vector<RenderProxy*>& vecObj);

	private:
		void _AddObject(OctreeNode* pNode, int nDepth, RenderProxy* pObject);

		void _FindObjectsIn(OctreeNode* pNode, bool bFull,const Frustum* pFrustum,uint32_t mask, OUT vector<RenderProxy*>& vecObj) const;
		
	private:
		OctreeNode*		m_pRoot;
		
		int				m_nMaxDepth;
	};

}