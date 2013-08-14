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
		m_autoDefaultBings["u_worldviewMatrix"] = WORLD_VIEW_MATRIX;
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

		return m_pCurRenderable->m_matWorld;
	}

	const Matrix4x4& MaterialManager::autoBindingGetViewMatrix() const
	{
		Camera* pCamera = GetRenderSystem()->GetCamera();
		return pCamera ? pCamera->GetViewMatrix() : Matrix4x4::identity();
	}

	const Matrix4x4& MaterialManager::autoBindingGetProjectionMatrix() const
	{
		Camera* pCamera = GetRenderSystem()->GetCamera();
		return pCamera ? pCamera->GetProjMatrix() : Matrix4x4::identity();
	}

	Matrix4x4 MaterialManager::autoBindingGetWorldViewMatrix() const
	{
		if (m_pCurRenderable == NULL)
			return Matrix4x4::identity();

		Camera* pCamera = GetRenderSystem()->GetCamera();
		if (pCamera == NULL)
			return Matrix4x4::identity();

		return m_pCurRenderable->m_matWorld * pCamera->GetViewMatrix();
	}

	Matrix4x4 MaterialManager::autoBindingGetViewProjectionMatrix() const
	{
		Camera* pCamera = GetRenderSystem()->GetCamera();
		if (pCamera == NULL)
			return Matrix4x4::identity();

		return pCamera->GetViewProjMatrix();
	}

	Matrix4x4 MaterialManager::autoBindingGetWorldViewProjectionMatrix() const
	{
		if (m_pCurRenderable == NULL)
			return Matrix4x4::identity();

		Camera* pCamera = GetRenderSystem()->GetCamera();
		if (pCamera == NULL)
			return Matrix4x4::identity();

		return m_pCurRenderable->m_matWorld * pCamera->GetViewProjMatrix();
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
		Camera* pCamera = GetRenderSystem()->GetCamera();
		if (pCamera)
		{
			Matrix4x4 mInvProj;
			Matrix4x4 matProj = pCamera->GetProjMatrix();
			MatrixInverse(&mInvProj, NULL, &matProj);
			return mInvProj;
		}
		else
		{
			return Matrix4x4::identity();
		}
	}

	Vector3 MaterialManager::autoBindingGetCameraWorldPosition() const
	{
		Camera* pCamera = GetRenderSystem()->GetCamera();
		return pCamera ? pCamera->GetTransform().m_vPos : Vec3Zero();
	}

	Vector3 MaterialManager::autoBindingGetCameraViewPosition() const
	{
		return /*_nodeBinding ? _nodeBinding->getActiveCameraTranslationView() :*/ Vec3Zero();
	}

	const Matrix4x4* MaterialManager::autoBindingGetMatrixPalette() const
	{
		if (m_pCurRenderable == NULL)
			return NULL;

		if ( m_pCurRenderable->m_arrSkinMatrix.empty() )
			return NULL;
		
		return &m_pCurRenderable->m_arrSkinMatrix[0];
	}

	UINT MaterialManager::autoBindingGetMatrixPaletteSize() const
	{
		if (m_pCurRenderable == NULL)
			return 0;

		return m_pCurRenderable->m_arrSkinMatrix.size();
	}

	const Vector3& MaterialManager::autoBindingGetAmbientColor() const
	{
		if (m_pCurLight == NULL)
			return Vec3Zero();

		return m_pCurLight->GetAmbientColor();
	}

	const Vector3& MaterialManager::autoBindingGetLightColor() const
	{
		if (m_pCurLight == NULL)
			return Vec3One();

		return m_pCurLight->GetDiffuse();
	}

	const Vector3& MaterialManager::autoBindingGetLightDirection() const
	{
		if (m_pCurLight == NULL)
			return Vec3One();

		return m_pCurLight->GetDirection();
	}

	Vector4 MaterialManager::autoBingingDepthNearFarInvfar() const
	{
		Camera* pCamera = GetRenderSystem()->GetCamera();
		if (pCamera == NULL)
			return Vec4One();
		
		float fNear = pCamera->GetNearClip();
		float fFar = pCamera->GetFarClip();
		
		return Vector4(fNear,fFar,1/fFar,0);
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
