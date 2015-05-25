#ifndef  _RenderComponent__H__
#define  _RenderComponent__H__


namespace ma
{
	class SubMaterial;
	class CullNode;
	class Renderable;


	class RenderComponent : public Component
	{
		DECL_OBJECT(RenderComponent)

	public:
		RenderComponent();
		
		virtual	void	Update();

		virtual void	Show(Camera* pCamera); 

		virtual	void	MarkDirty();
		
		virtual void	SetVisible(bool bVisible) {m_bVisible = bVisible;}
		virtual bool	GetVisible() const {return m_bVisible;}

		virtual	UINT	GetRenderableCount() const;
		virtual Renderable* GetRenderableByIndex(UINT index) const;

		void			SetShadowCaster(bool b) {m_bShadowCaster = b;}
		bool			GetShadowCaster() const {return m_bShadowCaster;}
	
		void			SetCullNode(CullNode* pCullNode) {m_pCullNode = pCullNode;}
		CullNode*		GetCullNode() const {return m_pCullNode;}

		const AABB&		GetAABBWS() const;

		virtual	void	SetViewMinMaxZ(float fZmin,float fZmax) {m_fViwMinZ = fZmin;m_fViwMaxZ = fZmax;}
		virtual float	GetViewMinZ() const {return m_fViwMinZ;}
		virtual	float	GetViewMaxZ() const {return m_fViwMaxZ;}

	private:
		void			UpdateAABBWS() const;
		void			UpdateCullTree();

	protected:
		CullNode*			m_pCullNode;

		AABB				m_AABB;
		mutable AABB		m_worldAABB;
		mutable bool		m_bMatrixDirty;
		mutable bool		m_bCullDirty;

		float				m_fViwMinZ;
		float				m_fViwMaxZ;
		
		bool				m_bVisible;

		bool				m_bShadowCaster;
	};
}

#endif // _RenderComponent__H__

