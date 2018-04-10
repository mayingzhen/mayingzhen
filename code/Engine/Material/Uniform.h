#pragma once

namespace ma
{
	class MethodBinding;

	class ConstantBuffer : public Referenced
	{
	public:

		virtual void SetParameter(unsigned offset, unsigned size, const void* data){}

		UINT		GetSize() { return m_nSize; }
		void		SetSize(uint32 nSize) { m_nSize = nSize; }

		UINT		GetBound() { return m_nBound; }
		void		SetBound(UINT nBound) { m_nBound = nBound; }

		Uniform*	AddUniform(const char* pName);
		UINT		GetUniformCount();
		Uniform*	GetUniformByIndex(UINT nIndex);

		Technique*	GetParent() { return m_pTech; }
		void		SetParent(Technique* pTech) { m_pTech = pTech; }

		void		SetName(const char* pszName);
		const char* GetName();

		RefPtr<ConstantBuffer> Clone();
		
	private:
		std::string m_strName;

		UINT m_nBound = 0;

		UINT m_nSize = 0;

		Technique* m_pTech = NULL;

		std::vector< RefPtr<Uniform> > m_vecUniform;
	};
	
	class Uniform : public Referenced
	{
	public:
		Uniform();

		~Uniform();
		
		void		Bind(Renderable* pRenderable);

		const char*	GetName() const;
		void		SetName(const char* pszName);

		uint32		GetIndex() {return m_index;}
		void		SetIndex(UINT nIndex) { m_index = nIndex; }

		void		SetParent(ConstantBuffer* pCB) { m_pCBPtr = pCB; }
		ConstantBuffer* GetParent() { return m_pCBPtr; }

		void		SetTechnique(Technique* pTech) { m_pTech = pTech; }
		Technique*	GetTechnique() { return m_pTech; }

		UINT		GetOffset() { return m_nOffSetInCB; }
		void		SetOffset(UINT nOffset) { m_nOffSetInCB = nOffset;}

		UINT		GetSize() { return m_nSizeInCB; }
		void		SetSize(UINT nSize) { m_nSizeInCB = nSize; }

		void		SetMethodBinding(MethodBinding* pMethod) { m_pMethod = pMethod; }
		MethodBinding* GetMethodBinding() { return m_pMethod; }

	private:
		std::string		m_name;
		UINT			m_index = 0;

		ConstantBuffer* m_pCBPtr = NULL;
		UINT			m_nOffSetInCB = 0;
		UINT			m_nSizeInCB = 0;
		Technique*		m_pTech = NULL;

		MethodBinding*  m_pMethod = NULL;
	};

	RefPtr<Uniform> CreateUniform(const char* pszName);
   
}


