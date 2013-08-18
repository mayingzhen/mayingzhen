#include "MaterialParameter.h"


namespace ma
{

	MaterialParameter::MaterialParameter(const char* name) :
		_type(MaterialParameter::NONE), _count(1), _dynamic(false), _name(name ? name : ""), _uniform(NULL)
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
			//    const_cast<Sampler*>(_value.samplerValue)->release();
			break;
		case MaterialParameter::SAMPLER_ARRAY:
			if (_value.samplerArrayValue)
			{
				for (unsigned int i = 0; i < _count; ++i)
				{
					//const_cast<Sampler*>(_value.samplerArrayValue[i])->release();
				}
			}
			break;
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
			case MaterialParameter::FLOAT_ARRAY:
			case MaterialParameter::VECTOR2:
			case MaterialParameter::VECTOR3:
			case MaterialParameter::VECTOR4:
			case MaterialParameter::MATRIX:
				SAFE_DELETE_ARRAY(_value.floatPtrValue);
				break;
			case MaterialParameter::INT:
			case MaterialParameter::INT_ARRAY:
				SAFE_DELETE_ARRAY(_value.intPtrValue);
				break;
			case MaterialParameter::METHOD:
				//SAFE_RELEASE(_value.method);
				break;
			case MaterialParameter::SAMPLER_ARRAY:
				SAFE_DELETE_ARRAY(_value.samplerArrayValue);
				break;
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

	const char* MaterialParameter::getName() const
	{
		return _name.c_str();
	}

	Sampler* MaterialParameter::getSampler(unsigned int index) const
	{
		if (_type == MaterialParameter::SAMPLER)
			return const_cast<Sampler*>(_value.samplerValue);
		if (_type == MaterialParameter::SAMPLER_ARRAY && index < _count)
			return const_cast<Sampler*>(_value.samplerArrayValue[index]);
		return NULL;
	}


	void MaterialParameter::setFloat(float value)
	{
		clearValue();

		_value.floatValue = value;
		_type = MaterialParameter::FLOAT;
	}

	void MaterialParameter::setFloatArray(const float* values, unsigned int count, bool copy)
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
		_type = MaterialParameter::FLOAT_ARRAY;
	}

	void MaterialParameter::setInt(int value)
	{
		clearValue();

		_value.intValue = value;
		_type = MaterialParameter::INT;
	}

	void MaterialParameter::setIntArray(const int* values, unsigned int count, bool copy)
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
		_type = MaterialParameter::INT_ARRAY;
	}

	void MaterialParameter::setVector2(const Vector2& value)
	{
		clearValue();

		// Copy data by-value into a dynamic array.
		float* array = new float[2];
		memcpy(array, &value.x, sizeof(float) * 2);

		_value.floatPtrValue = array;
		_dynamic = true;
		_count = 1;
		_type = MaterialParameter::VECTOR2;
	}

	void MaterialParameter::setVector2Array(const Vector2* values, unsigned int count, bool copy)
	{
		ASSERT(values);
		clearValue();

		if (copy)
		{
			_value.floatPtrValue = new float[2 * count];
			memcpy(_value.floatPtrValue, const_cast<float*> (&values[0].x), sizeof(float) * 2 * count);
			_dynamic = true;
		}
		else
		{
			_value.floatPtrValue = const_cast<float*> (&values[0].x);
		}

		_count = count;
		_type = MaterialParameter::VECTOR2;
	}

	void MaterialParameter::setVector3(const Vector3& value)
	{
		clearValue();

		// Copy data by-value into a dynamic array.
		float* array = new float[3];
		memcpy(array, &value.x, sizeof(float) * 3);

		_value.floatPtrValue = array;
		_dynamic = true;
		_count = 1;
		_type = MaterialParameter::VECTOR3;
	}

	void MaterialParameter::setVector3Array(const Vector3* values, unsigned int count, bool copy)
	{
		ASSERT(values);
		clearValue();

		if (copy)
		{
			_value.floatPtrValue = new float[3 * count];
			memcpy(_value.floatPtrValue, const_cast<float*> (&values[0].x), sizeof(float) * 3 * count);
			_dynamic = true;
		}
		else
		{
			_value.floatPtrValue = const_cast<float*> (&values[0].x);
		}

		_count = count;
		_type = MaterialParameter::VECTOR3;
	}

	void MaterialParameter::setVector4(const Vector4& value)
	{
		clearValue();

		// Copy data by-value into a dynamic array.
		float* array = new float[4];
		memcpy(array, &value.x, sizeof(float) * 4);

		_value.floatPtrValue = array;
		_dynamic = true;
		_count = 1;
		_type = MaterialParameter::VECTOR4;
	}

	void MaterialParameter::setVector4Array(const Vector4* values, unsigned int count, bool copy)
	{
		ASSERT(values);
		clearValue();

		if (copy)
		{
			_value.floatPtrValue = new float[4 * count];
			memcpy(_value.floatPtrValue, const_cast<float*> (&values[0].x), sizeof(float) * 4 * count);
			_dynamic = true;
		}
		else
		{
			_value.floatPtrValue = const_cast<float*> (&values[0].x);
		}

		_count = count;
		_type = MaterialParameter::VECTOR4;
	}

	void MaterialParameter::setMatrix(const Matrix4x4& value)
	{
		//SetValue(value);
		// If this parameter is already storing a single dynamic matrix, no need to clear it.
		if (!(_dynamic && _count == 1 && _type == MaterialParameter::MATRIX && _value.floatPtrValue != NULL))
		{
			clearValue();

			// Allocate a new dynamic matrix.
			_value.floatPtrValue = new float[16];
		}

		memcpy(_value.floatPtrValue, value.m, sizeof(float) * 16);

		_dynamic = true;
		_count = 1;
		_type = MaterialParameter::MATRIX;
	}

	void MaterialParameter::setMatrixArray(const Matrix4x4* values, unsigned int count, bool copy)
	{
		ASSERT(values);
		clearValue();

		if (copy)
		{
			_value.floatPtrValue = new float[16 * count];
			memcpy(_value.floatPtrValue, const_cast<Matrix4x4&> (values[0]).m, sizeof(float) * 16 * count);
			_dynamic = true;
		}
		else
		{
			//_value.floatPtrValue = const_cast<Matrix4x4&> (values[0]).m;
		}

		_count = count;
		_type = MaterialParameter::MATRIX;
	}

// 	Sampler* MaterialParameter::setSampler(const char* texturePath, bool generateMipmaps)
// 	{
// 		ASSERT(texturePath);
// 		clearValue();
// 
// 		Sampler* sampler = Sampler::create(texturePath, generateMipmaps);
// 		if (sampler)
// 		{
// 			_value.samplerValue = sampler;
// 			_type = MaterialParameter::SAMPLER;
// 		}
// 		return sampler;
// 	}

	void MaterialParameter::setSampler(const Sampler* value)
	{
		ASSERT(value);
		clearValue();

		//const_cast<Sampler*>(sampler)->IncReference();
		_value.samplerValue = value;
		_type = MaterialParameter::SAMPLER;
	}

	void MaterialParameter::setSamplerArray(const Sampler** values, unsigned int count, bool copy)
	{
		ASSERT(values);
		clearValue();

		if (copy)
		{
			_value.samplerArrayValue = new const Sampler*[count];
			memcpy(_value.samplerArrayValue, values, sizeof(Sampler*) * count);
			_dynamic = true;
		}
		else
		{
			_value.samplerArrayValue = values;
		}

		for (unsigned int i = 0; i < count; ++i)
		{
			//const_cast<Sampler*>(_value.samplerArrayValue[i])->IncReference();
		}

		_count = count;
		_type = MaterialParameter::SAMPLER_ARRAY;
	}

	void MaterialParameter::setTexture(const Texture* value)
	{
		ASSERT(value);
		clearValue();

		//const_cast<Sampler*>(sampler)->IncReference();
		_value.textureValue = value;
		_type = MaterialParameter::TEXTURE;
	}

	void MaterialParameter::setTextureArray(const Texture** values, unsigned int count, bool copy)
	{
		ASSERT(values);
		clearValue();

		if (copy)
		{
			_value.textureArrayValue = new const Texture*[count];
			memcpy(_value.textureArrayValue, values, sizeof(Texture*) * count);
			_dynamic = true;
		}
		else
		{
			_value.textureArrayValue = values;
		}

		_count = count;
		_type = MaterialParameter::TEXTURE_ARRAY;
	}

	void MaterialParameter::bind(ShaderProgram* effect)
	{
		ASSERT(effect);
		if (effect == NULL)
			return;

		// If we had a Uniform cached that is not from the passed in effect,
		// we need to update our uniform to point to the new effect's uniform.
		if (!_uniform || _uniform->GetEffect() != effect)
		{
			_uniform = effect->GetUniform(_name.c_str());

			if (!_uniform)
			{
				//ASSERT(false);
				// This parameter was not found in the specified effect, so do nothing.
				//GP_WARN("Warning: Material parameter '%s' not found in effect '%s'.", _name.c_str(), effect->GetId());
				return;
			}
		}

		switch (_type)
		{
		case MaterialParameter::FLOAT:
			effect->SetValue(_uniform, _value.floatValue);
			break;
		case MaterialParameter::FLOAT_ARRAY:
			effect->SetValue(_uniform, _value.floatPtrValue, _count);
			break;
		case MaterialParameter::INT:
			effect->SetValue(_uniform, _value.intValue);
			break;
		case MaterialParameter::INT_ARRAY:
			effect->SetValue(_uniform, _value.intPtrValue, _count);
			break;
		case MaterialParameter::VECTOR2:
			effect->SetValue(_uniform, reinterpret_cast<Vector2*>(_value.floatPtrValue), _count);
			break;
		case MaterialParameter::VECTOR3:
			effect->SetValue(_uniform, reinterpret_cast<Vector3*>(_value.floatPtrValue), _count);
			break;
		case MaterialParameter::VECTOR4:
			effect->SetValue(_uniform, reinterpret_cast<Vector4*>(_value.floatPtrValue), _count);
			break;
		case MaterialParameter::MATRIX:
			effect->SetValue(_uniform, reinterpret_cast<Matrix4x4*>(_value.floatPtrValue), _count);
			break;
		case MaterialParameter::SAMPLER:
			effect->SetValue(_uniform, _value.samplerValue);
			break;
		case MaterialParameter::SAMPLER_ARRAY:
			effect->SetValue(_uniform, _value.samplerArrayValue, _count);
			break;
		case MaterialParameter::TEXTURE:
			effect->SetValue(_uniform,_value.textureValue);
			break;
		case MaterialParameter::TEXTURE_ARRAY:
			effect->SetValue(_uniform,_value.textureArrayValue,_count);
			break;
		case MaterialParameter::METHOD:
			ASSERT(_value.method);
			_value.method->SetValue(effect);
			break;
		default:
			GP_ERROR("Unsupported material parameter type (%d).", _type);
			break;
		}
	}


	MaterialParameter::MethodBinding::MethodBinding(MaterialParameter* param) :
		_parameter(param), _autoBinding(false)
	{
	}

}
