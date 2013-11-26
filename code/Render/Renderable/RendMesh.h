#ifndef  _IRENDMESH__H__
#define  _IRENDMESH__H__


namespace ma
{
	class MeshData;
	class Material;

	class RENDER_API RenderMesh : public RenderObject
	{
		DECL_OBJECT(RenderMesh)

	public:
		RenderMesh(GameObject* pGameObj);

		virtual void			Render(Technique* pTech);

		virtual void			SetWorldMatrix(const Matrix4x4& matWorld);

		virtual void			SetMaterial(Material* pMaterial);

		virtual Material*		GetMaterial() {return m_pMaterial;}

		virtual	void			AddToRenderQueue();

		virtual	void			UpdateTransform();

		virtual AABB			GetAABBWS();

		virtual void			Serialize(Serializer& sl, const char* pszLable = "RenderMesh");

		bool					InitWithData(MeshData* pMeshData);

		bool					Load(const char* pszSknPath,const char* pszMatPath);
	
		void					SetSkinMatrix(const Matrix4x4* arrMatrixs,Uint nCount);

		MeshData*				GetMeshData() {return m_pMesData;}

		UINT					GetRenderableNumber() {return m_arrRenderable.size();}
	
		Renderable*				GetRenderableByIndex(UINT index) {return m_arrRenderable[index];}

	public:
		//std::string				m_sknPath;
		//std::string				m_matPath;	
		
 		MeshData*				m_pMesData;

		//Texture*				m_pTexture;

		Material*				m_pMaterial;

		std::vector<Renderable*>	m_arrRenderable;

		bool					m_bSkin;

		AABB					m_aabbWS;
	};
}

#endif
