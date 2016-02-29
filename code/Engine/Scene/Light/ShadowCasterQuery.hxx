#include "ShadowCasterQuery.h"
#include "../../Scene/RenderComponent.h"

namespace ma
{

	ShadowCasterQuery::ShadowCasterQuery(const Frustum& lightFrustum,const Frustum& lightViewFrustum, 
		const Matrix4& matLightView, vector<RenderComponent*>& result, AABB& castrAABB)
		:m_lightFrustum(lightFrustum),m_lightViwFrustum(lightViewFrustum),
		m_matLightView(matLightView),m_result(result),m_casteAAbb(castrAABB)
	{
	
	}

	Frustum::Visibility ShadowCasterQuery::TestCullNode(CullNode* pCullNode)
	{
		AABB box;
		pCullNode->GetCullBoundingBox(box);
		return m_lightFrustum.Intersect(box);
	}

	void ShadowCasterQuery::VisitCullNode(CullNode* pCullNode,Frustum::Visibility eNodeVisib)
	{
		for (uint32 i = 0;i < pCullNode->GetObjectAmount();++i)
		{
			RenderComponent* pObject = pCullNode->GetObjectByIndex(i);
			if ( !pObject->GetVisible() )
			{
				continue;
			}

			if ( !pObject->GetShadowCaster() )
			{
				continue;
			}

			bool bCulll = true;

			if (eNodeVisib == Frustum::Visibility_FULL)
			{
				bCulll = false;
			}
			else if (m_lightFrustum.Intersect(pObject->GetAABBWS()) != Frustum::Visibility_NONE)
			{
				bCulll = false;
			}

// 			if (!bCulll) 
// 			{
// 				float lightViewFrustumBoxz = m_lightViwFrustum.GetAABB().getMinimum().z;
// 				AABB lightViewNodeBox = pObject->GetWorldBoundingBox();
// 				lightViewNodeBox.transform(m_matLightView);
// 				float lightViewNodeBoxMinz = lightViewNodeBox.getMinimum().z;
// 				lightViewNodeBoxMinz = min( lightViewNodeBoxMinz, lightViewFrustumBoxz);
// 				lightViewNodeBox.setMinimumZ(lightViewNodeBoxMinz);
// 				if (m_lightViwFrustum.Intersect(lightViewNodeBox) == CFrustum::Visibility_NONE )
// 				{
// 					bCulll = true;
// 				}
// 			}

			if (bCulll)
				continue;

			m_result.push_back(pObject);
			
			m_casteAAbb.merge(pObject->GetAABBWS());
		}
	}

}