#ifndef  _RenderComponent__H__
#define  _RenderComponent__H__


namespace ma
{
	class Material;
	class CullNode;


	class ENGINE_API RenderComponent : public Component
	{
		DECL_OBJECT(RenderComponent)

	public:
		RenderComponent(SceneNode* pGameObj);

		virtual void	Show(Camera* pCamera); 

		virtual	void	OnTransformChange();
		
		virtual void	SetVisible(bool bVisible) {m_bVisible = bVisible;}
		virtual bool	GetVisible() {return m_bVisible;}
	
		void			SetCullNode(CullNode* pCullNode) {m_pCullNode = pCullNode;}
		CullNode*		GetCullNode() {return m_pCullNode;}

		const AABB&		GetAABBWS();
	
	protected:
		CullNode*			m_pCullNode;

		AABB				m_AABB;

		AABB				m_worldAABB;
		
		bool				m_bVisible;

	};
}

#endif // _RenderComponent__H__

