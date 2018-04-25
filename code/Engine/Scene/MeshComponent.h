#ifndef  _MeshComponent__H__
#define  _MeshComponent__H__

#include "Engine/Scene/RenderComponent.h"

namespace ma
{
	class Material;
	class SubMaterial;
	class Renderable;
	class MeshData;
	
	class MeshComponent : public RenderComponent
	{
		
	public:
		MeshComponent();

		~MeshComponent();

		DECL_OBJECT(MeshComponent)	

		static void					RegisterAttribute();

		virtual void				Update();

		virtual	void				Show(Camera* pCamera);

		bool						Load(const char* pszSknPath,const char* pszMatPath);

		const char*					GetMeshFile(uint32_t nLod) const;
		void						SetMeshFile(const char* pFile,uint32_t nLod);
		MeshData*					GetMeshData(uint32_t nLod);

		const char*					GetMaterialFile() const;
		void						SetMaterialFile(const char* pFile);
		void						SetMaterial(Material* pMaterial);

		uint32_t						GetSubMaterialCount(uint32_t nLod);
		SubMaterial*				GetSubMaterial(uint32_t nLod,uint32_t index);

		virtual uint32_t				GetRenderableCount(uint32_t nLod) const;
		virtual	Renderable*			GetRenderableByIndex(uint32_t nLod,uint32_t index) const;

		virtual uint32_t				GetShadowRenderableCount() const;
		virtual	Renderable*			GetShadowRenderableByIndex(uint32_t index) const;

		virtual void				SetShadowCaster(bool b);

		virtual void				SetSuportInstance(bool b);

		virtual bool				Import(rapidxml::xml_node<>* pXmlElem);
		virtual bool				Export(rapidxml::xml_node<>* pXmlElem,rapidxml::xml_document<>& doc);	

	protected:
		typedef std::vector< RefPtr<MeshData> > LODMESHDATA;
		typedef std::vector< RefPtr<MeshRenderable> > VEC_RENDERABLE;
		typedef std::vector< VEC_RENDERABLE > VEC_LOD_RENDERABLE;

		virtual bool				IsReady();
	
		virtual RefPtr<MeshRenderable> CreateMeshRenderable();

		void						CreateRenderable(VEC_LOD_RENDERABLE& arrLodRenderable);

	protected:

		uint32_t						m_nLod = 0;

		RefPtr<Material>			m_pMaterial;

		LODMESHDATA					m_vecMesData;

		VEC_LOD_RENDERABLE			m_arrLodRenderable;

		bool						m_bSuportInstance = false;

		bool						m_bOnLoadOver = false;
	};

	RefPtr<MeshComponent> CreateMeshComponent();
}

#endif
