#include "MaterialData.h"


namespace ma
{
	void SubMaterialData::SetShaderName(const char* pName)
	{
		m_strShaderName = pName ? pName : "";
	}

	const char* SubMaterialData::GetShaderName() const
	{
		return m_strShaderName.c_str();
	}

	void SubMaterialData::SetShderMacro(const char* pMacro)
	{
		m_strShaderMacro = pMacro ? pMacro : "";
	}

	const char*	SubMaterialData::GetShaderMacro() const
	{
		return m_strShaderMacro.c_str();
	}

	void SubMaterialData::AddParameter(const char* pName,Any value)
	{
		MaterialParameter matParam;
		matParam.SetName(pName);
		matParam.SetValue(value);
		m_arrParameters.push_back(matParam);
	}

	void SubMaterialData::Serialize(Serializer& sl, const char* pszLable/* = "SubMaterial"*/)
	{
		sl.BeginSection(pszLable);

		sl.BeginSection("Shader");
		sl.Serialize(m_strShaderName,"Name");
		sl.Serialize(m_strShaderMacro,"Macro");
		sl.EndSection();

		sl.Serialize(m_renderState,"RenderState");

		sl.Serialize(m_arrParameters,"Parameters");

		sl.EndSection();
	}

	IMPL_OBJECT(MaterialData,Resource);

	MaterialData::MaterialData(const char* pszPath/* = NULL*/)
		:Resource(pszPath)
	{
		m_eCreateType = XML_RES;
	}

	MaterialData::~MaterialData()
	{

	}

	void MaterialData::AddSubMatData(const SubMaterialData& subMatData)
	{
		m_arrSubMatData.push_back(subMatData);
	}

	SubMaterialData& MaterialData::AddSubMatData()
	{
		m_arrSubMatData.push_back( SubMaterialData() );
		return m_arrSubMatData.back();
	}

	void MaterialData::Serialize(Serializer& sl, const char* pszLable/* = "Material"*/)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_arrSubMatData,"SubMaterial");
		
		sl.EndSection();
	}
}
