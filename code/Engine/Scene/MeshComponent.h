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

		DECL_OBJECT(MeshComponent)	

		static void					RegisterAttribute();

		virtual	void				Show(Camera* pCamera);

		virtual void				Update();

		bool						Load(const char* pszSknPath,const char* pszMatPath);

		const char*					GetMeshFile() const;
		void						SetMeshFile(const char* pFile);

		const char*					GetMaterialFile() const;
		void						SetMaterialFile(const char* pFile);

		UINT32						GetSubMaterialCount();
		SubMaterial*				GetSubMaterial(UINT index);

		virtual UINT				GetRenderableCount() const;
		virtual	Renderable*			GetRenderableByIndex(UINT index) const;

	private:
		virtual bool				OnLoadOver();
	
		virtual void				CreateRenderable();

	protected:
		RefPtr<Material>			m_pMatData;
		RefPtr<MeshData>			m_pMesData;
	
		typedef std::vector< RefPtr<Renderable> > VEC_RENDERABLE;
		VEC_RENDERABLE				m_arrRenderable;

		bool						m_bOnLoadOver;
	};

	class SkinMeshComponent : public MeshComponent
	{
		
	public:

		DECL_OBJECT(SkinMeshComponent)	

		static void					RegisterAttribute();

		void						SetSkinMatrix(const Matrix3x4* arrMatrixs,uint32 nCount);
	
	private:
		virtual void				CreateRenderable();
	};
}

#endif
