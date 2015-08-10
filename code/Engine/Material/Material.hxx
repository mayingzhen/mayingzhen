#include "Material.h"
#include "ShaderProgram.h"

namespace ma
{

	static const char* ShadowDepth = "ShadowDepth";
	static const char* Shading = "Shading";
	
	SubMaterial::SubMaterial()
	{
	}

	SubMaterial::~SubMaterial()
	{
		m_pShadingTech = NULL;
	}

	void SubMaterial::SetShadingTechnqiue(Technique* pTech)
	{
		m_pShadingTech = pTech;
	}

	void SubMaterial::SetShadingTechnqiue(const char* pShaderName, const char* pDefine)
	{
		m_strShaderName = pShaderName;
		m_strShaderMacro = pDefine;
		m_pShadingTech = CreateTechnique(Shading, pShaderName, pShaderName, pDefine);
	}

	Technique* SubMaterial::GetShadowDepthTechnqiue()
	{
		if (m_pShadowDepthTech == NULL)
		{
			string strShaderMacro = m_pShadingTech->GetShaderProgram()->GetShaderMacro();
			m_pShadowDepthTech = CreateTechnique(ShadowDepth, ShadowDepth, ShadowDepth, strShaderMacro.c_str());
			//m_pShadowDepthTech->GetRenderState().m_eCullMode = CULL_FACE_SIDE_FRONT;
		
			if (GetDeviceCapabilities()->GetDepthTextureSupported())
			{
				m_pShadowDepthTech->GetRenderState().m_bColorWrite = false;
			}
		}

		return m_pShadowDepthTech.get();
	}

	void SubMaterial::SetShadowDepthTechnqiue(Technique* pTech)
	{
		m_pShadowDepthTech = pTech;
	}

	void SubMaterial::SetShadowDepthTechnqiue(const char* pShaderName,const char* pDefine)
	{
		string strMacro = pDefine ? pDefine : "";
		strMacro += m_pShadingTech->GetShaderProgram()->GetShaderMacro();
		m_pShadowDepthTech = CreateTechnique(ShadowDepth, pShaderName, pShaderName, strMacro.c_str());
	}

	Technique* SubMaterial::GetShadingTechnqiue()
	{
		return m_pShadingTech.get();
	}

	void SubMaterial::Serialize(Serializer& sl, const char* pszLable/* = "SubMaterial"*/)
	{
		sl.BeginSection(pszLable);

		sl.BeginSection("Shader");
		sl.Serialize(m_strShaderName,"Name");
		sl.Serialize(m_strShaderMacro,"Macro");
		sl.EndSection();

		sl.Serialize(m_renderState,"RenderState");

		sl.Serialize(m_arrParameters,"Parameters");

		sl.EndSection();

		if (sl.IsReading())
		{
			SetShadingTechnqiue(m_strShaderName.c_str(),m_strShaderMacro.c_str());
			m_pShadingTech->SetRenderState(m_renderState);
			for (uint32 i = 0; i < m_arrParameters.size(); ++i)
			{
				m_pShadingTech->SetParameter(m_arrParameters[i].GetName(),m_arrParameters[i].GetValue());
			}
		}
	}

	void SubMaterial::SetShaderName(const char* pszSharName)
	{
		m_strShaderName = pszSharName ? pszSharName : "";
	}

	void SubMaterial::SetShderMacro(const char* pszShaderMacro)
	{
		m_strShaderMacro = pszShaderMacro ? pszShaderMacro : "";
	}

	void SubMaterial::AddParameter(const char* pName,Any value)
	{
		Parameter matParam;
		matParam.SetName(pName);
		matParam.SetValue(value);
		m_arrParameters.push_back(matParam);
	}

	RefPtr<SubMaterial> SubMaterial::Clone()
	{
		SubMaterial* pClonMaterial = new SubMaterial();

		if (m_pShadingTech)
			pClonMaterial->m_pShadingTech = m_pShadingTech->Clone();

		if (m_pShadowDepthTech)
		{
			pClonMaterial->m_pShadowDepthTech = m_pShadowDepthTech->Clone();
		}
		pClonMaterial->m_strShaderMacro = m_strShaderMacro;

		return pClonMaterial;
	}


	RefPtr<SubMaterial> CreateSubMaterial()
	{
		SubMaterial* pMaterial = new SubMaterial();

		return pMaterial;
	}


	IMPL_OBJECT(Material,Serializable);

	Material::Material()
	{
	}

	Material::~Material()
	{
	}

	void Material::AddSubMaterial(SubMaterial* pSubMaterial)
	{
		m_arrSubMaterial.push_back(pSubMaterial);
	}

	void Material::Serialize(Serializer& sl, const char* pszLable/* = "Material"*/)
	{
		sl.Serialize(m_arrSubMaterial,pszLable);
	}

	RefPtr<Material> CreateMaterial()
	{
		return new Material();
	}

	RefPtr<Material> CreateMaterial(const char* pszPath)
	{
		RefPtr<Material> pMaterial = LoadResource<Material>(pszPath);
		return pMaterial;
	}


}
