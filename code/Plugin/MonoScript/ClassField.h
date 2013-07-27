#ifndef __MonoClassField_h__
#define __MonoClassField_h__

namespace ma
{
	class ClassField 
	{
		union Value
		{
			float           m_fFloat;
			int             m_iInt;
			UINT			m_uInt;
			//Vector3			m_vVector3;
			float			m_vVector3[3];
		};

		enum Type
		{
			NONE_TYPE,
			FLOAT_TYPE,
			INT_TYPE,
			VECTOR3_TYPE,
		};


	public:
		ClassField();

		float		GetFloat();

		void		SetFloat(float fValue);

		int			GetInt();

		void		SetInt(int nValue);

		Vector3		GetVector3();

		void		SetVector3(const Vector3& vecotr3);

		void		Serialize(Serializer& sl, const char* pszLable = "ClassField");

		void		ParseMonoFiled(MonoClassField* pMonoField,MonoObject* pMonoObject);

		void		SetMonoObjecField(MonoClassField* pMonoField,MonoObject* pMonoObject);

		const char* GetFieldName() {return m_strName.c_str();}

		ClassField* Clone();

	private:
		std::string ToStringType(Type eType);
		Type		ToEType(std::string stType);
		Type		ToEType(MonoType* pMonoType);
			
	private:

		std::string			m_strName;
		Type				m_eType;
		Value				m_value;		
		MonoClassField*		m_pMonoField;
		MonoObject*			m_pMonoObject;
	};
}

#endif

