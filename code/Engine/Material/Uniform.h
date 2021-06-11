#pragma once

namespace ma
{
	class MethodBinding;

	class ConstantBuffer : public Referenced
	{
	public:

		virtual void	SetParameter(unsigned offset, unsigned size, const void* data) = 0;

		uint32_t		GetSize() { return m_nSize; }
		void		    SetSize(uint32_t nSize) { m_nSize = nSize; }

		uint32_t		GetBound() { return m_nBound; }
		void		    SetBound(uint32_t nBound) { m_nBound = nBound; }

		Uniform*	    AddUniform(const char* pName);
		uint32_t		GetUniformCount();
		Uniform*	    GetUniformByIndex(uint32_t nIndex);

		void		    SetName(const char* pszName);
		const char*     GetName();

	private:
		std::string m_strName;

		uint32_t m_nBound = 0;

		uint32_t m_nSize = 0;

		std::vector< RefPtr<Uniform> > m_vecUniform;
	};
	
	class Uniform : public Referenced
	{
	public:
		Uniform();

		~Uniform();
		
		void		        Bind(Renderable* pRenderable, SceneContext* sc);

		const char*	        GetName() const;
		void		        SetName(const char* pszName);

		uint32_t	        GetIndex() {return m_index;}
		void		        SetIndex(uint32_t nIndex) { m_index = nIndex; }

		void		        SetParent(ConstantBuffer* pCB) { m_pCBPtr = pCB; }
		ConstantBuffer*     GetParent() { return m_pCBPtr; }

		uint32_t	        GetOffset() { return m_nOffSetInCB; }
		void		        SetOffset(uint32_t nOffset) { m_nOffSetInCB = nOffset;}

		uint32_t	        GetSize() { return m_nSizeInCB; }
		void		        SetSize(uint32_t nSize) { m_nSizeInCB = nSize; }

		void		        SetMethodBinding(MethodBinding* pMethod) { m_pMethod = pMethod; }
		MethodBinding*      GetMethodBinding() { return m_pMethod.get(); }

		ShaderType			GetShaderType() { return m_eType; }
		void				SetShaderType(ShaderType eType) { m_eType = eType; }

	private:
		std::string		    m_name;
		uint32_t			m_index = 0;

		ConstantBuffer*     m_pCBPtr = NULL;
		uint32_t			m_nOffSetInCB = 0;
		uint32_t			m_nSizeInCB = 0;

		ShaderType			m_eType;

		RefPtr<MethodBinding>  m_pMethod;
	};

	RefPtr<Uniform> CreateUniform(const char* pszName);
   
}


