#ifndef  _MeshComponent__H__
#define  _MeshComponent__H__


namespace ma
{
	class MaterialData;
	class Material;
	class Renderable;
	class MeshData;
	
	class RENDER_API MeshComponent : public RenderComponent
	{
		DECL_OBJECT(MeshComponent)	

	public:
		MeshComponent(SceneNode* pGameObj);

		virtual	void				Show(Camera* pCamera);

		virtual void				Update();

		virtual void				Serialize(Serializer& sl, const char* pszLable = "MeshComponent");

		bool						Load(const char* pszSknPath,const char* pszMatPath);
	
		void						SetSkinMatrix(const Matrix4* arrMatrixs,Uint nCount);

		RefPtr<MeshData>			GetMeshData() {return m_pMesData;}

	private:
		void						CreateRenderable(void* parm1,void* parm2);
		void						CreateMaterial(void* parm1,void* parm2);

	public:
		RefPtr<MaterialData>				m_pMatData;
		RefPtr<MeshData>					m_pMesData;

		std::vector< RefPtr<Material> >		m_arrMaterial;
		std::vector< RefPtr<Renderable> >	m_arrRenderable;

		typedef tEventListener<MeshComponent> ELMeshComponent;
		RefPtr< ELMeshComponent >			m_pElMeshLoaded;
		RefPtr< ELMeshComponent >			m_pElMaterialLoaded;
	};

	DeclareRefPtr(MeshComponent);
}

#endif
