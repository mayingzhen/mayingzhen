#ifndef __ShaderProgram_H_
#define __ShaderProgram_H_


namespace ma
{

	struct Uniform;
	class Technique;

	class RENDER_API ShaderProgram : public Resource
	{
		DECL_OBJECT(ShaderProgram)

	public:
		ShaderProgram(const char* pszPath = NULL);

		virtual ~ShaderProgram();

		virtual void		CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize) = 0;

		virtual void		ParseUniform() = 0;

		virtual	void		Bind() = 0;

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
		VEC_UNIFORM			m_uniforms;

		std::string			m_strVSFile;
		std::string			m_strPSFile;
		std::string			m_shaderDefine;

		friend class		RenderThread;
	};


	struct Uniform : public Referenced
	{
		std::string		m_name;
		int				m_location;
		int				m_type;
		bool			m_vshShder;
		UINT			m_index;
		UINT			m_nCount; 
		ShaderProgram*	m_pShader;

		Uniform()
		{
			m_location = -1;
			m_index = 0;
			m_type = 0;
			m_vshShder = false;
			m_index = 0;
			m_nCount = 0;
			m_pShader = NULL;
		}
	};

}

#endif
