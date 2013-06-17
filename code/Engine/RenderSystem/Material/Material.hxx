//#include "Base.h"
#include "Material.h"
//#include "FileSystem.h"
#include "ShaderProgram.h"
//#include "Technique.h"
//#include "Pass.h"
//#include "Properties.h"
//#include "Node.h"

namespace ma
{
	Material::Material() 
	{
		m_pShaderProgram = NULL; 
	}

	Material::~Material()
	{
		SAFE_DELETE(m_pShaderProgram);
	}

	Material* Material::create(const char* url)
	{
		// Load the material properties from file.
// 		Properties* properties = Properties::create(url);
// 		if (properties == NULL)
// 		{
// 			GP_ERROR("Failed to create material from file.");
// 			return NULL;
// 		}
// 
// 		//Material* material = create((strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace());
// 		SAFE_DELETE(properties);
// 
// 		return material;
		return NULL;
	}


	Material* Material::create(ShaderProgram* pShaderProgram)
	{
		ASSERT(pShaderProgram);

		// Create a new material with a single technique and pass for the given effect.
		Material* material = new Material();

		material->m_pShaderProgram = pShaderProgram;

		return material;
	}

	static bool isMaterialKeyword(const char* str)
	{
		ASSERT(str);

		#define MATERIAL_KEYWORD_COUNT 3
		static const char* reservedKeywords[MATERIAL_KEYWORD_COUNT] =
		{
			"vertexShader",
			"fragmentShader",
			"defines"
		};
		for (unsigned int i = 0; i < MATERIAL_KEYWORD_COUNT; ++i)
		{
			if (strcmp(reservedKeywords[i], str) == 0)
			{
				return true;
			}
		}
		return false;
	}

	static FilterOptions parseTextureFilterMode(const char* str, FilterOptions defaultValue)
	{
		if (str == NULL || strlen(str) == 0)
		{
			GP_ERROR("Texture filter mode string must be non-null and non-empty.");
			return defaultValue;
		}
		else if (strcmp(str, "TFO_POINT") == 0)
		{
			return TFO_POINT;
		}
		else if (strcmp(str, "TFO_BILINEAR") == 0)
		{
			return TFO_BILINEAR;
		}
		else if (strcmp(str, "TFO_TRILINEAR") == 0)
		{
			return TFO_TRILINEAR;
		}
		else if (strcmp(str, "TFO_ANISOTROPIC") == 0)
		{
			return TFO_ANISOTROPIC;
		}
		else
		{
			GP_ERROR("Unsupported texture filter mode string ('%s').", str);
			return defaultValue;
		}
	}

	static Wrap parseTextureWrapMode(const char* str, Wrap defaultValue)
	{
		if (str == NULL || strlen(str) == 0)
		{
			GP_ERROR("Texture wrap mode string must be non-null and non-empty.");
			return defaultValue;
		}
		else if (strcmp(str, "REPEAT") == 0)
		{
			return /*Texture::*/REPEAT;
		}
		else if (strcmp(str, "CLAMP") == 0)
		{
			return /*Texture::*/CLAMP;
		}
		else
		{
			GP_ERROR("Unsupported texture wrap mode string ('%s').", str);
			return defaultValue;
		}
	}

	void Material::Bind()
	{
		m_pShaderProgram->Bind();

		for (UINT i = 0; i < m_parameters.size(); ++i)
		{
			m_parameters[i]->bind(m_pShaderProgram);
		}

		GetRenderDevice()->SetRenderState(m_renderState);
	}

	void Material::UnBind()
	{
		//m_pShaderProgram->UnBind();
	}

	MaterialParameter* Material::getParameter(const char* name) 
	{
		ASSERT(name);

		// Search for an existing parameter with this name.
		MaterialParameter* param;
		for (size_t i = 0, count = m_parameters.size(); i < count; ++i)
		{
			param = m_parameters[i];
			ASSERT(param);
			if (strcmp(param->getName(), name) == 0)
			{
				return param;
			}
		}

		// Create a new parameter and store it in our list.
		param = new MaterialParameter(name);
		m_parameters.push_back(param);

		return param;
	}

	void Material::clearParameter(const char* name)
	{
		for (size_t i = 0, count = m_parameters.size(); i < count; ++i)
		{
			MaterialParameter* p = m_parameters[i];
			if (p->_name == name)
			{
				m_parameters.erase(m_parameters.begin() + i);
				//SAFE_RELEASE(p);
				break;
			}
		}
	}

	/**
	 * @script{ignore}
	 */
	const char* autoBindingToString(AutoBinding autoBinding)
	{
		// NOTE: As new AutoBinding values are added, this switch statement must be updatd.
		switch (autoBinding)
		{
		case NONE:
			return NULL;

		case VIEW_MATRIX:
			return "VIEW_MATRIX";

		case PROJECTION_MATRIX:
			return "PROJECTION_MATRIX";

		case WORLD_VIEW_MATRIX:
			return "WORLD_VIEW_MATRIX";

		case VIEW_PROJECTION_MATRIX:
			return "VIEW_PROJECTION_MATRIX";

		case WORLD_VIEW_PROJECTION_MATRIX:
			return "WORLD_VIEW_PROJECTION_MATRIX";

		case INVERSE_TRANSPOSE_WORLD_MATRIX:
			return "INVERSE_TRANSPOSE_WORLD_MATRIX";

		case INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX:
			return "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX";

		case CAMERA_WORLD_POSITION:
			return "CAMERA_WORLD_POSITION";

		case CAMERA_VIEW_POSITION:
			return "CAMERA_VIEW_POSITION";

		case MATRIX_PALETTE:
			return "MATRIX_PALETTE";

		case SCENE_AMBIENT_COLOR:
			return "SCENE_AMBIENT_COLOR";

		case SCENE_LIGHT_COLOR:
			return "SCENE_LIGHT_COLOR";

		case SCENE_LIGHT_DIRECTION:
			return "SCENE_LIGHT_DIRECTION";

		default:
			return "";
		}
	}

	void Material::setParameterAutoBinding(const char* name, AutoBinding autoBinding)
	{
		setParameterAutoBinding(name, autoBindingToString(autoBinding));
	}

	void Material::setParameterAutoBinding(const char* name, const char* autoBinding)
	{
		ASSERT(name);
		ASSERT(autoBinding);

		if (autoBinding == NULL)
		{
			// Remove an existing auto-binding
			std::map<std::string, std::string>::iterator itr = _autoBindings.find(name);
			if (itr != _autoBindings.end())
				_autoBindings.erase(itr);
		}
		else
		{
			// Add/update an auto-binding
			_autoBindings[name] = autoBinding;
		}

		// If we already have a node binding set, pass it to our handler now
	//     if (_nodeBinding)
	//     {
	//         applyAutoBinding(name, autoBinding);
	//     }
	}

}
