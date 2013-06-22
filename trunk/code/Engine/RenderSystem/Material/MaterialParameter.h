#ifndef MATERIALPARAMETER_H_
#define MATERIALPARAMETER_H_


namespace ma
{
	class ShaderProgram;
	struct Uniform;

	/**
	 * Defines a material parameter.
	 *
	 * This class represents a parameter that can be set for a material.
	 * The methods in this class provide a mechanism to set parameters
	 * of all supported types. Some types support setting by value,
	 * while others only support setting by reference/pointer.
	 *
	 * Setting a parameter by reference/pointer provides the ability to
	 * pass an array of values as well as a convenient way to support
	 * auto-binding of values to a material parameter. For example, by
	 * setting the parameter value to a pointer to a Matrix4x4, any changes
	 * to the Matrix4x4 will automatically be reflected in the technique the
	 * next time the parameter is applied to the render state.
	 *
	 * Note that for parameter values to arrays or pointers, the 
	 * MaterialParameter will keep a long-lived reference to the passed
	 * in array/pointer. Therefore, you must ensure that the pointers
	 * you pass in are valid for the lifetime of the MaterialParameter
	 * object.
	 */
	class ENGINE_API MaterialParameter 
	{
		friend class Material;

	public:

		/**
		 * Returns the name of this material parameter.
		 */
		const char* getName() const;

		/**
		 * Returns the texture sampler or NULL if this MaterialParameter is not a sampler type.
		 * 
		 * @param index Index of the sampler (if the parameter is a sampler array),
		 *      or zero if it is a single sampler value.
		 *
		 * @return The texture sampler or NULL if this MaterialParameter is not a sampler type.
		 */
		Sampler* getSampler(unsigned int index = 0) const;


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

		Sampler* setSampler(const char* texturePath, bool generateMipmaps);

		void setSampler(const Sampler* value);

		void setSamplerArray(const Sampler** values, unsigned int count, bool copy = false);

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
		 * Constructor.
		 */
		MaterialParameter(const char* name);
	    
		/**
		 * Destructor.
		 */
		~MaterialParameter();

		/**
		 * Hidden copy assignment operator.
		 */
		MaterialParameter& operator=(const MaterialParameter&);
	    
		/**
		 * Interface implemented by templated method bindings for simple storage and iteration.
		 */
		class MethodBinding 
		{
			friend class RenderState;

		public:

			virtual void SetValue(ShaderProgram* effect) = 0;

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
			void SetValue(ShaderProgram* effect);
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
			void SetValue(ShaderProgram* effect);
		private:
			ClassType* _instance;
			ValueMethod _valueMethod;
			CountMethod _countMethod;
		};

		void clearValue();

		void bind(ShaderProgram* effect);

	    
		union
		{
			/** @script{ignore} */
			float floatValue;
			/** @script{ignore} */
			int intValue;
			/** @script{ignore} */
			float* floatPtrValue;
			/** @script{ignore} */
			int* intPtrValue;
			/** @script{ignore} */
			const Sampler* samplerValue;
			/** @script{ignore} */
			const Sampler** samplerArrayValue;
			/** @script{ignore} */
			MethodBinding* method;
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
			METHOD
		} _type;
	    
		unsigned int _count;
		bool _dynamic;
		std::string _name;
		Uniform* _uniform;
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
	void MaterialParameter::MethodValueBinding<ClassType, ParameterType>::SetValue(ShaderProgram* effect)
	{
		effect->SetValue(_parameter->_uniform, (_instance->*_valueMethod)());
	}

	template <class ClassType, class ParameterType>
	MaterialParameter::MethodArrayBinding<ClassType, ParameterType>::MethodArrayBinding(MaterialParameter* param, ClassType* instance, ValueMethod valueMethod, CountMethod countMethod) :
		MethodBinding(param), _instance(instance), _valueMethod(valueMethod), _countMethod(countMethod)
	{
	}

	template <class ClassType, class ParameterType>
	void MaterialParameter::MethodArrayBinding<ClassType, ParameterType>::SetValue(ShaderProgram* effect)
	{
		effect->SetValue(_parameter->_uniform, (_instance->*_valueMethod)(), (_instance->*_countMethod)());
	}

}

#endif
