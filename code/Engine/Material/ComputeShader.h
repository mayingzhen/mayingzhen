#pragma once

#include "MethodBinding.h"

namespace ma
{

	class ConstantBuffer;
	class Uniform;

	class ComputeShader : public Resource
	{
		
	public:
		ComputeShader();

		virtual ~ComputeShader();
	
		DECL_OBJECT(ComputeShader)

		void				CreateFromFile(const std::string& strFile, const std::string& strMacro);

		void				AddConstBuffer(ConstantBuffer* pConstBuffer);
		uint32_t			GetConstBufferCount();
		ConstantBuffer*		GetConstBufferByIndex(uint32_t nIndex);

		void				AddStorgeBuffer(Uniform* pUniform);
		uint32_t			GetStorgeBufferCount();
		Uniform*			GetStorgeBufferByIndex(uint32_t nIndex);

		virtual void		RT_StreamComplete() = 0;

	private:
		std::string			m_strFile;
		std::string			m_strMacro;

		typedef std::vector< RefPtr<ConstantBuffer> > VEC_CONSTBUFFER;
		VEC_CONSTBUFFER		m_vecConstBuffer;

		typedef std::vector< RefPtr<Uniform> > VEC_UNIFORM;
		VEC_UNIFORM			m_vecStorgeBuffer;
	};

	RefPtr<ComputeShader> CreateComputeShader(const std::string& strFile, const std::string& strMacro);
}

