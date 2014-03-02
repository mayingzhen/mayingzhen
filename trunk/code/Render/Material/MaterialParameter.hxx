#include "MaterialParameter.h"


namespace ma
{

	MaterialParameter::MaterialParameter(const char* name) :
		_type(MaterialParameter::NONE), _count(1), _dynamic(false), _name(name ? name : "")/*, _uniform(NULL)*/
	{
		clearValue();
	}

	MaterialParameter::~MaterialParameter()
	{
		clearValue();
	}

	void MaterialParameter::clearValue()
	{
		// Release parameters
		switch (_type)
		{
		case MaterialParameter::SAMPLER:
			//if (_value.samplerValue)
			//    const_cast<SamplerState*>(_value.samplerValue)->release();
			break;
// 		case MaterialParameter::SAMPLER_ARRAY:
// 			if (_value.samplerArrayValue)
// 			{
// 				for (UINT i = 0; i < _count; ++i)
// 				{
// 					//const_cast<SamplerState*>(_value.samplerArrayValue[i])->release();
// 				}
// 			}
// 			break;
		default:
			// Ignore all other cases.
			break;
		}

		// Free dynamic data
		if (_dynamic)
		{
			switch (_type)
			{
			case MaterialParameter::FLOAT:
				SAFE_DELETE_ARRAY(_value.floatPtrValue);
				break;
			case MaterialParameter::INT:
				SAFE_DELETE_ARRAY(_value.intPtrValue);
				break;
			case MaterialParameter::METHOD:
				//SAFE_RELEASE(_value.method);
				break;
// 			case MaterialParameter::SAMPLER_ARRAY:
// 				SAFE_DELETE_ARRAY(_value.samplerArrayValue);
// 				break;
			default:
				// Ignore all other cases.
				break;
			}

			_dynamic = false;
			_count = 1;
		}

		memset(&_value, 0, sizeof(_value));
		_type = MaterialParameter::NONE;
	}

	const char* MaterialParameter::GetName() const
	{
		return _name.c_str();
	}

	void MaterialParameter::setParameter(MaterialParameter* pParam)
	{
		if (pParam == NULL)
			return;

		if (pParam->_type == FLOAT)
		{
			setFloatArray(pParam->_value.floatPtrValue,pParam->_count,true);
		}
		else if (pParam->_type == SAMPLER)
		{
			setSampler(pParam->_value.samplerValue);
		}
		else if (pParam->_type == TEXTURE)
		{
			setTexture(pParam->_value.textureValue);
		}
	}


	void MaterialParameter::setFloat(float value)
	{
		setFloatArray(&value,1,true);
	}

	void MaterialParameter::setFloatArray(const float* values, UINT count, bool copy)
	{
		ASSERT(values);
		clearValue();

		if (copy)
		{
			_value.floatPtrValue = new float[count];
			memcpy(_value.floatPtrValue, values, sizeof(float) * count);
			_dynamic = true;
		}
		else
		{
			_value.floatPtrValue = const_cast<float*> (values);
		}

		_count = count;
		_type = MaterialParameter::FLOAT;
	}

	void MaterialParameter::setInt(int value)
	{
		setIntArray(&value,1,true);
	}

	void MaterialParameter::setIntArray(const int* values, UINT count, bool copy)
	{
		ASSERT(values);
		clearValue();

		if (copy)
		{
			_value.intPtrValue = new int[count];
			memcpy(_value.intPtrValue, values, sizeof(int) * count);
			_dynamic = true;
		}
		else
		{
			_value.intPtrValue = const_cast<int*> (values);
		}

		_count = count;
		_type = MaterialParameter::INT;
	}

	void MaterialParameter::setVector2(const Vector2& value)
	{
		setFloatArray(&value.x,2,true);
	}


	void MaterialParameter::setVector3(const Vector3& value)
	{
		setFloatArray(&value.x,3,true);
	}


	void MaterialParameter::setVector4(const Vector4& value)
	{
		setFloatArray(&value.x,4,true);
	}

	void MaterialParameter::setVector4Array(const Vector4* values, UINT count, bool copy)
	{
		setFloatArray(const_cast<float*> (&values[0].x),4 * count, copy);
	}

	void MaterialParameter::setMatrix(const Matrix4x4& value)
	{
		setFloatArray(const_cast<float*> (&value._11),16,true);
	}

	void MaterialParameter::setMatrixArray(const Matrix4x4* values, UINT count, bool copy)
	{
		setFloatArray(const_cast<float*> (&values[0]._11),16 * count,true);
	}


	void MaterialParameter::setSampler(const SamplerState* value)
	{
		ASSERT(value);
		clearValue();

		//const_cast<SamplerState*>(sampler)->ref();
		_value.samplerValue = value;
		_type = MaterialParameter::SAMPLER;
	}


	void MaterialParameter::setTexture(const Texture* value)
	{
		ASSERT(value);
		clearValue();

		_value.textureValue = value;
		_type = MaterialParameter::TEXTURE;
	}


	std::string GetStrType(MaterialParameter::DataType& type)
	{
		switch (type)
		{
		case MaterialParameter::FLOAT:
			return "FLOAT";
		case MaterialParameter::INT:
			return "INT";
		case MaterialParameter::SAMPLER:
			return "SAMPLER";
		case  MaterialParameter::TEXTURE:
			return "TEXTURE";
		default:
			break ;
		}

		return "NONE";
	}

	MaterialParameter::DataType GetEnumType(std::string& strType)
	{
		if (strType == "FLOAT")
			return MaterialParameter::FLOAT;
		else if (strType == "INT")
			return MaterialParameter::INT;
		else if (strType == "SAMPLER")
			return MaterialParameter::SAMPLER;
		else if (strType == "TEXTURE")
			return MaterialParameter::TEXTURE;
		else
			return MaterialParameter::NONE;
	}

	void MaterialParameter::Serialize(Serializer& sl, const char* pszLable/* = "Parameter"*/)
	{
		sl.BeginSection(pszLable);

		sl.Serialize(_name,"name");

		std::string strType;
		if ( sl.IsReading() )
		{
			sl.Serialize(strType,"type");

			_type = GetEnumType(strType);
		}
		else
		{
			strType = GetStrType(_type);

			sl.Serialize(strType,"type");
		}

		sl.Serialize(_count,"count");

		switch (_type)
		{
		case MaterialParameter::FLOAT:
			sl.Serialize(_value.floatPtrValue,_count,"value");
			break;
		case MaterialParameter::INT:
			//sl.Serialize(_value.intPtrValue,_count,"value");
			break;
		case  MaterialParameter::SAMPLER:
			{
				if ( sl.IsReading() )
				{
					SamplerState* pSampler = new SamplerState();//(SamplerState*)_value.samplerValue;
					pSampler->Serialize(sl);
					setSampler(pSampler);
				}
				else
				{
					SamplerState* pSampler = (SamplerState*)_value.samplerValue;
					pSampler->Serialize(sl);
				}
			}
		case  MaterialParameter::TEXTURE:
			{

			}
		default:
			break;
		}

		sl.EndSection();
	}

	void MaterialParameter::Bind(Uniform* pUniform)
	{
		ASSERT(pUniform && pUniform->m_pShader);
		if (pUniform == NULL || pUniform->m_pShader == NULL)
			return;
	
		ShaderProgram* effect = pUniform->m_pShader;
		
		switch (_type)
		{
		case MaterialParameter::FLOAT:
			effect->SetValue(pUniform, _value.floatPtrValue, _count);
			break;
		case MaterialParameter::INT:
			effect->SetValue(pUniform, _value.intPtrValue, _count);
			break;
		case MaterialParameter::SAMPLER:
			effect->SetValue(pUniform, _value.samplerValue);
			break;
		case MaterialParameter::TEXTURE:
			effect->SetValue(pUniform,_value.textureValue);
			break;
		case MaterialParameter::METHOD:
			ASSERT(_value.method);
			_value.method->SetValue(pUniform,effect);
			break;
		default:
			GP_ERROR("Unsupported material parameter type (%d).", _type);
			break;
		}
	}

}
