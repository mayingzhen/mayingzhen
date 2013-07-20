#include "Material.h"
#include "ShaderProgram.h"


namespace ma
{
	std::map<std::string, AutoBinding> Material::m_autoDefaultBings;
	Camera* Material::m_auotBingCamera = NULL;
	Light*	Material::m_autoBingLight = NULL;

	Material::Material(const char* pMaterialFlag,const char* pShaderName) 
	{
		if (m_autoDefaultBings.empty())
		{
			m_autoDefaultBings["u_worldViewProjectionMatrix"] = WORLD_VIEW_PROJECTION_MATRIX;
			m_autoDefaultBings["u_worldviewMatrix"] = WORLD_VIEW_MATRIX;
			m_autoDefaultBings["u_matrixPalette"] = MATRIX_PALETTE;
			m_autoDefaultBings["depth_near_far_invfar"] = DepthNearFarInvfar;
			m_autoDefaultBings["u_InvProjMatrix"] =  INVERSE_PROJECTION_MATRIX;
			
		}

		m_pRenderable = NULL;
		m_pCurTechnque = NULL;

		m_strMaterialFlag = pMaterialFlag;
		
		if (pShaderName)
		{
			m_pCurTechnque = CreateTechnique(pShaderName,pShaderName);
		}
			

	}

	Material::~Material()
	{
		//SAFE_DELETE(m_pShaderProgram);
	}

	Technique*	Material::CreateTechnique(const char* pTechName,const char* pShadrName, const char* pDefine)
	{
		ASSERT(pTechName);
		if (pTechName == NULL)
			return NULL;

		Technique* pTechnique = new Technique(this,pTechName);
		m_arrTechnique.push_back(pTechnique);
		pTechnique->CreateShaderProgram(pShadrName,pDefine);
		return pTechnique;
	}

	void Material::SetCurTechnqiue(const char* pShaderName,const char* pDefine)
	{
		std::string sTechName = pShaderName;
		if (pDefine && strcmp(pDefine,"") != 0)
		{
			sTechName += std::string("_") + pDefine;
		}

		for (Uint i = 0; i < m_arrTechnique.size(); ++i)
		{
			Technique* pTech = m_arrTechnique[i];
			ASSERT(pTech);
			if (pTech == NULL)
				continue;

			if ( strcmp(sTechName.c_str(),pTech->GetTechName()) == 0 )
			{
				m_pCurTechnque = m_arrTechnique[i];
				return;
			}
		}

		Technique* pTech = CreateTechnique(sTechName.c_str(),pShaderName,pDefine);
		m_pCurTechnque = pTech;

		//ASSERT(false);
	}


// 	Material* Material::create(const char* url)
// 	{
// 		// Load the material properties from file.
// // 		Properties* properties = Properties::create(url);
// // 		if (properties == NULL)
// // 		{
// // 			GP_ERROR("Failed to create material from file.");
// // 			return NULL;
// // 		}
// // 
// // 		//Material* material = create((strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace());
// // 		SAFE_DELETE(properties);
// // 
// // 		return material;
// 		return NULL;
// 	}


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
		ASSERT(m_pCurTechnque);
		if (m_pCurTechnque == NULL)
			return;

		m_pCurTechnque->Bind();

		for (UINT i = 0; i < m_parameters.size(); ++i)
		{
			m_parameters[i]->bind(m_pCurTechnque->GetShaderProgram());
		}
	}

	void Material::UnBind()
	{
		//m_pShaderProgram->UnBind();
	}

	MaterialParameter* Material::GetParameter(const char* name) 
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

	void Material::ClearParameter(const char* name)
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

// 	void Material::setParameterAutoBinding(const char* name, AutoBinding autoBinding)
// 	{
// 		ASSERT(name);
// 	
// 		if (autoBinding == NONE)
// 		{
// 			// Remove an existing auto-binding
// 			std::map<std::string, AutoBinding>::iterator itr = m_autoBindings.find(name);
// 			if (itr != m_autoBindings.end())
// 				m_autoBindings.erase(itr);
// 		}
// 		else
// 		{
// 			// Add/update an auto-binding
// 			m_autoBindings[name] = autoBinding;
// 		}
// 	}

	void Material::UseDefaultBing(Uniform* pUniform)
	{
		std::map<std::string, AutoBinding>::iterator itr =  m_autoDefaultBings.find(pUniform->getName());
		if (itr != m_autoDefaultBings.end())
		{
			setParameterAutoBinding(pUniform->getName(),itr->second);
		}
	}

	void Material::setParameterAutoBinding(const char* uniformName,AutoBinding autoBinding)
	{
		MaterialParameter* param = GetParameter(uniformName);
		ASSERT(param);

		if (autoBinding == WORLD_MATRIX)
		{
			param->bindValue(this, &Material::autoBindingGetWorldMatrix);
		}
		else if (autoBinding == VIEW_MATRIX)
		{
			param->bindValue(this, &Material::autoBindingGetViewMatrix);
		}
		else if (autoBinding == PROJECTION_MATRIX)
		{
			param->bindValue(this, &Material::autoBindingGetProjectionMatrix);
		}
		else if (autoBinding == WORLD_VIEW_MATRIX)
		{
			param->bindValue(this, &Material::autoBindingGetWorldViewMatrix);
		}
		else if (autoBinding == VIEW_PROJECTION_MATRIX)
		{
			param->bindValue(this, &Material::autoBindingGetViewProjectionMatrix);
		}
		else if (autoBinding == WORLD_VIEW_PROJECTION_MATRIX)
		{
			param->bindValue(this, &Material::autoBindingGetWorldViewProjectionMatrix);
		}
		else if (autoBinding == INVERSE_TRANSPOSE_WORLD_MATRIX)
		{
			param->bindValue(this, &Material::autoBindingGetInverseTransposeWorldMatrix);
		}
		else if (autoBinding == INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX)
		{
			param->bindValue(this, &Material::autoBindingGetInverseTransposeWorldViewMatrix);
		}
		else if (autoBinding == INVERSE_PROJECTION_MATRIX)
		{
			param->bindValue(this,&Material::autoBindingGetInverseProjectionMatrix);
		}
		else if (autoBinding == CAMERA_WORLD_POSITION)
		{
			param->bindValue(this, &Material::autoBindingGetCameraWorldPosition);
		}
		else if (autoBinding == CAMERA_VIEW_POSITION)
		{
			param->bindValue(this, &Material::autoBindingGetCameraViewPosition);
		}
		else if (autoBinding == MATRIX_PALETTE)
		{
			param->bindValue(this, &Material::autoBindingGetMatrixPalette, &Material::autoBindingGetMatrixPaletteSize);
		}
		else if (autoBinding == SCENE_AMBIENT_COLOR)
		{
			param->bindValue(this, &Material::autoBindingGetAmbientColor);
		}
		else if (autoBinding == SCENE_LIGHT_COLOR)
		{
			param->bindValue(this, &Material::autoBindingGetLightColor);
		}
		else if (autoBinding == SCENE_LIGHT_DIRECTION)
		{
			param->bindValue(this, &Material::autoBindingGetLightDirection);
		}
		else if (autoBinding == DepthNearFarInvfar)
		{
			param->bindValue(this, &Material::autoBingingDepthNearFarInvfar);
		}
		else
		{
			ASSERT("Unsupported auto binding type " && autoBinding);
		}
	}

	const Matrix4x4& Material::autoBindingGetWorldMatrix() const
	{
		return m_pRenderable ? m_pRenderable->m_matWorld : Matrix4x4::identity();
	}

	const Matrix4x4& Material::autoBindingGetViewMatrix() const
	{
		return m_auotBingCamera ? m_auotBingCamera->GetViewMatrix() : Matrix4x4::identity();
	}

	const Matrix4x4& Material::autoBindingGetProjectionMatrix() const
	{
		return m_auotBingCamera ? m_auotBingCamera->GetProjMatrix() : Matrix4x4::identity();
	}

	Matrix4x4 Material::autoBindingGetWorldViewMatrix() const
	{
		return m_pRenderable && m_auotBingCamera ? m_pRenderable->m_matWorld * m_auotBingCamera->GetViewMatrix(): Matrix4x4::identity();
	}

	Matrix4x4 Material::autoBindingGetViewProjectionMatrix() const
	{
		return m_auotBingCamera ? m_auotBingCamera->GetViewProjMatrix() : Matrix4x4::identity();
	}

	Matrix4x4 Material::autoBindingGetWorldViewProjectionMatrix() const
	{
		ASSERT(m_pRenderable);
		return m_pRenderable && m_auotBingCamera ? m_pRenderable->m_matWorld * m_auotBingCamera->GetViewProjMatrix() : Matrix4x4::identity();
	}

	const Matrix4x4& Material::autoBindingGetInverseTransposeWorldMatrix() const
	{
		return /*_nodeBinding ? _nodeBinding->getInverseTransposeWorldMatrix() :*/ Matrix4x4::identity();
	}

	const Matrix4x4& Material::autoBindingGetInverseTransposeWorldViewMatrix() const
	{
		return /*_nodeBinding ? _nodeBinding->getInverseTransposeWorldViewMatrix() :*/ Matrix4x4::identity();
	}
		
	Matrix4x4 Material::autoBindingGetInverseProjectionMatrix() const
	{
		if (m_auotBingCamera)
		{
			Matrix4x4 mInvProj;
			Matrix4x4 matProj = m_auotBingCamera->GetProjMatrix();
			MatrixInverse(&mInvProj, NULL, &matProj);
			return mInvProj;
		}
		else
		{
			return Matrix4x4::identity();
		}
	}

	Vector3 Material::autoBindingGetCameraWorldPosition() const
	{
		return m_auotBingCamera ? m_auotBingCamera->GetTransform().m_vPos : Vec3Zero();
	}

	Vector3 Material::autoBindingGetCameraViewPosition() const
	{
		return /*_nodeBinding ? _nodeBinding->getActiveCameraTranslationView() :*/ Vec3Zero();
	}

	const Matrix4x4* Material::autoBindingGetMatrixPalette() const
	{
		if (m_pRenderable == NULL)
			return NULL;

		if ( m_pRenderable->m_arrSkinMatrix.empty() )
			return NULL;
		
		return &m_pRenderable->m_arrSkinMatrix[0];
	}

	unsigned int Material::autoBindingGetMatrixPaletteSize() const
	{
		return m_pRenderable ? m_pRenderable->m_arrSkinMatrix.size() : 0;
	}

	const Vector3& Material::autoBindingGetAmbientColor() const
	{
		return /*m_pLight ? scene->getAmbientColor() :*/ Vec3Zero();
	}

	const Vector3& Material::autoBindingGetLightColor() const
	{
		return m_autoBingLight ? m_autoBingLight->GetDiffuse() : Vec3One();
	}

	const Vector3& Material::autoBindingGetLightDirection() const
	{
		return m_autoBingLight ? m_autoBingLight->GetDirection() : Vec3One();
	}

	Vector4 Material::autoBingingDepthNearFarInvfar() const
	{
		if (m_auotBingCamera == NULL)
			return Vec4One();
		
		float fNear = m_auotBingCamera->GetNearClip();
		float fFar = m_auotBingCamera->GetFarClip();
		
		return Vector4(fNear,fFar,1/fFar,0);
	}

}
