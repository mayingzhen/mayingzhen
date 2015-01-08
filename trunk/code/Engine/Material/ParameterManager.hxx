#include "ParameterManager.h"
#include "../RenderScheme/GBufferPass.h"
#include "../RenderScheme/DeferredLightPass.h"
#include "../RenderScheme/DeferredShadowPass.h"
#include "../Scene/Light/LightSystem.h"

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
		m_autoDefaultBings["boneDQ"] = MATRIX_PALETTE;
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
			pParam->BindMethod(this, &ParameterManager::autoBindingGetWorldMatrix);
		}
		else if (autoBinding == VIEW_MATRIX)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetViewMatrix);
		}
		else if (autoBinding == PROJECTION_MATRIX)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetProjectionMatrix);
		}
		else if (autoBinding == WORLD_VIEW_MATRIX)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetWorldViewMatrix);
		}
		else if (autoBinding == VIEW_PROJECTION_MATRIX)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetViewProjectionMatrix);
		}
		else if (autoBinding == WORLD_VIEW_PROJECTION_MATRIX)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetWorldViewProjectionMatrix);
		}
		else if (autoBinding == INVERSE_TRANSPOSE_WORLD_MATRIX)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetInverseTransposeWorldMatrix);
		}
		else if (autoBinding == INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetInverseTransposeWorldViewMatrix);
		}
		else if (autoBinding == INVERSE_PROJECTION_MATRIX)
		{
			pParam->BindMethod(this,&ParameterManager::autoBindingGetInverseProjectionMatrix);
		}
		else if (autoBinding == CAMERA_WORLD_POSITION)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetCameraWorldPosition);
		}
		else if (autoBinding == CAMERA_VIEW_POSITION)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetCameraViewPosition);
		}
		else if (autoBinding == MATRIX_PALETTE)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetMatrixPalette, &ParameterManager::autoBindingGetMatrixPaletteSize);
		}
		else if (autoBinding == SCENE_AMBIENT_COLOR)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetAmbientColor);
		}
		else if (autoBinding == SCENE_LIGHT_COLOR)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetLightColor);
		}
		else if (autoBinding == SCENE_LIGHT_DIRECTION)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingGetLightDirection);
		}
		else if (autoBinding == DepthNearFarInvfar)
		{
			pParam->BindMethod(this, &ParameterManager::autoBingingDepthNearFarInvfar);
		}
		else if (autoBinding == TextureSceneDepth)
		{
			pParam->BindMethod(this, &ParameterManager::autoBingingSceneDetph);
		}
		else if (autoBinding == TextureSceneNormal)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingSceneNormal);
		}
		else if (autoBinding == TextureLightDiffuse)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingTextureLightDiffuse);
		}
		else if (autoBinding == TextureLightSpecular)
		{
			pParam->BindMethod(this, &ParameterManager::autoBindingTextureightSpecular);
		}
		else if (autoBinding == TextureLightShadow)
		{
			pParam->BindMethod(this,&ParameterManager::autoBindingTextureLightShadow);
		}
		else
		{
			ASSERT("Unsupported auto binding type " && autoBinding);
		}
	}

	const Matrix4& ParameterManager::autoBindingGetWorldMatrix() const
	{
		Renderable* pRenderable = GetRenderContext()->GetCurRenderObj();
		if (pRenderable == NULL)
			return Matrix4::IDENTITY;

		int index = GetRenderSystem()->CurThreadProcess();
		return pRenderable->m_matWorld[index];
	}

	const Matrix4& ParameterManager::autoBindingGetViewMatrix() const
	{
		return GetRenderContext()->GetViewMatrix();
	}

	const Matrix4& ParameterManager::autoBindingGetProjectionMatrix() const
	{
		return GetRenderContext()->GetProjMatrix();;
	}

	Matrix4 ParameterManager::autoBindingGetWorldViewMatrix() const
	{
		Renderable* pRenderable = GetRenderContext()->GetCurRenderObj();
		if (pRenderable == NULL)
			return Matrix4::IDENTITY;

		int index = GetRenderSystem()->CurThreadProcess();
		return GetRenderContext()->GetViewMatrix() * pRenderable->m_matWorld[index];
	}

	Matrix4 ParameterManager::autoBindingGetViewProjectionMatrix() const
	{
		return GetRenderContext()->GetViewProjMatrix();
	}

	Matrix4 ParameterManager::autoBindingGetWorldViewProjectionMatrix() const
	{
		Renderable* pRenderable = GetRenderContext()->GetCurRenderObj();
		if (pRenderable == NULL)
			return Matrix4::IDENTITY;

		int index = GetRenderSystem()->CurThreadProcess();
		return GetRenderContext()->GetViewProjMatrix() * pRenderable->m_matWorld[index];
	}

	const Matrix4& ParameterManager::autoBindingGetInverseTransposeWorldMatrix() const
	{
		return /*_nodeBinding ? _nodeBinding->getInverseTransposeWorldMatrix() :*/ Matrix4::IDENTITY;
	}

	const Matrix4& ParameterManager::autoBindingGetInverseTransposeWorldViewMatrix() const
	{
		return /*_nodeBinding ? _nodeBinding->getInverseTransposeWorldViewMatrix() :*/ Matrix4::IDENTITY;
	}
		
	Matrix4 ParameterManager::autoBindingGetInverseProjectionMatrix() const
	{
		return GetRenderContext()->GetProjMatrix().inverse();
	}

	Vector3 ParameterManager::autoBindingGetCameraWorldPosition() const
	{
		return GetRenderContext()->GetViewMatrix().inverse().getTrans();
	}

	Vector3 ParameterManager::autoBindingGetCameraViewPosition() const
	{
		return /*_nodeBinding ? _nodeBinding->getActiveCameraTranslationView() :*/ Vector3::ZERO;
	}

	const Vector4* ParameterManager::autoBindingGetMatrixPalette() const
	{
		Renderable* pRenderable = GetRenderContext()->GetCurRenderObj();
		if (pRenderable == NULL)
			return NULL;

		int index = GetRenderSystem()->CurThreadProcess();
		
		std::vector<DualQuaternion>& arrSkinDQ = pRenderable->m_arrSkinDQ[index];

		if ( arrSkinDQ.empty() )
			return NULL;
		
		return (const Vector4*)&arrSkinDQ[0];
	}

	UINT ParameterManager::autoBindingGetMatrixPaletteSize() const
	{
		Renderable* pRenderable = GetRenderContext()->GetCurRenderObj();
		if (pRenderable == NULL)
			return 0;

		int index = GetRenderSystem()->CurThreadProcess();
		std::vector<DualQuaternion>& arrSkinDQ = pRenderable->m_arrSkinDQ[index];

		return arrSkinDQ.size() * 2;
	}

	const ColourValue& ParameterManager::autoBindingGetAmbientColor() const
	{
		return GetLightSystem()->GetAmbientColor();
	}

	const ColourValue& ParameterManager::autoBindingGetLightColor() const
	{
		Light* pCurLight = GetRenderContext()->GetCurLight();
		if (pCurLight == NULL)
			return ColourValue::White;

		return pCurLight->GetLightColor();
	}

	Vector3 ParameterManager::autoBindingGetLightDirection() const
	{
		Light* pCurLight = GetRenderContext()->GetCurLight();
		if (pCurLight == NULL || pCurLight->GetLightType() != LIGHT_DIRECTIONAL)
		{
			ASSERT(false);
			return Vector3::UNIT_SCALE;
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
