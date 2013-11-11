#ifndef  __RenderQueueBuilder_H__
#define  __RenderQueueBuilder_H__



namespace ma
{

	class RenderQueueBuilder : public SceneVisiter
	{

	public:
		RenderQueueBuilder(Camera* pCamera)
		{
			m_pCamera = pCamera;
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
				if ( !m_pCamera->IsCull(pRenderObj->GetAABB()) )
				{
					m_arrRenderObject.push_back(pRenderObj);
					m_aabb.Merge( pRenderObj->GetAABB() );
				}
			}

			return true;
		}

		const AABB& GetWorldAABB() {return m_aabb;}

		void AddToRenderQueue()
		{
			for (UINT i = 0; i < m_arrRenderObject.size(); ++i)
			{
				m_arrRenderObject[i]->UpdateTransform();
				m_arrRenderObject[i]->AddToRenderQueue();
			}
		}


	private:
		Camera*						m_pCamera;

		std::vector<RenderObject*>	m_arrRenderObject;

		AABB						m_aabb;
	};

}

#endif // __SceneVisiter_H__
