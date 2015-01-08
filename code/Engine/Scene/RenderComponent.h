#ifndef  _RenderComponent__H__
#define  _RenderComponent__H__


namespace ma
{
	class SubMaterial;
	class CullNode;


	class ENGINE_API RenderComponent : public Component
	{
		DECL_OBJECT(RenderComponent)

	public:
		RenderComponent();

		virtual void	Show(Camera* pCamera); 

		virtual	void	OnTransformChange();
		
		virtual void	SetVisible(bool bVisible) {m_bVisible = bVisible;}
		virtual bool	GetVisible() const {return m_bVisible;}

		void			SetShadowCaster(bool b) {m_bShadowCaster = b;}
		bool			GetShadowCaster() const {return m_bShadowCaster;}
	
		void			SetCullNode(CullNode* pCullNode) {m_pCullNode = pCullNode;}
		CullNode*		GetCullNode() const {return m_pCullNode;}

		const AABB&		GetAABBWS();
		float			GetViewMinZ();
		float			GetViewMaxZ();

	protected:
		CullNode*			m_pCullNode;

		AABB				m_AABB;

		AABB				m_worldAABB;

		float				m_fViewMinZ;

		float				m_fViewMaxZ;
		
		bool				m_bVisible;

		bool				m_bShadowCaster;

	};
}

#endif // _RenderComponent__H__

