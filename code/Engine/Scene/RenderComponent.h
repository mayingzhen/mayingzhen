#pragma once

namespace ma
{
	class SubMaterial;
	class CullNode;
	class Renderable;
	class RenderView;

	enum AABB_CHANGE_TYPE
	{
		ACT_NONE = 0x00,
		ACT_SELF_MATRIX = 0x01,
		ACT_SELF_CUSTOM = 0x02,//自定义包围盒
		ACT_NOTIFY = 0x04,
	};


	class RenderComponent : public Component
	{
		DECL_OBJECT(RenderComponent)

	public:
		RenderComponent();
		
		virtual	void	Update();
		
		virtual void	SetVisible(bool bVisible) {m_bVisible = bVisible;}
		virtual bool	GetVisible() const {return m_bVisible;}
		
		virtual void	SetShadowCaster(bool b);
		bool			GetShadowCaster() const {return m_bShadowCaster;}

		const	AABB&	GetAABB() const;
		void			SetAABB(const AABB& box);
		const AABB&		GetAABBWS();

		virtual	void	SetViewMinMaxZ(float fZmin,float fZmax) {m_fViwMinZ = fZmin;m_fViwMaxZ = fZmax;}
		virtual float	GetViewMinZ() const {return m_fViwMinZ;}
		virtual	float	GetViewMaxZ() const {return m_fViwMaxZ;}

		virtual void	SetNeedChange(CHANGE_TYPE eChangeType);

		virtual	void	OnAddToSceneNode(SceneNode* pNode);
		virtual	void	OnRemoveFromSceneNode(SceneNode* pNode);

		virtual	bool	GetTransluce() { return false; }

	protected:
		virtual void	UpdateWorldBoundingBox();

	protected:
		
		AABB				m_AABB;
		AABB				m_worldAABB;
		int					m_nAABBChangeType;	

		float				m_fViwMinZ;
		float				m_fViwMaxZ;
		
		bool				m_bVisible;

		bool				m_bShadowCaster;

		RefPtr<RenderProxy>	m_pRenderproxy;
	};

	class RenderProxy : public Referenced
	{
	public:

		void			SetCullNode(CullNode* pCullNode) { m_pCullNode = pCullNode; }
		CullNode*		GetCullNode() const { return m_pCullNode; }

		uint32_t		GetCullIndex() { return m_nCullIndex; }
		void			SetCullIndex(uint32_t nIndex) { m_nCullIndex = nIndex; }

		void			SetAABBWS(const AABB& aabb);
		const AABB&		GetAABBWS();

		virtual uint32_t GetRenderableCount() const { return 0; }
		virtual Renderable* GetRenderableByIndex(uint32_t index) const { return nullptr; }

		virtual void	SetShadowCaster(bool b) { m_bShadowCaster = b; }
		bool			GetShadowCaster() const { return m_bShadowCaster; }

	protected:
		CullNode*		m_pCullNode = nullptr;
		uint32_t		m_nCullIndex = -1;

		AABB			m_worldAABB[2];

		bool			m_bShadowCaster = false;
	};

	
}


