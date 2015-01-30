#include "MaterialParameter.h"


namespace ma
{
	Parameter::Parameter()
	{
	}

	Parameter::~Parameter()
	{	
	}

	const char* Parameter::GetName() const
	{
		return m_sName.c_str();
	}

	void Parameter::SetName(const char* pName)
	{
		m_sName = pName ? pName : "";
	}

	std::string GetStrMateParmType(const Any& vale)
	{
		std::string strType = GetStrTypeAny(vale);
		if (strType != "")
			return strType;

		const std::type_info& type = vale.getType();
		if (type == typeid(RefPtr<Texture>))
			return "SamplerState";
		//else if (type == typeid(RefPtr<SamplerState>))
		//	return "SamplerState";
		else
		{
			return "";
		}
	}

	void Parameter::Serialize(Serializer& sl, const char* pszLable/* = "Parameter"*/)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_sName,"name");

		std::string strType = GetStrMateParmType(m_anyValue);

		SerializeAnyValue(sl,strType,m_anyValue);

		if (strType == "SamplerState")
		{
			Wrap eWrap = REPEAT;
			FilterOptions eFilter = TFO_POINT;
			string strIamgePath;
			if (sl.IsReading())
			{
				sl.Serialize(strIamgePath,"ImagePath");
 				sl.Serialize(eFilter,strFilterOptions,"Filter");
 				sl.Serialize(eWrap,strWrap,"Wrap");

				m_anyValue = Any( CreateSamplerState(strIamgePath.c_str(),eWrap,eFilter) );
			}
			else
			{
				RefPtr<Texture> pTexture = any_cast< RefPtr<Texture> >(m_anyValue);

				eWrap = pTexture->GetWrapMode();
				FilterOptions eFilter = pTexture->GetFilterMode();
				string strIamgePath = pTexture->GetImagePath();

				sl.Serialize(strIamgePath,"ImagePath");
				sl.Serialize(eFilter,strFilterOptions,"Filter");
				sl.Serialize(eWrap,strWrap,"Wrap");
			}
		}

		sl.EndSection();
	}

}
