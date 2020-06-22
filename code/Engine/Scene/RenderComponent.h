#pragma once

namespace ma
{
	class SubMaterial;
	class CullNode;
	class Renderable;

	enum AABB_CHANGE_TYPE
	{
		ACT_NONE = 0x00,
		ACT_SELF_MATRIX = 0x01,
		ACT_SELF_CUSTOM = 0x02,//自定义包围盒
		ACT_NOTIFY = 0x04,
	};

	enum RenderListType
	{
		RL_Mesh,
		RL_Terrain,
		RL_TerrainBorder,
		RL_TerrainSkirt,
		RL_SkyBox,
		RL_MeshTrans,
		RL_UI,
		RL_LAST,
		RL_Count,
	};

	class RenderComponent : public Component
	{
		DECL_OBJECT(RenderComponent)

	public:
		RenderComponent();
		
		virtual	void	Update();

		virtual void	Render(RenderQueue* pRenderQueue);

		virtual	void	RenderShadow(RenderQueue* pRenderQueue);
		
		virtual void	SetVisible(bool bVisible) {m_bVisible = bVisible;}
		virtual bool	GetVisible() const {return m_bVisible;}
		
		virtual void	SetShadowCaster(bool b) {m_bShadowCaster = b;}
		bool			GetShadowCaster() const {return m_bShadowCaster;}
	
		void			SetCullNode(CullNode* pCullNode) {m_pCullNode = pCullNode;}
		CullNode*		GetCullNode() const {return m_pCullNode;}

		const	AABB&	GetAABB() const;
		void			SetAABB(const AABB& box);
		const AABB&		GetAABBWS();

		virtual	void	SetViewMinMaxZ(float fZmin,float fZmax) {m_fViwMinZ = fZmin;m_fViwMaxZ = fZmax;}
		virtual float	GetViewMinZ() const {return m_fViwMinZ;}
		virtual	float	GetViewMaxZ() const {return m_fViwMaxZ;}

		virtual void	SetNeedChange(CHANGE_TYPE eChangeType);

		virtual	void	OnAddToSceneNode(SceneNode* pNode);
		virtual	void	OnRemoveFromSceneNode(SceneNode* pNode);

		uint32_t		GetCullIndex() {return m_nCullIndex;}
		void			SetCullIndex(uint32_t nIndex) {m_nCullIndex = nIndex;} 

	protected:
		virtual void	UpdateWorldBoundingBox();

	protected:
		CullNode*			m_pCullNode;

		AABB				m_AABB;
		AABB				m_worldAABB;
		int					m_nAABBChangeType;	

		float				m_fViwMinZ;
		float				m_fViwMaxZ;
		
		bool				m_bVisible;

		bool				m_bShadowCaster;

		bool				m_bParallelUpdate;

		uint32_t			m_nCullIndex;
	};
}


