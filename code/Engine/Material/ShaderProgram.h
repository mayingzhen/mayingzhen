#ifndef __ShaderProgram_H_
#define __ShaderProgram_H_

#include "MethodBinding.h"

namespace ma
{

	class Uniform;

	class ShaderProgram : public Resource
	{
		
	public:
		ShaderProgram();

		virtual ~ShaderProgram();
	
		DECL_OBJECT(ShaderProgram)

		void				CreateFromFile(const char* vshPath, const char* fshPath, const char* defines, VertexDeclaration* pVertexDecl);

		void				Reload();	

		const char*			GetVSFile() const;
		void				SetVSFile(const char* pszVSFile);

		const char*			GetPSFile() const;
		void				SetPSFile(const char* pszPSFile);

		const char*			GetShaderMacro() const;
		void				SetShaderMacro(const char* pszMacro);
        
        VertexDeclaration*  GetVertexDeclaration() {return m_pVertexDecl.get();}

		void				AddConstBuffer(ShaderType eType, ConstantBuffer* pConstBuffer);
		UINT				GetConstBufferCount(ShaderType eType);
		ConstantBuffer*		GetConstBufferByIndex(ShaderType eType,UINT nIndex);

		void				AddSampler(Uniform* pUniform);
		UINT				GetSamplerCount();
		Uniform*			GetSamplerByIndex(UINT nIndex);

	protected:
		virtual void		RT_StreamComplete() = 0;

	private:
		typedef std::vector< RefPtr<ConstantBuffer> > VEC_CONSTBUFFER;
		VEC_CONSTBUFFER		m_vecConstBuffer[ShaderType_Number];

		std::vector< RefPtr<Uniform> > m_vecPSSamplers;

		std::string			m_strVSFile;
		std::string			m_strPSFile;
		std::string			m_shaderMacro;
        
        RefPtr<VertexDeclaration> m_pVertexDecl;

		friend class		RenderThread;
	};

	RefPtr<ShaderProgram> CreateShaderProgram(const char* pszVSFile,const char* pszPSFile,const char* pszMarco,VertexDeclaration* pVertexDecl);
}

#endif
