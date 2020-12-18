#pragma once

#include "CullTree.h"

namespace ma
{
	class ParallelCull : public CullTree
	{
	public:
		ParallelCull();

		~ParallelCull(void);

		// resize the culltree to the given size
		void Resize(const AABB& box);

		void AddObject(RenderProxy* pObject);

		void RemoveObject(RenderProxy* pObject);

		void UpdateObject(RenderProxy* pobject);

		void FindObjectsIn(const Frustum* pFrustum,uint32_t mask, OUT vector<RenderProxy*>& vecObj);

	public:
		struct NodeBound
		{
			Vector3 m_vCenter;
			Vector3 m_vExtern;
			bool m_bInfinite; 

			NodeBound()
			{
				m_bInfinite = true;
				m_vCenter = Vector3::ZERO;
				m_vExtern = Vector3::ZERO;
			}

			NodeBound(const AABB& aabb)
			{
				if (aabb.isInfinite())
				{
					m_bInfinite = true;
					m_vCenter = Vector3::ZERO;
					m_vExtern = Vector3::ZERO;
				}
				else 
				{
					m_bInfinite = false;
					m_vCenter = aabb.getCenter();
					m_vExtern = aabb.getHalfSize();
				}
			}
		};

	private:

		vector<RenderProxy*> m_vecNode;
		
		vector<NodeBound> m_vecNodeBound;
	};

}