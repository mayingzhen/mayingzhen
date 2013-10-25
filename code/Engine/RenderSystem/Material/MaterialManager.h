# ifndef _MaterialManager_H_
# define _MaterialManager_H_


namespace ma
{

    enum AutoBinding
	{
		NONE,
		WORLD_MATRIX,
		VIEW_MATRIX,
		PROJECTION_MATRIX,
		WORLD_VIEW_MATRIX,
		VIEW_PROJECTION_MATRIX,
		WORLD_VIEW_PROJECTION_MATRIX,
		INVERSE_TRANSPOSE_WORLD_MATRIX,
		INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX,
		INVERSE_PROJECTION_MATRIX,
		CAMERA_WORLD_POSITION,
		CAMERA_VIEW_POSITION,
		MATRIX_PALETTE,			// matrix palette of MeshSkin
		SCENE_AMBIENT_COLOR,
		SCENE_LIGHT_COLOR,
		SCENE_LIGHT_DIRECTION,
		DepthNearFarInvfar,
		TextureLightDiffuse,
		TextureLightSpecular,
		TextureLightShadow,
		TextureSceneDepth,
		TextureSceneNormal,
	};

	class MaterialParameter;

	class MaterialManager
	{
	public:
		MaterialManager();

		~MaterialManager();

		void SetParameterAutoBinding(MaterialParameter* pParam,AutoBinding autoBinding);

		void UseDefaultBing(MaterialParameter* pParam);

		void SetCurRenderable(Renderable* pRenderable) {m_pCurRenderable = pRenderable;}

		void SetCurLight(Light* pLight) {m_pCurLight = pLight;}

		void SetCurViewPojectMat(const Matrix4x4& matView,const Matrix4x4& matProj); 

	private:
		// Internal auto binding handler methods.
		const Matrix4x4&	autoBindingGetWorldMatrix() const;
		const Matrix4x4&	autoBindingGetViewMatrix() const;
		const Matrix4x4&	autoBindingGetProjectionMatrix() const;
		Matrix4x4			autoBindingGetWorldViewMatrix() const;
		Matrix4x4			autoBindingGetViewProjectionMatrix() const;
		Matrix4x4			autoBindingGetWorldViewProjectionMatrix() const;
		const Matrix4x4&	autoBindingGetInverseTransposeWorldMatrix() const;
		const Matrix4x4&	autoBindingGetInverseTransposeWorldViewMatrix() const;
		Matrix4x4			autoBindingGetInverseProjectionMatrix() const;
		Vector3				autoBindingGetCameraWorldPosition() const;
		Vector3				autoBindingGetCameraViewPosition() const;
		const Matrix4x4*	autoBindingGetMatrixPalette() const;
		UINT				autoBindingGetMatrixPaletteSize() const;
		const Vector4&		autoBindingGetAmbientColor() const;
		const Vector4&		autoBindingGetLightColor() const;
		const Vector3&		autoBindingGetLightDirection() const;
		Vector4				autoBingingDepthNearFarInvfar() const;
		Texture*			autoBingingSceneDetph() const;
		Texture*			autoBindingSceneNormal() const;
		Texture*			autoBindingTextureLightDiffuse() const;
		Texture*			autoBindingTextureightSpecular() const;
		Texture*			autoBindingTextureLightShadow() const;

	private:	
		std::map<std::string, AutoBinding>	m_autoDefaultBings;

		Renderable*							m_pCurRenderable;
		Light*								m_pCurLight;							
	
		Matrix4x4							m_matView;
		Matrix4x4							m_matProj;	
	};

	MaterialManager*	GetMaterialManager();
	void				SetMaterialManager(MaterialManager* pMaterialMang);		

}

#endif
