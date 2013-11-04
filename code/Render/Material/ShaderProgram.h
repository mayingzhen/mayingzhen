#ifndef __ShaderProgram_H_
#define __ShaderProgram_H_


namespace ma
{

	struct Uniform;
	class Technique;

	class RENDER_API ShaderProgram 
	{
	public:

		ShaderProgram(Technique* pTech,const char* shName,const char* defines = NULL);

		virtual ~ShaderProgram();

		void				Create();

		void				CreateFromFile(const char* vshPath, const char* fshPath, const char* defines = NULL);

		virtual void		CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize) = 0;

		virtual void		ParseUniform() = 0;

		Uniform*			GetUniform(const char* name) const;

		Uniform*			GetUniform(UINT index) const;

		UINT				GetUniformCount() const;

		virtual void		SetValue(Uniform* uniform, float value) = 0;

		virtual void		SetValue(Uniform* uniform, const float* values, UINT count = 1) = 0;

		virtual void		SetValue(Uniform* uniform, int value) = 0;

		virtual void		SetValue(Uniform* uniform, const int* values, UINT count = 1) = 0;

		virtual void		SetValue(Uniform* uniform, const Matrix4x4& value) = 0;

		virtual void		SetValue(Uniform* uniform, const Matrix4x4* values, UINT count = 1) = 0;

		virtual void		SetValue(Uniform* uniform, const Vector2& value) = 0;

		virtual void		SetValue(Uniform* uniform, const Vector2* values, UINT count = 1) = 0;

		virtual void		SetValue(Uniform* uniform, const Vector3& value) = 0;

		virtual void		SetValue(Uniform* uniform, const Vector3* values, UINT count = 1) = 0;

		virtual void		SetValue(Uniform* uniform, const Vector4& value) = 0;

		virtual void		SetValue(Uniform* uniform, const Vector4* values, UINT count = 1) = 0;

		virtual void		SetValue(Uniform* uniform, const SamplerState* sampler) = 0;

		virtual void		SetValue(Uniform* uniform, const SamplerState** values, UINT count) = 0;

		virtual void		SetValue(Uniform* uniform, const Texture* sampler) = 0;

		virtual void		SetValue(Uniform* uniform, const Texture** values, UINT count) = 0;

		virtual	void		Bind() = 0;


	protected:
		std::string							m_name;

		std::map<std::string, Uniform*>		m_uniforms;

		std::string							m_ShaderName;
		std::string							m_shaderDefine;

		Technique*							m_pTech;
	};


	struct RENDER_API Uniform
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
