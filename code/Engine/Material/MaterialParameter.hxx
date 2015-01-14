#include "MaterialParameter.h"


namespace ma
{
	IMPL_OBJECT(MaterialParameter,Serializable)
	
	MaterialParameter::MaterialParameter()
	{
	}

	MaterialParameter::~MaterialParameter()
	{	
	}

	const char* MaterialParameter::GetName() const
	{
		return m_sName.c_str();
	}

	void MaterialParameter::SetName(const char* pName)
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
			return "Texture";
		else if (type == typeid(RefPtr<SamplerState>))
			return "SamplerState";
		else
		{
			return "";
		}
	}

	void MaterialParameter::Serialize(Serializer& sl, const char* pszLable/* = "Parameter"*/)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(m_sName,"name");

		std::string strType = GetStrMateParmType(m_anyValue);

		SerializeAnyValue(sl,strType,m_anyValue);
		
		if (strType == "SamplerState")
		{
			if ( sl.IsReading() )
			{
				RefPtr<SamplerState> pSampler = CreateSamplerState();
				pSampler->Serialize(sl,"Value");
				m_anyValue = Any(pSampler);
			}
			else
			{
				RefPtr<SamplerState> pSampler = any_cast< RefPtr<SamplerState> >(m_anyValue);
				pSampler->Serialize(sl,"Value");	
			}
		}	
		else if (strType == "Texture")
		{
			if (sl.IsReading())
			{
				string strValue;
				sl.Serialize(strValue,"Value");
				m_anyValue = Any( CreateTexture( strValue.c_str() ) );
			}
			else
			{
				RefPtr<Texture> pTexure = any_cast< RefPtr<Texture> >(m_anyValue);
				string strValue = pTexure->GetResPath();
				sl.Serialize(strValue,"Value");
			}
		}

		sl.EndSection();
	}


	void MaterialParameter::Bind(Uniform* pUniform)
	{
		ASSERT(pUniform);
		if (pUniform == NULL)
			return;
	
		const std::type_info& type = m_anyValue.getType();
		if (type == typeid(float))
		{
			const float* value = any_cast<float>(&m_anyValue);
			GetRenderSystem()->SetValue(pUniform, *value);
		}
		else if (type == typeid(Vector2))
		{
			const Vector2* value = any_cast<Vector2>(&m_anyValue);
			GetRenderSystem()->SetValue(pUniform, *value);
		}
		else if (type == typeid(Vector3))
		{
			const Vector3* value = any_cast<Vector3>(&m_anyValue);
			GetRenderSystem()->SetValue(pUniform, *value);
		}
		else if (type == typeid(Vector4))
		{
			const Vector4* value = any_cast<Vector4>(&m_anyValue);
			GetRenderSystem()->SetValue(pUniform, *value);
		}
		else if (type == typeid(ColourValue))
		{
			ColourValue cColor = any_cast<ColourValue>(m_anyValue);
			Vector4 vColor(cColor.r,cColor.g,cColor.b,cColor.a);
			GetRenderSystem()->SetValue(pUniform,vColor);
		}
		else if (type == typeid(Matrix4))
		{
			const Matrix4* value = any_cast<Matrix4>(&m_anyValue);
			GetRenderSystem()->SetValue(pUniform, *value);
		}
		else if (type == typeid(RefPtr<Texture>))
		{
			Texture* pTexture = any_cast< RefPtr<Texture> >(&m_anyValue)->get();
			GetRenderSystem()->SetValue(pUniform,pTexture);
		}
		else if (type == typeid(SamplerStatePtr))
		{
			SamplerState* pSampler = any_cast< RefPtr<SamplerState> >(&m_anyValue)->get();
			GetRenderSystem()->SetValue(pUniform,pSampler);
		}
		else
		{
			ASSERT(false);
		}	
	}
}
