#include "MaterialData.h"


namespace ma
{
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

	MaterialData::MaterialData(const char* pszPath/* = NULL*/):Resource(pszPath)
	{
	}

	MaterialData::~MaterialData()
	{

	}

	void MaterialData::AddSubMatData(const SubMaterialData& subMatData)
	{
		m_arrSubMatData.push_back(subMatData);
	}

	void MaterialData::SaveToFile(const char* pszPath)
	{
		std::string strSavePath = pszPath ? pszPath : m_sResPath;

		XMLOutputSerializer ar;
		bool bLoadOK = ar.Open(strSavePath.c_str());
		if (!bLoadOK)
		{
			ASSERT(false && "Fail to save to file");
			return ;
		}

		Serialize(ar);

		ar.Close();
	}

	bool MaterialData::CreateFromMemeory()
	{
		ASSERT(m_eResState == ResLoaded);
		if (m_eResState != ResLoaded)
			return false;

		XMLInputSerializer arIn;
		bool bLoadOk = arIn.Open(m_pDataStream.get());
		if (!bLoadOk)
		{
			ASSERT(false && "Fail to Load to file");
			return  false;
		}

 		Serialize(arIn);
 
 		arIn.Close();

		m_eResState = ResInited;

		return true;
	}


	void MaterialData::Serialize(Serializer& sl, const char* pszLable/* = "Material"*/)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_arrSubMatData,"SubMaterial");
		
		sl.EndSection();
	}
}
