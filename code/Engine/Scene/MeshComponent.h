#ifndef  _MeshComponent__H__
#define  _MeshComponent__H__

#include "Engine/Scene/RenderComponent.h"

namespace ma
{
	class MaterialData;
	class SubMaterial;
	class Renderable;
	class MeshData;
	
	class RENDER_API MeshComponent : public RenderComponent
	{
		DECL_OBJECT(MeshComponent)	

	public:
		MeshComponent();

		static void					RegisterObject(Context* context);

		virtual	void				Show(Camera* pCamera);

		virtual void				Update();

		bool						Load(const char* pszSknPath,const char* pszMatPath);
	
		void						SetSkinMatrix(const Matrix4* arrMatrixs,uint32 nCount);

		const char*					GetMeshFile() const;
		void						SetMeshFile(const char* pFile);

		const char*					GetMaterialFile() const;
		void						SetMaterialFile(const char* pFile);

		UINT32						GetSubMaterialCount();
		SubMaterial*				GetSubMaterial(UINT index);

	private:
		bool						OnLoadOver();
	
		void						CreateRenderable();

	private:
		RefPtr<MaterialData>		m_pMatData;
		RefPtr<MeshData>			m_pMesData;
	
		typedef std::vector< RefPtr<Renderable> > VEC_RENDERABLE;
		VEC_RENDERABLE				m_arrRenderable;

		bool						m_bOnLoadOver;
	};

	DeclareRefPtr(MeshComponent);

	//MeshComponentPtr CreateMeshComponent(const char* pszSknPath,const char* pszMatPath);
}

#endif
