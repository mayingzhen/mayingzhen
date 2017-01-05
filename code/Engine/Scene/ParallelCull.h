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

		void AddObject(RenderComponent* pObject);

		void RemoveObject(RenderComponent* pObject);

		void UpdateObject(RenderComponent* pobject);

		void FindObjectsIn(const Frustum* pFrustum,uint32 mask, OUT vector<RenderComponent*>& vecObj); 

		void FindObjectsIn(CullTreeQuery& query,uint32 mask);		

	public:
		struct NodeBound
		{
			Vector3 m_vCenter;
			Vector3 m_vExtern;

			NodeBound(const AABB& aabb)
			{
				if (aabb.isNull())
				{
					m_vCenter = Vector3::ZERO;
					m_vExtern = Vector3::ZERO;
				}
				else
				{
					m_vCenter = aabb.getCenter();
					m_vExtern = aabb.getHalfSize();
				}
			}
		};

	private:

		vector<RenderComponent*> m_vecNode;
		
		vector<NodeBound> m_vecNodeBound;
	};

}