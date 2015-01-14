#ifndef __ShaderProgram_H_
#define __ShaderProgram_H_

#include "MethodBinding.h"

namespace ma
{

	class Uniform;

	class RENDER_API ShaderProgram : public Resource
	{
		DECL_OBJECT(ShaderProgram)

	public:
		ShaderProgram(const char* pszPath = NULL);

		virtual ~ShaderProgram();

		virtual void		CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize) = 0;

		virtual void		ParseUniform() = 0;

		virtual	void		Bind() = 0;

		void				BindUniform();
		
		void				Reload();

		void				CreateFromFile(const char* vshPath, const char* fshPath, const char* defines = NULL);

		Uniform*			GetUniform(const char* name) const;

		Uniform*			GetUniform(UINT index) const;

		UINT				GetUniformCount() const;

	protected:
		virtual void		Serialize(Serializer& sl, const char* pszLable = "Resource");

		virtual bool		CreateFromMemeory();

		void				RT_StreamComplete();

		const char*			GetVSFile() const;
		
		const char*			GetPSFile() const;

		const char*			GetShaderDefine() const;

		Uniform*			AddUniform(const char* pName);

	private:
		typedef std::vector< RefPtr<Uniform> > VEC_UNIFORM;
		VEC_UNIFORM			m_arrUniform;

		std::string			m_strVSFile;
		std::string			m_strPSFile;
		std::string			m_shaderDefine;

		friend class		RenderThread;
	};

}

#endif
