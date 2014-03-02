#ifndef _MaterialParameter_H_
#define _MaterialParameter_H_

#include "ShaderProgram.h"
#include "MethodBinding.h"

namespace ma
{

	class RENDER_API MaterialParameter 
	{
		friend class Material;
		friend class Technique;

	public:

		MaterialParameter(const char* name = NULL);

		~MaterialParameter();

		const char* GetName() const;

		void setParameter(MaterialParameter* pParam);

		void setFloat(float value);

		void setFloatArray(const float* values, UINT count, bool copy = false);

		void setInt(int value);

		void setIntArray(const int* values, UINT count, bool copy = false);

		void setVector2(const Vector2& value);

		void setVector2Array(const Vector2* values, UINT count, bool copy = false);

		void setVector3(const Vector3& value);

		void setVector3Array(const Vector3* values, UINT count, bool copy = false);

		void setVector4(const Vector4& value);

		void setVector4Array(const Vector4* values, UINT count, bool copy = false);

		void setMatrix(const Matrix4x4& value);

		void setMatrixArray(const Matrix4x4* values, UINT count, bool copy = false);

		void setSampler(const SamplerState* value);

		//void setSamplerArray(const SamplerState** values, UINT count, bool copy = false);

		void setTexture(const Texture* value);

		//void setTextureArray(const Texture** values, UINT count, bool copy = false);

		void Serialize(Serializer& sl, const char* pszLable = "Parameter");

		/**
		 * Binds the return value of a class method to this material parameter.
		 *
		 * This method enables binding of arbitrary class methods to a material
		 * parameter. This is useful when you want to set a material parameter
		 * to a variable that is frequently changing (such as a world matrix).
		**/
		template <class ClassType, class ParameterType>
		void bindValue(ClassType* classInstance, ParameterType (ClassType::*valueMethod)() const);

		/**
		 * Binds the return value of a class method to this material parameter.
		 *
		 * This overloads the setBinding method to provide support for array parameters.
		 * The valueMethod parameter should return an array (pointer) of a supported
		 * material parameter type, such as Matrix4x4* for an array of matrices. The
		 * countMethod should point to a method that returns the number of entries in
		 * the value returned from valueMethod.
		 */
		template <class ClassType, class ParameterType>
		void bindValue(ClassType* classInstance, ParameterType (ClassType::*valueMethod)() const, UINT (ClassType::*countMethod)() const);

	//private:
	   

		/**
		 * Hidden copy assignment operator.
		 */
		MaterialParameter& operator=(const MaterialParameter&);
	    
		void clearValue();

		void Bind(Uniform* pUniform);

	    
		union DataValue
		{
			//float           floatValue;
			//int             intValue;
			float*          floatPtrValue;
			int*            intPtrValue;
			const SamplerState*  samplerValue;
			//const SamplerState** samplerArrayValue;
			const Texture*	textureValue;
			//const Texture**	textureArrayValue;
			MethodBinding*  method;
		}; 
	    
		enum DataType
		{
			NONE,
			FLOAT,
			INT,
			SAMPLER,
			//SAMPLER_ARRAY,
			TEXTURE,
			//TEXTURE_ARRAY,
			METHOD
		};

		DataType	_type;

		DataValue	_value;

		UINT		_count;

		bool		_dynamic;

		std::string _name;
	};

	template <class ClassType, class ParameterType>
	void MaterialParameter::bindValue(ClassType* classInstance, ParameterType (ClassType::*valueMethod)() const)
	{
		clearValue();

		_value.method = new MethodValueBinding<ClassType, ParameterType>(this, classInstance, valueMethod);
		_dynamic = true;
		_type = MaterialParameter::METHOD;
	}

	template <class ClassType, class ParameterType>
	void MaterialParameter::bindValue(ClassType* classInstance, ParameterType (ClassType::*valueMethod)() const, UINT (ClassType::*countMethod)() const)
	{
		clearValue();

		_value.method = new MethodArrayBinding<ClassType, ParameterType>(this, classInstance, valueMethod, countMethod);
		_dynamic = true;
		_type = MaterialParameter::METHOD;
	}

}

#endif
