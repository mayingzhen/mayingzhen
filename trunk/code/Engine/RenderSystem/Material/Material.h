# ifndef MATERIAL_H_
# define MATERIAL_H_


namespace ma
{
	class MaterialParameter;
	class Camera;
	class Light;
	struct Renderable;
	class Technique;
	struct Uniform;

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
	};


	class ENGINE_API Material
	{
	public:

		Material(const char* pMaterialFlag,const char* pShaderName = NULL);

		~Material();

		Technique*			CreateTechnique(const char* pTechName,const char* pShadrName, const char* pDefine = NULL);

		void				Bind();

		void				UnBind();

		MaterialParameter*	GetParameter(const char* name);

		void				ClearParameter(const char* name);

		UINT				GetParameterNumber() {return m_parameters.size();}

		MaterialParameter*	GetParameterByIndex(UINT index) {return m_parameters[index];}

		void				setParameterAutoBinding(const char* name, AutoBinding autoBinding);

		void				SetMaterialFlage(const char* pszFlage) {m_strMaterialFlag = pszFlage;}

		const char*			GetMaterialFlage() {return m_strMaterialFlag.c_str();}

		void				SetRenderable(Renderable* pRenderable) {m_pRenderable = pRenderable;}

		void				UseDefaultBing(Uniform* pUniform);

		Technique*			GetCurTechnqiue() {return m_pCurTechnque;}

		void				SetCurTechnqiue(const char* pShaderName,const char* pDefine);

		static void			SetAuotBingCamera(Camera* pCamera) {m_auotBingCamera = pCamera;}
		static void			SetAutoBingLight(Light* pLight) {m_autoBingLight = pLight;}

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
		unsigned int		autoBindingGetMatrixPaletteSize() const;
		const Vector3&		autoBindingGetAmbientColor() const;
		const Vector3&		autoBindingGetLightColor() const;
		const Vector3&		autoBindingGetLightDirection() const;
		Vector4				autoBingingDepthNearFarInvfar() const;

	private:

		std::vector<Technique*>	m_arrTechnique;

		std::string			m_strMaterialFlag;

		Renderable*			m_pRenderable;
			
		std::vector<MaterialParameter*> m_parameters;

		Technique*				m_pCurTechnque;
		
		// Map of parameter names to auto binding strings. 
		static std::map<std::string, AutoBinding> m_autoDefaultBings;
		static Camera*  m_auotBingCamera;
		static Light*	m_autoBingLight;
	};

}

#endif
