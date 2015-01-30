# ifndef _ParameterManager_H_
# define _ParameterManager_H_


namespace ma
{

    enum AutoBinding
	{
		NONE,
		g_matWorld,
		g_matView,
		g_matProj,
		g_matWorldView,
		g_matViewProj,
		g_matWorldViewProj,
		INVERSE_TRANSPOSE_WORLD_MATRIX,
		INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX,
		INVERSE_PROJECTION_MATRIX,
		CAMERA_WORLD_POSITION,
		CAMERA_VIEW_POSITION,
		MATRIX_PALETTE,			
		SCENE_AMBIENT_COLOR,
		SCENE_LIGHT_COLOR,
		SCENE_LIGHT_DIRECTION,
		DepthNearFarInvfar,
		TextureLightDiffuse,
		TextureLightSpecular,
		TextureLightShadow,
		TextureSceneDepth,
		TextureSceneNormal,
		g_tShadowMap,
		g_fSplitPlane,
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
		const Vector4*		autoBindingGetMatrixPalette() const;
		UINT				autoBindingGetMatrixPaletteSize() const;
		const ColourValue&	autoBindingGetAmbientColor() const;
		const ColourValue&	autoBindingGetLightColor() const;
		Vector3				autoBindingGetLightDirection() const;
		Vector4				autoBingingDepthNearFarInvfar() const;
		Texture*			autoBingingSceneDetph() const;
		Texture*			autoBindingSceneNormal() const;
		Texture*			autoBindingTextureLightDiffuse() const;
		Texture*			autoBindingTextureightSpecular() const;
		Texture*			autoBindingTextureLightShadow() const;

	private:	
		std::map<std::string, AutoBinding>	m_autoDefaultBings;
	};

	ParameterManager*	GetParameterManager();
	void				SetParameterManager(ParameterManager* pParameterManager);		

}

#endif
