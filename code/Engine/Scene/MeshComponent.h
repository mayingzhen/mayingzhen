#pragma once

#include "Engine/Scene/RenderComponent.h"

namespace ma
{
	class Material;
	class SubMaterial;
	class Renderable;
	class MeshData;
	class MeshRenderProxy;
	
	class MeshComponent : public RenderComponent
	{
	public:
		MeshComponent();

		~MeshComponent();

		DECL_OBJECT(MeshComponent)	

		static void					RegisterAttribute();

		virtual void				Update();

		bool						Load(const char* pszSknPath,const char* pszMatPath);

		const char*					GetMeshFile() const;
		void						SetMeshFile(const char* pFile);
		MeshData*					GetMeshData();

		const char*					GetMaterialFile() const;
		void						SetMaterialFile(const char* pFile);
		void						SetMaterial(Material* pMaterial);

		uint32_t					GetSubMaterialCount();
		SubMaterial*				GetSubMaterial(uint32_t index);

		virtual void				SetShadowCaster(bool b);

		virtual void				SetSuportInstance(bool b);	

	protected:
		virtual bool				IsReady();

		virtual RefPtr<MeshRenderable>	CreateMeshRenderable();

		void						CreateRenderable();

		MeshRenderProxy*			GetMeshRenderProxy();

	protected:
		RefPtr<Material>			m_pMaterial;

		RefPtr<MeshData>			m_pMesData;

		bool						m_bSuportInstance = false;
	};

	class MeshRenderProxy : public RenderProxy
	{
	public:

		uint32_t					GetRenderableCount() const;
		Renderable*					GetRenderableByIndex(uint32_t index) const;

		void						AddRenderable(MeshRenderable* renderable);

		void						SetWorldMatrix(const Matrix4& matWS);

		void						Clear();

		void						SetSuportInstance(bool b);

		//virtual	void				Render(RenderView* pRenderView) override;;

	protected:
		typedef std::vector< RefPtr<MeshRenderable> > VEC_RENDERABLE;

		VEC_RENDERABLE				m_arrRenderable;
	};

	RefPtr<MeshComponent> CreateMeshComponent();
}

