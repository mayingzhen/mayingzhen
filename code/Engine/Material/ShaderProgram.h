#ifndef __ShaderProgram_H_
#define __ShaderProgram_H_

#include "MethodBinding.h"

namespace ma
{

	class Uniform;

	class ShaderProgram : public Resource
	{
		DECL_OBJECT(ShaderProgram)

	public:
		ShaderProgram(const char* pszPath = NULL);

		virtual ~ShaderProgram();

		virtual void		CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize) = 0;

		virtual void		ParseUniform() = 0;

		void				BindUniform();
		
		void				Reload();

		void				CreateFromFile(const char* vshPath, const char* fshPath, const char* defines = NULL);

		Uniform*			GetUniform(const char* name) const;

		Uniform*			GetUniform(UINT index) const;

		UINT				GetUniformCount() const;

		const char*			GetVSFile() const;

		const char*			GetPSFile() const;

		const char*			GetShaderMacro() const;

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

		friend class		RenderThread;
	};

}

#endif
