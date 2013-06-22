# ifndef MATERIAL_H_
# define MATERIAL_H_

//#include "RenderState.h"
//#include "Technique.h"
//#include "Properties.h"

namespace ma
{
	class MaterialParameter;
	class Camera;
	class Light;
	class Renderable;

	struct RenderState 
    {
		bool				m_bDepthWrite;
		//bool				m_bDepthTest;
        DEPTH_CHECK_MODE	m_eDepthCheckMode;
		BLEND_MODE			m_eBlendMode;
		CULL_MODE			m_eCullMode;

		RenderState()
		{
			m_bDepthWrite = true;
			//m_bDepthTest = true;
			m_eDepthCheckMode = DCM_LESS_EQUAL;
			m_eBlendMode = BM_OPATICY;
			m_eCullMode = CULL_FACE_SIDE_BACK;
		}
    };

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
		CAMERA_WORLD_POSITION,
		CAMERA_VIEW_POSITION,
		MATRIX_PALETTE,			// matrix palette of MeshSkin
		SCENE_AMBIENT_COLOR,
		SCENE_LIGHT_COLOR,
		SCENE_LIGHT_DIRECTION
	};


	class ENGINE_API Material
	{
	public:

		Material();

		~Material();

		void				Bind();

		void				UnBind();

		MaterialParameter*	GetParameter(const char* name);

		void				ClearParameter(const char* name);

		void				setParameterAutoBinding(const char* name, AutoBinding autoBinding);

		void				SetShaderProgram(const char* pszName,const char* define);

		RenderState*		GetRenderState()  {return &m_renderState;}

		ShaderProgram*		GetShaderProgram() {return m_pShaderProgram;}

		void				SetRenderable(Renderable* pRenderable) {m_pRenderable = pRenderable;}

		static void			SetAuotBingCamera(Camera* pCamera) {m_auotBingCamera = pCamera;}

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
		Vector3				autoBindingGetCameraWorldPosition() const;
		Vector3				autoBindingGetCameraViewPosition() const;
		const Matrix4x4*	autoBindingGetMatrixPalette() const;
		unsigned int		autoBindingGetMatrixPaletteSize() const;
		const Vector3&		autoBindingGetAmbientColor() const;
		const Vector3&		autoBindingGetLightColor() const;
		const Vector3&		autoBindingGetLightDirection() const;

	private:

		RenderState			m_renderState;

		ShaderProgram*		m_pShaderProgram;

		std::string			m_strShaderName;

		std::string			m_strShaderDefine;

		Renderable*			m_pRenderable;
			
		std::vector<MaterialParameter*> m_parameters;

		
		// Map of parameter names to auto binding strings. 
		static std::map<std::string, AutoBinding> m_autoDefaultBings;

		static Camera*  m_auotBingCamera;
		static Light*	m_autoBingLight;
	};

}

#endif
