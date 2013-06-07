# ifndef MATERIAL_H_
# define MATERIAL_H_

//#include "RenderState.h"
//#include "Technique.h"
//#include "Properties.h"

namespace ma
{
	class MaterialParameter;

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

		/**
		 * Creates a material using the data from the Properties object defined at the specified URL, 
		 * where the URL is of the format "<file-path>.<extension>#<namespace-id>/<namespace-id>/.../<namespace-id>"
		 * (and "#<namespace-id>/<namespace-id>/.../<namespace-id>" is optional). 
		 * 
		 * @param url The URL pointing to the Properties object defining the material.
		 * 
		 * @return A new Material.
		 * @script{create}
		 */
		static Material* create(const char* url);

		/**
		 * Creates a material from the specified effect.
		 *
		 * The returned material has a single technique and a single pass for the
		 * given effect.
		 *
		 * @param effect ShaderProgram for the new material.
		 * 
		 * @return A new Material.
		 * @script{create}
		 */
		static Material* create(ShaderProgram* pShaderProgram);

		void Bind();

		void UnBind();


		 /**
		 * Returns a MaterialParameter for the specified name.
		 * 
		 * The returned MaterialParameter can be used to set values for the specified
		 * parameter name.
		 *
		 * Note that this method causes a new MaterialParameter to be created if one
		 * does not already exist for the given parameter name.
		 *
		 * @param name Material parameter (uniform) name.
		 * 
		 * @return A MaterialParameter for the specified name.
		 */
		MaterialParameter* getParameter(const char* name);


		/**
		 * Clears the MaterialParameter with the given name.
		 *
		 * If a material parameter exists for the given name, it is destroyed and
		 * removed from this RenderState.
		 *
		 * @param name Material parameter (uniform) name.
		 */
		void clearParameter(const char* name);

		/**
		 * Sets a material parameter auto-binding.
		 *
		 * @param name The name of the material parameter to store an auto-binding for.
		 * @param autoBinding A valid AutoBinding value.
		 */
		void setParameterAutoBinding(const char* name, AutoBinding autoBinding);

		/**
		 * Sets a material parameter auto-binding.
		 *
		 * This method parses the passed in autoBinding string and attempts to convert it
		 * to an AutoBinding enumeration value, which is then stored in this render state.
		 *
		 * @param name The name of the material parameter to store an auto-binding for.
		 * @param autoBinding A string matching one of the built-in AutoBinding enum constants.
		 */
		void setParameterAutoBinding(const char* name, const char* autoBinding);

		RenderState*		GetRenderState()  {return &m_renderState;}

		ShaderProgram*		GetShaderProgram() {return m_pShaderProgram;}

	private:

		/**
		 * Constructor.
		 */
		Material();

		/**
		 * Constructor.
		 */
		Material(const Material& m);
	    
		/**
		 * Destructor.
		 */
		~Material();

		/**
		 * Clones this material.
		 * 
		 * @param context The clone context.
		 * 
		 * @return The newly created material.
		 * @script{create}
		 */
		//Material* clone(NodeCloneContext &context) const;


		RenderState		m_renderState;

		ShaderProgram*	m_pShaderProgram;
			
		/**
		* Collection of MaterialParameter's to be applied to the ma::ShaderProgram.
		*/
		std::vector<MaterialParameter*> m_parameters;

		/**
		 * Map of parameter names to auto binding strings.
		 */
		std::map<std::string, std::string> _autoBindings;
	};

}

#endif
