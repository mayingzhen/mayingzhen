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

		virtual void				Update();

		virtual	void				Show(Camera* pCamera);

		bool						Load(const char* pszSknPath,const char* pszMatPath);

		const char*					GetMeshFile() const;
		void						SetMeshFile(const char* pFile);

		const char*					GetMaterialFile() const;
		void						SetMaterialFile(const char* pFile);

		UINT32						GetSubMaterialCount(uint32 nLod);
		SubMaterial*				GetSubMaterial(uint32 nLod,UINT index);

		virtual UINT				GetRenderableCount(uint32 nLod) const;
		virtual	Renderable*			GetRenderableByIndex(uint32 nLod,UINT index) const;

	private:
		virtual bool				IsReady();
	
		virtual void				CreateRenderable();

	protected:
		RefPtr<Material>			m_pMaterial;
		RefPtr<MeshData>			m_pMesData;
	
		typedef std::vector< RefPtr<Renderable> > VEC_RENDERABLE;
		typedef std::vector< VEC_RENDERABLE > VEC_LOD_RENDERABLE;
		VEC_LOD_RENDERABLE			m_arrLodRenderable;

		bool						m_bOnLoadOver;
	};

	RefPtr<MeshComponent> CreateMeshComponent();

	class SkinMeshComponent : public MeshComponent
	{
		
	public:

		DECL_OBJECT(SkinMeshComponent)	

		static void					RegisterAttribute();

		void						SetSkinMatrix(const Matrix3x4* arrMatrixs,uint32 nCount);
	
	private:
		virtual void				CreateRenderable();
	};

	RefPtr<SkinMeshComponent> CreateSkinMeshComponent();
}

#endif
