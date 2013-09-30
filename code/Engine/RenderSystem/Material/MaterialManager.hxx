#include "MaterialManager.h"


namespace ma
{
	static MaterialManager* gpMaterialManager = NULL;

	MaterialManager* GetMaterialManager()
	{
		return gpMaterialManager;
	}

	void SetMaterialManager(MaterialManager* pMaterialMang)
	{
		gpMaterialManager = pMaterialMang;
	}

	MaterialManager::MaterialManager() 
	{
		m_autoDefaultBings["u_worldViewProjectionMatrix"] = WORLD_VIEW_PROJECTION_MATRIX;
		m_autoDefaultBings["u_worldViewMatrix"] = WORLD_VIEW_MATRIX;
		m_autoDefaultBings["u_matrixPalette"] = MATRIX_PALETTE;
		m_autoDefaultBings["depth_near_far_invfar"] = DepthNearFarInvfar;
		m_autoDefaultBings["u_InvProjMatrix"] =  INVERSE_PROJECTION_MATRIX;
		m_autoDefaultBings["u_textureSceneDepth"] =  TextureSceneDepth;
		m_autoDefaultBings["u_textureSceneNormal"] =  TextureSceneNormal;
		m_autoDefaultBings["u_textureLightDiffuse"] =  TextureLightDiffuse;
		m_autoDefaultBings["u_textureLightSpecular"] =  TextureLightSpecular;

		m_pCurRenderable = NULL;
		m_pCurLight = NULL;
	}

	MaterialManager::~MaterialManager()
	{
	
	}

	void MaterialManager::UseDefaultBing(MaterialParameter* pParam)
	{
		ASSERT(pParam);
		if (pParam == NULL)
			return;

		std::map<std::string, AutoBinding>::iterator itr =  m_autoDefaultBings.find(pParam->getName());
		if (itr != m_autoDefaultBings.end())
		{
			SetParameterAutoBinding(pParam,itr->second);
		}
	}


	void MaterialManager::SetParameterAutoBinding(MaterialParameter* pParam,AutoBinding autoBinding)
	{
		ASSERT(pParam);
		if (pParam == NULL)
			return;

		if (autoBinding == WORLD_MATRIX)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingGetWorldMatrix);
		}
		else if (autoBinding == VIEW_MATRIX)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingGetViewMatrix);
		}
		else if (autoBinding == PROJECTION_MATRIX)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingGetProjectionMatrix);
		}
		else if (autoBinding == WORLD_VIEW_MATRIX)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingGetWorldViewMatrix);
		}
		else if (autoBinding == VIEW_PROJECTION_MATRIX)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingGetViewProjectionMatrix);
		}
		else if (autoBinding == WORLD_VIEW_PROJECTION_MATRIX)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingGetWorldViewProjectionMatrix);
		}
		else if (autoBinding == INVERSE_TRANSPOSE_WORLD_MATRIX)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingGetInverseTransposeWorldMatrix);
		}
		else if (autoBinding == INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingGetInverseTransposeWorldViewMatrix);
		}
		else if (autoBinding == INVERSE_PROJECTION_MATRIX)
		{
			pParam->bindValue(this,&MaterialManager::autoBindingGetInverseProjectionMatrix);
		}
		else if (autoBinding == CAMERA_WORLD_POSITION)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingGetCameraWorldPosition);
		}
		else if (autoBinding == CAMERA_VIEW_POSITION)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingGetCameraViewPosition);
		}
		else if (autoBinding == MATRIX_PALETTE)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingGetMatrixPalette, &MaterialManager::autoBindingGetMatrixPaletteSize);
		}
		else if (autoBinding == SCENE_AMBIENT_COLOR)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingGetAmbientColor);
		}
		else if (autoBinding == SCENE_LIGHT_COLOR)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingGetLightColor);
		}
		else if (autoBinding == SCENE_LIGHT_DIRECTION)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingGetLightDirection);
		}
		else if (autoBinding == DepthNearFarInvfar)
		{
			pParam->bindValue(this, &MaterialManager::autoBingingDepthNearFarInvfar);
		}
		else if (autoBinding == TextureSceneDepth)
		{
			pParam->bindValue(this, &MaterialManager::autoBingingSceneDetph);
		}
		else if (autoBinding == TextureSceneNormal)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingSceneNormal);
		}
		else if (autoBinding == TextureLightDiffuse)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingTextureLightDiffuse);
		}
		else if (autoBinding == TextureLightSpecular)
		{
			pParam->bindValue(this, &MaterialManager::autoBindingTextureightSpecular);
		}
		else
		{
			ASSERT("Unsupported auto binding type " && autoBinding);
		}
	}

	const Matrix4x4& MaterialManager::autoBindingGetWorldMatrix() const
	{
		if (m_pCurRenderable == NULL)
			return Matrix4x4::identity();

		int index = GetRenderThread()->m_nCurThreadProcess;
		return m_pCurRenderable->m_matWorld[index];
	}

	const Matrix4x4& MaterialManager::autoBindingGetViewMatrix() const
	{
		return GetRenderSystem()->GetViewMatrix();
	}

	const Matrix4x4& MaterialManager::autoBindingGetProjectionMatrix() const
	{
		return GetRenderSystem()->GetProjMatrix();
	}

	Matrix4x4 MaterialManager::autoBindingGetWorldViewMatrix() const
	{
		if (m_pCurRenderable == NULL)
			return Matrix4x4::identity();

		int index = GetRenderThread()->m_nCurThreadProcess;
		return m_pCurRenderable->m_matWorld[index] * GetRenderSystem()->GetViewMatrix();
	}

	Matrix4x4 MaterialManager::autoBindingGetViewProjectionMatrix() const
	{
		return GetRenderSystem()->GetViewProjMatrix();
	}

	Matrix4x4 MaterialManager::autoBindingGetWorldViewProjectionMatrix() const
	{
		if (m_pCurRenderable == NULL)
			return Matrix4x4::identity();

		int index = GetRenderThread()->m_nCurThreadProcess;
		return m_pCurRenderable->m_matWorld[index] * GetRenderSystem()->GetViewProjMatrix();
	}

	const Matrix4x4& MaterialManager::autoBindingGetInverseTransposeWorldMatrix() const
	{
		return /*_nodeBinding ? _nodeBinding->getInverseTransposeWorldMatrix() :*/ Matrix4x4::identity();
	}

	const Matrix4x4& MaterialManager::autoBindingGetInverseTransposeWorldViewMatrix() const
	{
		return /*_nodeBinding ? _nodeBinding->getInverseTransposeWorldViewMatrix() :*/ Matrix4x4::identity();
	}
		
	Matrix4x4 MaterialManager::autoBindingGetInverseProjectionMatrix() const
	{
		Matrix4x4 mInvProj;
		Matrix4x4 matProj = GetRenderSystem()->GetProjMatrix();
		MatrixInverse(&mInvProj, NULL, &matProj);
		return mInvProj;
	}

	Vector3 MaterialManager::autoBindingGetCameraWorldPosition() const
	{
		Matrix4x4 matWS;
		MatrixInverse(&matWS,NULL,&GetRenderSystem()->GetViewMatrix());
		return matWS.GetRow(3);
	}

	Vector3 MaterialManager::autoBindingGetCameraViewPosition() const
	{
		return /*_nodeBinding ? _nodeBinding->getActiveCameraTranslationView() :*/ Vec3Zero();
	}

	const Matrix4x4* MaterialManager::autoBindingGetMatrixPalette() const
	{
		if (m_pCurRenderable == NULL)
			return NULL;

		int index = GetRenderThread()->m_nCurThreadProcess;
		
		std::vector<Matrix4x4>&  arrSkinMatrix = m_pCurRenderable->m_arrSkinMatrix[index];

		if ( arrSkinMatrix.empty() )
			return NULL;
		
		return &arrSkinMatrix[0];
	}

	UINT MaterialManager::autoBindingGetMatrixPaletteSize() const
	{
		if (m_pCurRenderable == NULL)
			return 0;

		int index = GetRenderThread()->m_nCurThreadProcess;
		std::vector<Matrix4x4>&  arrSkinMatrix = m_pCurRenderable->m_arrSkinMatrix[index];

		return arrSkinMatrix.size();
	}

	const Vector4& MaterialManager::autoBindingGetAmbientColor() const
	{
		return GetRenderSystem()->GetAmbientColor();
	}

	const Vector4& MaterialManager::autoBindingGetLightColor() const
	{
		if (m_pCurLight == NULL)
			return Vec4One();

		return m_pCurLight->GetLightColor();
	}

	const Vector3& MaterialManager::autoBindingGetLightDirection() const
	{
		if (m_pCurLight == NULL || m_pCurLight->GetLightType() != LIGHT_DIRECTIONAL)
		{
			ASSERT(false);
			return Vec3One();
		}
		
		DirectonalLight* pDirLigt = (DirectonalLight*)m_pCurLight;
		return pDirLigt->GetDirection();
	}

	Vector4 MaterialManager::autoBingingDepthNearFarInvfar() const
	{
		float fNear = GetRenderSystem()->GetNearClip();
		float fFar = GetRenderSystem()->GetFarClip();
		
		return Vector4(fNear,fFar,1.0f/fFar,0);
	}

	Texture* MaterialManager::autoBingingSceneDetph() const
	{
		if (GetDeferredLight() == NULL)
			return NULL;

		return GetDeferredLight()->GetSceneDepth();
	}

	Texture* MaterialManager::autoBindingSceneNormal() const
	{
		if (GetDeferredLight() == NULL)
			return NULL;

		return GetDeferredLight()->GetSceneNormal();
	}

	Texture* MaterialManager::autoBindingTextureLightDiffuse() const
	{
		if (GetDeferredLight() == NULL)
			return NULL;

		return GetDeferredLight()->GetTextureLightDiffuse();
	}

	Texture* MaterialManager::autoBindingTextureightSpecular() const
	{
		if (GetDeferredLight() == NULL)
			return NULL;

		return GetDeferredLight()->GetTextureightSpecular();
	}		

}
