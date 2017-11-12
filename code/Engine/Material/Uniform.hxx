#include "Uniform.h"


namespace ma
{

	Uniform* ConstantBuffer::AddUniform(const char* pName)
	{
		Uniform* pUnifrom = new Uniform();
		pUnifrom->SetName(pName);
		pUnifrom->SetParent(this);
		m_vecUniform.push_back(pUnifrom);
		return pUnifrom;
	}

	UINT ConstantBuffer::GetUniformCount()
	{
		return m_vecUniform.size();
	}

	Uniform* ConstantBuffer::GetUniformByIndex(UINT nIndex)
	{
		return m_vecUniform[nIndex].get();
	}

	void ConstantBuffer::SetName(const char* pszName)
	{
		m_strName = pszName ? pszName : "";
	}

	const char* ConstantBuffer::GetName()
	{
		return m_strName.c_str();
	}

	RefPtr<ConstantBuffer> CreateConstantBuffer(UINT nSize)
	{
		return GetRenderDevice()->CreateConstantBuffer(nSize);
	}

	Uniform::Uniform()
	{
		//m_location = -1;
		m_index = 0;
		//m_type = 0;
		//m_vshShder = false;
		//m_nCount = 0;

		m_pMethod = NULL;

		m_pCBPtr = NULL;
		m_nOffSetInCB = NULL;
		m_nSizeInCB = 0;
	}

	Uniform::~Uniform()
	{
		SAFE_DELETE(m_pMethod);
	}

	void Uniform::Bind(Renderable* pRenderable)
	{
		if (m_pMethod) 
			m_pMethod->SetValue(pRenderable);
	}

	const char* Uniform::GetName() const
	{
		return m_name.c_str();
	}

	void Uniform::SetName(const char* pszName)
	{
		m_name = pszName ? pszName : "";
	}

	RefPtr<Uniform> CreateUniform(const char* pszName)
	{
		Uniform* pUniform = new Uniform;
		pUniform->SetName(pszName);
		return pUniform;
	}

}

