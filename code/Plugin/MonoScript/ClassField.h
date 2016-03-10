#ifndef __MonoClassField_h__
#define __MonoClassField_h__

namespace ma
{
	class ClassField 
	{
	public:
		ClassField();

		float		GetFloat();

		void		SetFloat(float fValue);

		int			GetInt();

		void		SetInt(int nValue);

		Vector3		GetVector3();

		void		SetVector3(const Vector3& vecotr3);

		void		ParseMonoFiled(MonoClassField* pMonoField,MonoObject* pMonoObject);

		void		SetMonoObjecField(MonoClassField* pMonoField,MonoObject* pMonoObject);

		const char* GetFieldName() {return m_strName.c_str();}

		ClassField* Clone();
			
	private:

		std::string			m_strName;	
		Any					m_anyValue;
		MonoClassField*		m_pMonoField;
		MonoObject*			m_pMonoObject;
	};
}

#endif

