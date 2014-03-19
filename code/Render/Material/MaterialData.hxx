#include "MaterialData.h"


namespace ma
{
	IMPL_OBJECT(MaterialData,Resource);

	MaterialData::MaterialData(const char* pszPath)
	{
		m_pTechnique = NULL;
	}

	MaterialData::~MaterialData()
	{
		SAFE_DELETE(m_pTechnique);
		for (UINT i = 0; i < m_arrParameters.size(); ++i)
		{
			SAFE_DELETE(m_arrParameters[i]);
		}
	}

	void MaterialData::SetShader(const char* pShaderName,const char* pShaderMacro)
	{
		m_strShaderName = pShaderName;
		m_strShaderMacro = pShaderMacro;
		SAFE_DELETE(m_pTechnique);
		m_pTechnique = new Technique("",pShaderName,pShaderName,pShaderMacro);
	}

	void MaterialData::SetParameter(const char* pParmName,Vector4 value)
	{
		ASSERT(m_pTechnique);
		if (m_pTechnique == NULL)
			return;

		MaterialParameter* pMatDataParm = GetParameter(pParmName);
		if (pMatDataParm == NULL)
		{
			MaterialParameter* pMatParm = m_pTechnique->GetParameter(pParmName);
			ASSERT(pMatParm);
			if (pMatParm == NULL)
				return;

			pMatDataParm = new MaterialParameter(pParmName);
			m_arrParameters.push_back(pMatDataParm);
		}

		pMatDataParm->setVector4(value);
	}

	void MaterialData::SetParameter(const char* pParmName,SamplerState& sampler)
	{
		ASSERT(m_pTechnique);
		if (m_pTechnique == NULL)
			return;

		MaterialParameter* pMatDataParm = GetParameter(pParmName);
		if (pMatDataParm == NULL)
		{
			MaterialParameter* pMatParm = m_pTechnique->GetParameter(pParmName);
			ASSERT(pMatParm);
			if (pMatParm == NULL)
				return;

			pMatDataParm = new MaterialParameter(pParmName);
			m_arrParameters.push_back(pMatDataParm);
		}


		pMatDataParm->setSampler(&sampler);
	}

	MaterialParameter* MaterialData::GetParameter(const char* pszName)
	{
		for (UINT i = 0; i < m_arrParameters.size(); ++i)
		{
			if (m_arrParameters[i] && strcmp(m_arrParameters[i]->GetName(), pszName) == 0)
			{
				return m_arrParameters[i];
			}
		}

		return NULL;
	}

	void MaterialData::SaveToFile(const char* pszPath)
	{
		std::string strSavePath = pszPath ? pszPath : m_sResPath;

		XMLOutputArchive ar;
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
		XMLInputArchive arIn;
		bool bLoadOk = arIn.Open(m_pDataStream);
		if (!bLoadOk)
		{
			ASSERT(false && "Fail to Load to file");
			return  false;
		}

 		Serialize(arIn);
 
 		arIn.Close();

		m_eResState = ResLoaded;

		return true;
	}


	void MaterialData::Serialize(Serializer& sl, const char* pszLable/* = "Material"*/)
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
}
