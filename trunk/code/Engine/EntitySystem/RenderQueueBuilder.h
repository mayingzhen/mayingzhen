#ifndef  __RenderQueueBuilder_H__
#define  __RenderQueueBuilder_H__

#include "Engine/RenderSystem/RenderObject.h"

namespace ma
{

	class RenderQueueBuilder : public SceneVisiter
	{

	public:
		RenderQueueBuilder(Frustum* pFrustum)
		{
			m_pFrustum = pFrustum;
		}

		virtual bool VisiteSceneNodeBegin(SceneNode* pSceneNode)
		{
			return true;
		}

		virtual bool VisiteComponent(Component* pComp)
		{
			RenderObject* pRenderObj = SafeCast<RenderObject>(pComp);
			if (pRenderObj)
			{
				AABB aabb = pRenderObj->GetAABB();

				if ( !m_pFrustum->IsCull(pRenderObj->GetAABB()) )
				{
					m_arrRenderObject.push_back(pRenderObj);
				}
			}

			return true;
		}

		void AddToRenderQueue()
		{
			for (UINT i = 0; i < m_arrRenderObject.size(); ++i)
			{
				m_arrRenderObject[i]->UpdateTransform();
				m_arrRenderObject[i]->AddToRenderQueue();
			}
		}

		void UpdateCastList(ShadowMapFrustum* pSMF)
		{
			pSMF->ClearCasterList();
			for (UINT i = 0; i < m_arrRenderObject.size(); ++i)
			{
				pSMF->AddCaster(m_arrRenderObject[i]);
			}		
		}

	private:
		Frustum* m_pFrustum;

		std::vector<RenderObject*>	m_arrRenderObject;
	};

}

#endif // __SceneVisiter_H__
