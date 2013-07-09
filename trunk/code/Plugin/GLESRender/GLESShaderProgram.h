#ifndef GLESEFFECT_H_
#define GLESEFFECT_H_



namespace ma
{

	class GLESShaderProgram : public ShaderProgram
	{
	public:

		GLESShaderProgram();

		~GLESShaderProgram();

		virtual void CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize);

		virtual void ParseUniform();

		void SetValue(Uniform* uniform, float value);

		void SetValue(Uniform* uniform, const float* values, unsigned int count = 1);

		void SetValue(Uniform* uniform, int value);

		void SetValue(Uniform* uniform, const int* values, unsigned int count = 1);

		void SetValue(Uniform* uniform, const Matrix4x4& value);

		void SetValue(Uniform* uniform, const Matrix4x4* values, unsigned int count = 1);

		void SetValue(Uniform* uniform, const Vector2& value);

		void SetValue(Uniform* uniform, const Vector2* values, unsigned int count = 1);

		void SetValue(Uniform* uniform, const Vector3& value);

		void SetValue(Uniform* uniform, const Vector3* values, unsigned int count = 1);

		void SetValue(Uniform* uniform, const Vector4& value);

		void SetValue(Uniform* uniform, const Vector4* values, unsigned int count = 1);

		void SetValue(Uniform* uniform, const Sampler* sampler);

		void SetValue(Uniform* uniform, const Sampler** values, unsigned int count);

		void Bind();

		VertexAttribute getVertexAttribute(const char* name) const;

	private:
		void BindSampler(const Sampler* pSampler);

	private:

		GLuint m_program;

		std::map<std::string, VertexAttribute> m_vertexAttributes;
	};


}

#endif
