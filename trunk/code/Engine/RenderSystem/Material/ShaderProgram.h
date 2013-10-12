#ifndef __ShaderProgram_H_
#define __ShaderProgram_H_


namespace ma
{

	struct Uniform;


	class ENGINE_API ShaderProgram /*: public RefCountObject*/
	{
	public:

		ShaderProgram(const char* shName,const char* defines = NULL);

		virtual ~ShaderProgram();

		void				Create();

		//void				CreateFromShaderName(const char* shName,const char* defines = NULL);

		void				CreateFromFile(const char* vshPath, const char* fshPath, const char* defines = NULL);

		virtual void		CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize) = 0;

		const char*			GetId() const {return m_id.c_str();}


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

		//virtual void		UnBind() = 0;

		//static ShaderProgram* GetCurrentEffect();

	protected:
		std::string	m_name;
		std::string m_id;
		std::map<std::string, Uniform*> m_uniforms;
		//static Uniform _emptyUniform;

		std::string m_ShaderName;
		std::string m_shaderDefine;
	};

	/**
	 * Represents a uniform variable within an effect.
	 */
	struct ENGINE_API Uniform
	{
		friend class ShaderProgram;

	public:

		/**
		 * Constructor.
		 */
		Uniform();

		/**
		 * Copy constructor.
		 */
		Uniform(const Uniform& copy);

		/**
		 * Destructor.
		 */
		~Uniform();

		/**
		 * Hidden copy assignment operator.
		 */
		Uniform& operator=(const Uniform&);

		/**
		 * Returns the name of this uniform.
		 * 
		 * @return The name of the uniform.
		 */
		const char* getName() const;

		/**
		 * Returns the OpenGL uniform type.
		 * 
		 * @return The OpenGL uniform type.
		 */
		int getType() const;

		/**
		 * Returns the effect for this uniform.
		 *
		 * @return The uniform's effect.
		 */
		ShaderProgram* GetEffect() const;

		void	SetEffect( ShaderProgram* pEffect) {m_effect = pEffect;}

	//private:



		std::string		m_name;
		int				m_location;
		int				m_type;
		bool			m_vshShder;
		UINT			m_index;
		UINT			m_nCount; 
		ShaderProgram*	m_effect;
	};

}

#endif
