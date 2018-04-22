#ifndef  _RenderComponent__H__
#define  _RenderComponent__H__


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

	class RenderComponent : public Component
	{
		DECL_OBJECT(RenderComponent)

	public:
		RenderComponent();
		
		virtual	void	Update();

		virtual void	Show(Camera* pCamera); 
		
		virtual void	SetVisible(bool bVisible) {m_bVisible = bVisible;}
		virtual bool	GetVisible() const {return m_bVisible;}
		
		virtual uint32  GetLodIndex() {return 0;}
		virtual	UINT	GetRenderableCount(uint32 nLod) const;
		virtual Renderable* GetRenderableByIndex(uint32 nLod,UINT index) const;

		virtual UINT	GetShadowRenderableCount() const;
		virtual	Renderable*	GetShadowRenderableByIndex(UINT index) const;

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

		UINT32			GetCullIndex() {return m_nCullIndex;}
		void			SetCullIndex(uint32 nIndex) {m_nCullIndex = nIndex;} 

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

		UINT32				m_nCullIndex;
	};
}

#endif // _RenderComponent__H__

