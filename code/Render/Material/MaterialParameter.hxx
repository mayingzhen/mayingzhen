#include "MaterialParameter.h"


namespace ma
{

	MaterialParameter::MaterialParameter(const char* pName)
	{
		m_sName = pName ? pName : "";
		m_pMethod = NULL;
	}

	MaterialParameter::~MaterialParameter()
	{
		SAFE_DELETE(m_pMethod);
	}

	std::string GetStrMateParmType(const Any& vale)
	{
		std::string strType = GetStrTypeAny(vale);
		if (strType != "")
			return strType;

		const std::type_info& type = vale.getType();
		if (type == typeid(RefPtr<Texture>))
			return "TexturePtr";
		else if (type == typeid(RefPtr<SamplerState>))
			return "SamplerStatePtr";
		else if (type == typeid(SamplerState))
			return "SamplerState";
		else
			return "";
	}

	void MaterialParameter::Serialize(Serializer& sl, const char* pszLable/* = "Parameter"*/)
	{
		ASSERT(m_pMethod == NULL);

		sl.BeginSection(pszLable);

		sl.Serialize(m_sName,"name");

		std::string strType = GetStrMateParmType(m_anyValue);

		SerializeAnyValue(sl,strType,m_anyValue);
		
		if (strType == "SamplerState")
		{
			if ( sl.IsReading() )
			{
				SamplerStatePtr pSampler = new SamplerState();
				pSampler->Serialize(sl,"Value");
				m_anyValue = Any(pSampler);
			}
			else
			{
				sl.Serialize(any_cast<SamplerState>(m_anyValue),"Value");		
			}
		}	

		sl.EndSection();
	}

	void MaterialParameter::Bind(Uniform* pUniform)
	{
		ASSERT(pUniform && pUniform->m_pShader);
		if (pUniform == NULL || pUniform->m_pShader == NULL)
			return;
	
		ShaderProgram* pShader = pUniform->m_pShader;

		if (m_pMethod)
		{
			m_pMethod->SetValue(pUniform,pShader);		
		}
		else
		{
			const std::type_info& type = m_anyValue.getType();
			if (type == typeid(float))
			{
				pShader->SetValue(pUniform, any_cast<float>(&m_anyValue));
			}
			else if (type == typeid(Vector2))
			{
				pShader->SetValue(pUniform, any_cast<Vector2>(&m_anyValue));
			}
			else if (type == typeid(Vector3))
			{
				pShader->SetValue(pUniform, any_cast<Vector3>(&m_anyValue));
			}
			else if (type == typeid(Vector4))
			{
				pShader->SetValue(pUniform, any_cast<Vector4>(&m_anyValue));
			}
			else if (type == typeid(ColourValue))
			{
				ColourValue cColor = any_cast<ColourValue>(m_anyValue);
				Vector4 vColor(cColor.r,cColor.g,cColor.b,cColor.a);
				pShader->SetValue(pUniform,vColor);
			}
			else if (type == typeid(std::vector<Vector4>))
			{
				std::vector<Vector4>& arrVec4 = *any_cast< std::vector<Vector4> >(&m_anyValue);
				pShader->SetValue(pUniform, &arrVec4[0], arrVec4.size());
			}
			else if (type == typeid(Matrix4))
			{
				pShader->SetValue(pUniform, any_cast<Matrix4>(&m_anyValue));
			}
			else if (type == typeid(std::vector<Matrix4>))
			{
				std::vector<Matrix4>& arrMat4 = *any_cast< std::vector<Matrix4> >(&m_anyValue);
				pShader->SetValue(pUniform, &arrMat4[0], arrMat4.size());
			}
			else if (type == typeid(RefPtr<Texture>))
			{
				pShader->SetValue(pUniform,any_cast< RefPtr<Texture> >(&m_anyValue)->get());
			}
			else if (type == typeid(SamplerStatePtr))
			{
				pShader->SetValue(pUniform,any_cast< RefPtr<SamplerState> >(&m_anyValue)->get());
			}
			else
			{
				ASSERT(false);
			}
		}
		
	}

}
