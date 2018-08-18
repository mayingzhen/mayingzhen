#pragma once

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

		virtual	void				Render(RenderQueue* pRenderQueue);

		virtual	void				RenderShadow(RenderQueue* pRenderQueue);

		bool						Load(const char* pszSknPath,const char* pszMatPath);

		const char*					GetMeshFile() const;
		void						SetMeshFile(const char* pFile);
		MeshData*					GetMeshData();

		const char*					GetMaterialFile() const;
		void						SetMaterialFile(const char* pFile);
		void						SetMaterial(Material* pMaterial);

		uint32_t					GetSubMaterialCount();
		SubMaterial*				GetSubMaterial(uint32_t index);

		virtual uint32_t			GetRenderableCount() const;
		virtual	Renderable*			GetRenderableByIndex(uint32_t index) const;

		virtual uint32_t			GetShadowRenderableCount() const;
		virtual	Renderable*			GetShadowRenderableByIndex(uint32_t index) const;

		virtual void				SetShadowCaster(bool b);

		virtual void				SetSuportInstance(bool b);	

	protected:
		typedef std::vector< RefPtr<MeshRenderable> > VEC_RENDERABLE;

		virtual bool				IsReady();

		virtual RefPtr<MeshRenderable>	CreateMeshRenderable();

		void						CreateRenderable(VEC_RENDERABLE& arrRenderable);

	protected:
		RefPtr<Material>			m_pMaterial;

		RefPtr<MeshData>			m_pMesData;

		VEC_RENDERABLE				m_arrRenderable;

		bool						m_bSuportInstance = false;

		bool						m_bOnLoadOver = false;
	};

	RefPtr<MeshComponent> CreateMeshComponent();
}

