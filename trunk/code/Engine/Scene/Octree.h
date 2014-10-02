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

		// resize the culltree to the given size
		void Resize(const AABB& box);

		// object operation
		void AddObject(RenderComponent* pObject);
		
		void RemoveObject(RenderComponent* pObject);
		
		void UpdateObject(RenderComponent* pobject);

		void FindObjectsIn(CullTreeQuery& query) const;

	private:
		void _AddObject(OctreeNode* pNode, int nDepth, RenderComponent* pObject);

		void _FindObjectsIn(OctreeNode* pNode, bool bFull, CullTreeQuery& query) const;
		
	private:
		OctreeNode*		m_pRoot;
		
		int				m_nMaxDepth;
	};

}

#endif