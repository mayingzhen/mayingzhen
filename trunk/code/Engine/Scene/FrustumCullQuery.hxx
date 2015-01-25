#include "FrustumCullQuery.h"

namespace ma
{
	FrustumCullQuery::FrustumCullQuery(const Frustum& cameraFrustum, std::vector< RefPtr<RenderComponent> >& result)
			:m_cameraFrustum(cameraFrustum),m_result(result)
	{

	}

	Frustum::Visibility FrustumCullQuery::TestCullNode(CullNode* pCullNode)
	{
		AABB box;
		pCullNode->GetCullBoundingBox(box);
		return m_cameraFrustum.Intersect(box);
	}

	void FrustumCullQuery::VisitCullNode(CullNode* pCullNode,Frustum::Visibility eNodeVisib)
	{
		for (UINT i = 0;i < pCullNode->GetObjectAmount();++i)
		{
			RenderComponent* pObject = pCullNode->GetObjectByIndex(i);
			if ( !pObject->GetVisible() )
			{
				continue;
			}

			if (eNodeVisib == Frustum::Visibility_FULL)
			{
				m_result.push_back(pObject);
			}
			else if (m_cameraFrustum.Intersect(pObject->GetAABBWS()) != Frustum::Visibility_NONE)
			{
				m_result.push_back(pObject);
			}
		}
	}
}
