# ifndef _ParameterManager_H_
# define _ParameterManager_H_


namespace ma
{

    enum AutoBinding
	{
		NONE,
		g_matWorld,
		g_matView,
		g_matViewInv,
		g_matProj,
		g_matProjInv,
		g_matWorldView,
		g_matViewProj,
		g_matWorldViewProj,
		INVERSE_TRANSPOSE_WORLD_MATRIX,
		INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX,
		CAMERA_WORLD_POSITION,
		CAMERA_VIEW_POSITION,
		//MATRIX_PALETTE,			
		SCENE_AMBIENT_COLOR,
		SCENE_LIGHT_COLOR,
		SCENE_LIGHT_DIRECTION,
		DepthNearFarInvfar,
		//TextureLightDiffuse,
		//TextureLightSpecular,
		TextureLightShadow,
		TextureSceneDiffuse,
		TextureSceneDepth,
		TextureSceneDeviceDepth,
		TextureSceneNormal,
		g_tShadowMap,
		//g_fSplitPlane,
		g_matShadow,
		g_shadowMapTexelSize,
		g_ShadowDepthFade,
	};

	class Parameter;

	class ParameterManager
	{
	public:
		ParameterManager();

		~ParameterManager();

		void SetParameterAutoBinding(Uniform* pParam,AutoBinding autoBinding);

		void UseDefaultBing(Uniform* pParam);

	private:
		// Internal auto binding handler methods.
		const Matrix4&		autoBindingGetWorldMatrix() const;
		const Matrix4&		autoBindingGetViewMatrix() const;
		const Matrix4&		autoBindingGetProjectionMatrix() const;
		Matrix4				autoBindingGetWorldViewMatrix() const;
		Matrix4				autoBindingGetViewProjectionMatrix() const;
		Matrix4				autoBindingGetWorldViewProjectionMatrix() const;
		const Matrix4&		autoBindingGetInverseTransposeWorldMatrix() const;
		const Matrix4&		autoBindingGetInverseTransposeWorldViewMatrix() const;
		Matrix4				autoBindingGetInverseProjectionMatrix() const;
		Vector3				autoBindingGetCameraWorldPosition() const;
		Vector3				autoBindingGetCameraViewPosition() const;
		const ColourValue&	autoBindingGetAmbientColor() const;
		const ColourValue&	autoBindingGetLightColor() const;
		Vector3				autoBindingGetLightDirection() const;
		Vector4				autoBingingDepthNearFarInvfar() const;
		Texture*			autoBingingSceneDiffuse() const;
		Texture*			autoBingingSceneDetph() const;
		Texture*			autoBindingSceneNormal() const;
		Texture*			autoBindingTextureLightShadow() const;
		Texture*			autoBindingShadowMap() const;
		Vector4				autoBindingSpitPos() const;
		UINT				autoBindingSplitCount() const;
		const Matrix4&		autoBindingShadowMatrix() const;
		Vector4				autoBindingShadowMapTexSize() const;
		Vector4				autoBindingShadowDepthFade() const;

	private:	
		std::map<std::string, AutoBinding>	m_autoDefaultBings;
	};

	extern ParameterManager* g_pParameterManager;
	ParameterManager*	GetParameterManager();		

}

#endif
