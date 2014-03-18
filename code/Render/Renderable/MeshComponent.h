#ifndef  _MeshComponent__H__
#define  _MeshComponent__H__


namespace ma
{
	class MaterialData;
	
	class RENDER_API MeshComponent : public RenderComponent
	{
		DECL_OBJECT(MeshComponent)

	public:
		MeshComponent(GameObject* pGameObj);

		virtual	void				Show(Camera* pCamera);

		virtual void				Update();

		virtual Material*			GetMaterial() {return m_pMaterial;}

		virtual AABB				GetAABBWS();

		virtual void				Serialize(Serializer& sl, const char* pszLable = "MeshComponent");

		bool						Load(const char* pszSknPath,const char* pszMatPath);
	
		void						SetSkinMatrix(const Matrix4x4* arrMatrixs,Uint nCount);

		ref_ptr<MeshData>			GetMeshData() {return m_pMesData;}

 		UINT						GetRenderableNumber() {return m_arrRenderable.size();}
 	
 		Renderable*					GetRenderableByIndex(UINT index) {return m_arrRenderable[index].get();}

	private:
		bool						CreateRenderable(ref_ptr<MeshData> pMeshData);

	public:
		ref_ptr<MaterialData>		m_pMatData;

		Material*					m_pMaterial;

 		ref_ptr<MeshData>			m_pMesData;

		std::vector<RenderablePtr>	m_arrRenderable;

		bool						m_bSkin;

		AABB						m_aabbWS;
	};

	DeclareRefPtr(MeshComponent);
}

#endif
