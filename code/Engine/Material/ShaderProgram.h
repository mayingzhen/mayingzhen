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

		virtual void		CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize) = 0;
		void				CreateFromFile(const char* vshPath, const char* fshPath, const char* defines, VertexDeclaration* pVertexDecl);

		void				Reload();	

		void				BindUniform();	
		Uniform*			GetUniform(const char* name) const;
		Uniform*			GetUniform(UINT index) const;
		UINT				GetUniformCount() const;

		const char*			GetVSFile() const;
		void				SetVSFile(const char* pszVSFile);

		const char*			GetPSFile() const;
		void				SetPSFile(const char* pszPSFile);

		const char*			GetShaderMacro() const;
		void				SetShaderMacro(const char* pszMacro);
        
        VertexDeclaration*  GetVertexDeclaration() {return m_pVertexDecl.get();}

		virtual void		CommitChanges() { ASSERT(false); }

	protected:

		virtual	void		RT_SetShader() = 0;

		void				RT_StreamComplete();

		Uniform*			AddUniform(const char* pName);

	private:
		typedef std::vector< RefPtr<Uniform> > VEC_UNIFORM;
		VEC_UNIFORM			m_arrUniform;

		std::string			m_strVSFile;
		std::string			m_strPSFile;
		std::string			m_shaderMacro;
        
        RefPtr<VertexDeclaration> m_pVertexDecl;

		friend class		RenderThread;
	};

	RefPtr<ShaderProgram> CreateShaderProgram(const char* pszVSFile,const char* pszPSFile,const char* pszMarco,VertexDeclaration* pVertexDecl);
}

#endif
