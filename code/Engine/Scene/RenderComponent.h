#ifndef  _RenderComponent__H__
#define  _RenderComponent__H__


namespace ma
{
	class SubMaterial;
	class CullNode;
	class Renderable;


	class ENGINE_API RenderComponent : public Component
	{
		DECL_OBJECT(RenderComponent)

	public:
		RenderComponent();

		virtual void	Show(Camera* pCamera); 

		virtual	void	OnTransformChange();

		virtual	void	OnAddToSceneNode(SceneNode* pGameObj);
		
		virtual void	SetVisible(bool bVisible) {m_bVisible = bVisible;}
		virtual bool	GetVisible() const {return m_bVisible;}

		virtual	UINT	GetRenderableCount() const;
		virtual Renderable* GetRenderableByIndex(UINT index) const;

		void			SetShadowCaster(bool b) {m_bShadowCaster = b;}
		bool			GetShadowCaster() const {return m_bShadowCaster;}
	
		void			SetCullNode(CullNode* pCullNode) {m_pCullNode = pCullNode;}
		CullNode*		GetCullNode() const {return m_pCullNode;}

		const AABB&		GetAABBWS() const {return m_worldAABB;}

		virtual	void	SetViewMinMaxZ(float fZmin,float fZmax) {m_fViwMinZ = fZmin;m_fViwMaxZ = fZmax;}
		virtual float	GetViewMinZ() const {return m_fViwMinZ;}
		virtual	float	GetViewMaxZ() const {return m_fViwMaxZ;}	

	protected:
		CullNode*			m_pCullNode;

		AABB				m_AABB;
		AABB				m_worldAABB;

		float				m_fViwMinZ;
		float				m_fViwMaxZ;
		
		bool				m_bVisible;

		bool				m_bShadowCaster;
	};
}

#endif // _RenderComponent__H__

