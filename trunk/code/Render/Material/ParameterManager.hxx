#include "ParameterManager.h"
#include "../RenderScheme/GBufferPass.h"
#include "../RenderScheme/DeferredLightPass.h"
#include "../RenderScheme/DeferredShadowPass.h"

namespace ma
{
	static ParameterManager* gpParameterManager = NULL;

	ParameterManager* GetParameterManager()
	{
		return gpParameterManager;
	}

	void SetParameterManager(ParameterManager* pParameterManager)
	{
		gpParameterManager = pParameterManager;
	}

	ParameterManager::ParameterManager() 
	{
		m_autoDefaultBings["u_worldViewProjectionMatrix"] = WORLD_VIEW_PROJECTION_MATRIX;
		m_autoDefaultBings["u_worldViewMatrix"] = WORLD_VIEW_MATRIX;
		m_autoDefaultBings["u_matrixPalette"] = MATRIX_PALETTE;
		m_autoDefaultBings["depth_near_far_invfar"] = DepthNearFarInvfar;
		m_autoDefaultBings["u_InvProjMatrix"] =  INVERSE_PROJECTION_MATRIX;
		m_autoDefaultBings["u_textureSceneDepth"] =  TextureSceneDepth;
		m_autoDefaultBings["u_textureSceneNormal"] =  TextureSceneNormal;
		m_autoDefaultBings["u_TextureSceneShadow"] =  TextureLightShadow;
		m_autoDefaultBings["u_textureLightDiffuse"] =  TextureLightDiffuse;
		m_autoDefaultBings["u_textureLightSpecular"] =  TextureLightSpecular;
	}

	ParameterManager::~ParameterManager()
	{
	
	}

	void ParameterManager::UseDefaultBing(MaterialParameter* pParam)
	{
		ASSERT(pParam);
		if (pParam == NULL)
			return;

		std::map<std::string, AutoBinding>::iterator itr =  m_autoDefaultBings.find(pParam->GetName());
		if (itr != m_autoDefaultBings.end())
		{
			SetParameterAutoBinding(pParam,itr->second);
		}
	}

	void ParameterManager::SetParameterAutoBinding(MaterialParameter* pParam,AutoBinding autoBinding)
	{
		ASSERT(pParam);
		if (pParam == NULL)
			return;

		if (autoBinding == WORLD_MATRIX)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingGetWorldMatrix);
		}
		else if (autoBinding == VIEW_MATRIX)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingGetViewMatrix);
		}
		else if (autoBinding == PROJECTION_MATRIX)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingGetProjectionMatrix);
		}
		else if (autoBinding == WORLD_VIEW_MATRIX)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingGetWorldViewMatrix);
		}
		else if (autoBinding == VIEW_PROJECTION_MATRIX)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingGetViewProjectionMatrix);
		}
		else if (autoBinding == WORLD_VIEW_PROJECTION_MATRIX)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingGetWorldViewProjectionMatrix);
		}
		else if (autoBinding == INVERSE_TRANSPOSE_WORLD_MATRIX)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingGetInverseTransposeWorldMatrix);
		}
		else if (autoBinding == INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingGetInverseTransposeWorldViewMatrix);
		}
		else if (autoBinding == INVERSE_PROJECTION_MATRIX)
		{
			pParam->bindValue(this,&ParameterManager::autoBindingGetInverseProjectionMatrix);
		}
		else if (autoBinding == CAMERA_WORLD_POSITION)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingGetCameraWorldPosition);
		}
		else if (autoBinding == CAMERA_VIEW_POSITION)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingGetCameraViewPosition);
		}
		else if (autoBinding == MATRIX_PALETTE)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingGetMatrixPalette, &ParameterManager::autoBindingGetMatrixPaletteSize);
		}
		else if (autoBinding == SCENE_AMBIENT_COLOR)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingGetAmbientColor);
		}
		else if (autoBinding == SCENE_LIGHT_COLOR)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingGetLightColor);
		}
		else if (autoBinding == SCENE_LIGHT_DIRECTION)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingGetLightDirection);
		}
		else if (autoBinding == DepthNearFarInvfar)
		{
			pParam->bindValue(this, &ParameterManager::autoBingingDepthNearFarInvfar);
		}
		else if (autoBinding == TextureSceneDepth)
		{
			pParam->bindValue(this, &ParameterManager::autoBingingSceneDetph);
		}
		else if (autoBinding == TextureSceneNormal)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingSceneNormal);
		}
		else if (autoBinding == TextureLightDiffuse)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingTextureLightDiffuse);
		}
		else if (autoBinding == TextureLightSpecular)
		{
			pParam->bindValue(this, &ParameterManager::autoBindingTextureightSpecular);
		}
		else if (autoBinding == TextureLightShadow)
		{
			pParam->bindValue(this,&ParameterManager::autoBindingTextureLightShadow);
		}
		else
		{
			ASSERT("Unsupported auto binding type " && autoBinding);
		}
	}

	const Matrix4x4& ParameterManager::autoBindingGetWorldMatrix() const
	{
		Renderable* pRenderable = GetRenderContext()->GetCurRenderObj();
		if (pRenderable == NULL)
			return Matrix4x4::identity();

		int index = GetRenderSystem()->CurThreadProcess();
		return pRenderable->m_matWorld[index];
	}

	const Matrix4x4& ParameterManager::autoBindingGetViewMatrix() const
	{
		return GetRenderContext()->GetViewMatrix();
	}

	const Matrix4x4& ParameterManager::autoBindingGetProjectionMatrix() const
	{
		return GetRenderContext()->GetProjMatrix();;
	}

	Matrix4x4 ParameterManager::autoBindingGetWorldViewMatrix() const
	{
		Renderable* pRenderable = GetRenderContext()->GetCurRenderObj();
		if (pRenderable == NULL)
			return Matrix4x4::identity();

		int index = GetRenderSystem()->CurThreadProcess();
		return pRenderable->m_matWorld[index] * GetRenderContext()->GetViewMatrix();
	}

	Matrix4x4 ParameterManager::autoBindingGetViewProjectionMatrix() const
	{
		return GetRenderContext()->GetViewProjMatrix();
	}

	Matrix4x4 ParameterManager::autoBindingGetWorldViewProjectionMatrix() const
	{
		Renderable* pRenderable = GetRenderContext()->GetCurRenderObj();
		if (pRenderable == NULL)
			return Matrix4x4::identity();

		int index = GetRenderSystem()->CurThreadProcess();
		return pRenderable->m_matWorld[index] * GetRenderContext()->GetViewProjMatrix();
	}

	const Matrix4x4& ParameterManager::autoBindingGetInverseTransposeWorldMatrix() const
	{
		return /*_nodeBinding ? _nodeBinding->getInverseTransposeWorldMatrix() :*/ Matrix4x4::identity();
	}

	const Matrix4x4& ParameterManager::autoBindingGetInverseTransposeWorldViewMatrix() const
	{
		return /*_nodeBinding ? _nodeBinding->getInverseTransposeWorldViewMatrix() :*/ Matrix4x4::identity();
	}
		
	Matrix4x4 ParameterManager::autoBindingGetInverseProjectionMatrix() const
	{
		Matrix4x4 mInvProj;
		MatrixInverse(&mInvProj, NULL, & GetRenderContext()->GetProjMatrix() );
		return mInvProj;
	}

	Vector3 ParameterManager::autoBindingGetCameraWorldPosition() const
	{
		Matrix4x4 matWS;
		MatrixInverse(&matWS,NULL,& GetRenderContext()->GetViewMatrix() );
		return matWS.GetRow(3);
	}

	Vector3 ParameterManager::autoBindingGetCameraViewPosition() const
	{
		return /*_nodeBinding ? _nodeBinding->getActiveCameraTranslationView() :*/ Vec3Zero();
	}

	const Matrix4x4* ParameterManager::autoBindingGetMatrixPalette() const
	{
		Renderable* pRenderable = GetRenderContext()->GetCurRenderObj();
		if (pRenderable == NULL)
			return NULL;

		int index = GetRenderSystem()->CurThreadProcess();
		
		std::vector<Matrix4x4>&  arrSkinMatrix = pRenderable->m_arrSkinMatrix[index];

		if ( arrSkinMatrix.empty() )
			return NULL;
		
		return &arrSkinMatrix[0];
	}

	UINT ParameterManager::autoBindingGetMatrixPaletteSize() const
	{
		Renderable* pRenderable = GetRenderContext()->GetCurRenderObj();
		if (pRenderable == NULL)
			return 0;

		int index = GetRenderSystem()->CurThreadProcess();
		std::vector<Matrix4x4>&  arrSkinMatrix = pRenderable->m_arrSkinMatrix[index];

		return arrSkinMatrix.size();
	}

	const Vector4& ParameterManager::autoBindingGetAmbientColor() const
	{
		return GetLightSystem()->GetAmbientColor();
	}

	const Vector4& ParameterManager::autoBindingGetLightColor() const
	{
		Light* pCurLight = GetRenderContext()->GetCurLight();
		if (pCurLight == NULL)
			return Vec4One();

		return pCurLight->GetLightColor();
	}

	const Vector3& ParameterManager::autoBindingGetLightDirection() const
	{
		Light* pCurLight = GetRenderContext()->GetCurLight();
		if (pCurLight == NULL || pCurLight->GetLightType() != LIGHT_DIRECTIONAL)
		{
			ASSERT(false);
			return Vec3One();
		}
		
		DirectonalLight* pDirLigt = (DirectonalLight*)pCurLight;
		return pDirLigt->GetDirection();
	}

	Vector4 ParameterManager::autoBingingDepthNearFarInvfar() const
	{
		float fNear = GetRenderContext()->GetNearClip();
		float fFar = GetRenderContext()->GetFarClip();
		
		return Vector4(fNear,fFar,1.0f/fFar,0);
	}

	Texture* ParameterManager::autoBingingSceneDetph() const
	{
		if (GetBuffferPass() == NULL)
			return NULL;

		return GetBuffferPass()->GetSceneDepth();
	}

	Texture* ParameterManager::autoBindingSceneNormal() const
	{
		if (GetBuffferPass() == NULL)
			return NULL;

		return GetBuffferPass()->GetSceneNormal();
	}

	Texture* ParameterManager::autoBindingTextureLightDiffuse() const
	{
		if (GetDeferredLightPass() == NULL)
			return NULL;

		return GetDeferredLightPass()->GetTextureLightDiffuse();
	}

	Texture* ParameterManager::autoBindingTextureightSpecular() const
	{
		if (GetDeferredLightPass() == NULL)
			return NULL;

		return GetDeferredLightPass()->GetTextureightSpecular();
	}	

	Texture* ParameterManager::autoBindingTextureLightShadow() const
	{
		if (GetDeferredShadowPass() == NULL)
			return NULL;

		return GetDeferredShadowPass()->GetShadowTexture();
	}

}
