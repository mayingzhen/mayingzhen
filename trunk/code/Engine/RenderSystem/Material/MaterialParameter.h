#ifndef _MaterialParameter_H_
#define _MaterialParameter_H_

#include "ShaderProgram.h"


namespace ma
{

	class ENGINE_API MaterialParameter 
	{
		friend class Material;
		friend class Technique;

	public:

		MaterialParameter(const char* name = NULL);

		~MaterialParameter();

		const char* getName() const;

		void setFloat(float value);

		void setFloatArray(const float* values, unsigned int count, bool copy = false);

		void setInt(int value);

		void setIntArray(const int* values, unsigned int count, bool copy = false);

		void setVector2(const Vector2& value);

		void setVector2Array(const Vector2* values, unsigned int count, bool copy = false);

		void setVector3(const Vector3& value);

		void setVector3Array(const Vector3* values, unsigned int count, bool copy = false);

		void setVector4(const Vector4& value);

		void setVector4Array(const Vector4* values, unsigned int count, bool copy = false);

		void setMatrix(const Matrix4x4& value);

		void setMatrixArray(const Matrix4x4* values, unsigned int count, bool copy = false);

		void setSampler(const SamplerState* value);

		void setSamplerArray(const SamplerState** values, unsigned int count, bool copy = false);

		void setTexture(const Texture* value);

		void setTextureArray(const Texture** values, unsigned int count, bool copy = false);

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
		void bindValue(ClassType* classInstance, ParameterType (ClassType::*valueMethod)() const, unsigned int (ClassType::*countMethod)() const);

	private:
	   

		/**
		 * Hidden copy assignment operator.
		 */
		MaterialParameter& operator=(const MaterialParameter&);
	    
		/**
		 * Interface implemented by templated method bindings for simple storage and iteration.
		 */
		class MethodBinding 
		{
			friend struct RenderState;

		public:

			virtual void SetValue(Uniform* pUniform,ShaderProgram* effect) = 0;

		protected:

			/**
			 * Constructor.
			 */
			MethodBinding(MaterialParameter* param);

			/**
			 * Destructor.
			 */
			virtual ~MethodBinding() { }

			/**
			 * Hidden copy assignment operator.
			 */
			MethodBinding& operator=(const MethodBinding&);

			MaterialParameter* _parameter;
			bool _autoBinding;
		};

		/**
		 * Defines a method parameter binding for a single value.
		 */
		template <class ClassType, class ParameterType>
		class MethodValueBinding : public MethodBinding
		{
			typedef ParameterType (ClassType::*ValueMethod)() const;
		public:
			MethodValueBinding(MaterialParameter* param, ClassType* instance, ValueMethod valueMethod);
			void SetValue(Uniform* pUniform,ShaderProgram* effect);
		private:
			ClassType* _instance;
			ValueMethod _valueMethod;

		};

		/**
		 * Defines a method parameter binding for an array of values.
		 */
		template <class ClassType, class ParameterType>
		class MethodArrayBinding : public MethodBinding
		{
			typedef ParameterType (ClassType::*ValueMethod)() const;
			typedef unsigned int (ClassType::*CountMethod)() const;
		public:
			MethodArrayBinding(MaterialParameter* param, ClassType* instance, ValueMethod valueMethod, CountMethod countMethod);
			void SetValue(Uniform* pUniform,ShaderProgram* effect);
		private:
			ClassType* _instance;
			ValueMethod _valueMethod;
			CountMethod _countMethod;
		};

		void clearValue();

		//void bind(ShaderProgram* effect);

		void Bind(Uniform* pUniform);

	    
		union
		{
			float           floatValue;
			int             intValue;
			float*          floatPtrValue;
			int*            intPtrValue;
			const SamplerState*  samplerValue;
			const SamplerState** samplerArrayValue;
			const Texture*	textureValue;
			const Texture**	textureArrayValue;
			MethodBinding*  method;
		} _value;
	    
		enum
		{
			NONE,
			FLOAT,
			FLOAT_ARRAY,
			INT,
			INT_ARRAY,
			VECTOR2,
			VECTOR3,
			VECTOR4,
			MATRIX,
			SAMPLER,
			SAMPLER_ARRAY,
			TEXTURE,
			TEXTURE_ARRAY,
			METHOD
		} _type;
	    
		unsigned int _count;
		bool _dynamic;
		std::string _name;
		//Uniform* _uniform;
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
	void MaterialParameter::bindValue(ClassType* classInstance, ParameterType (ClassType::*valueMethod)() const, unsigned int (ClassType::*countMethod)() const)
	{
		clearValue();

		_value.method = new MethodArrayBinding<ClassType, ParameterType>(this, classInstance, valueMethod, countMethod);
		_dynamic = true;
		_type = MaterialParameter::METHOD;
	}

	template <class ClassType, class ParameterType>
	MaterialParameter::MethodValueBinding<ClassType, ParameterType>::MethodValueBinding(MaterialParameter* param, ClassType* instance, ValueMethod valueMethod) :
		MethodBinding(param), _instance(instance), _valueMethod(valueMethod)
	{
	}

	template <class ClassType, class ParameterType>
	void MaterialParameter::MethodValueBinding<ClassType, ParameterType>::SetValue(Uniform* pUniform,ShaderProgram* effect)
	{
		effect->SetValue(pUniform, (_instance->*_valueMethod)());
	}

	template <class ClassType, class ParameterType>
	MaterialParameter::MethodArrayBinding<ClassType, ParameterType>::MethodArrayBinding(MaterialParameter* param, ClassType* instance, ValueMethod valueMethod, CountMethod countMethod) :
		MethodBinding(param), _instance(instance), _valueMethod(valueMethod), _countMethod(countMethod)
	{
	}

	template <class ClassType, class ParameterType>
	void MaterialParameter::MethodArrayBinding<ClassType, ParameterType>::SetValue(Uniform* pUniform,ShaderProgram* effect)
	{
		effect->SetValue(pUniform, (_instance->*_valueMethod)(), (_instance->*_countMethod)());
	}

}

#endif
