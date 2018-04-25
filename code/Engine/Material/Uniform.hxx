#include "Uniform.h"


namespace ma
{

	Uniform* ConstantBuffer::AddUniform(const char* pName)
	{
		RefPtr<Uniform> pUnifrom = CreateUniform(pName);
		pUnifrom->SetParent(this);
		m_vecUniform.push_back(pUnifrom);
		return pUnifrom.get();
	}

	uint32_t ConstantBuffer::GetUniformCount()
	{
		return m_vecUniform.size();
	}

	Uniform* ConstantBuffer::GetUniformByIndex(uint32_t nIndex)
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

	RefPtr<ConstantBuffer> CreateConstantBuffer(uint32_t nSize)
	{
		return GetRenderDevice()->CreateConstantBuffer(nSize);
	}

	Uniform::Uniform()
	{
		m_index = 0;

		m_pMethod = NULL;

		m_pCBPtr = NULL;
		m_nOffSetInCB = NULL;
		m_nSizeInCB = 0;
	}

	Uniform::~Uniform()
	{
	}

	void Uniform::Bind(Renderable* pRenderable)
	{
		Technique* pTech = m_pTech ? m_pTech : m_pCBPtr->GetParent();
		ASSERT(pTech);
		if (pTech == NULL)
			return;

		if (m_pMethod)
		{
			m_pMethod->SetValue(pRenderable, pTech, this);
		}
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
		pUniform->SetMethodBinding(g_pParameterManager->GetMethodBinding(pszName));
		return pUniform;
	}

}

