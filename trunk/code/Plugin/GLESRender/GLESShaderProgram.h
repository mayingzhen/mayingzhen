#ifndef GLESEFFECT_H_
#define GLESEFFECT_H_



namespace ma
{

	class GLESShaderProgram : public ShaderProgram
	{
	public:

		GLESShaderProgram(Technique* pTech,const char* pVSFile,const char* pPSFile,const char* pszDefine);

		~GLESShaderProgram();

		virtual void CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize);

		virtual void ParseUniform();

		void SetValue(Uniform* uniform, float value);

		void SetValue(Uniform* uniform, const float* values, UINT count = 1);

		void SetValue(Uniform* uniform, int value);

		void SetValue(Uniform* uniform, const int* values, UINT count = 1);

		void SetValue(Uniform* uniform, const Matrix4& value);

		void SetValue(Uniform* uniform, const Matrix4* values, UINT count = 1);

		void SetValue(Uniform* uniform, const Vector2& value);

		void SetValue(Uniform* uniform, const Vector2* values, UINT count = 1);

		void SetValue(Uniform* uniform, const Vector3& value);

		void SetValue(Uniform* uniform, const Vector3* values, UINT count = 1);

		void SetValue(Uniform* uniform, const Vector4& value);

		void SetValue(Uniform* uniform, const Vector4* values, UINT count = 1);

		void SetValue(Uniform* uniform, const ColourValue& value);

		void SetValue(Uniform* uniform, const SamplerState* sampler);

		void SetValue(Uniform* uniform, const SamplerState** values, UINT count);

		void SetValue(Uniform* uniform, const Texture* sampler);

		void SetValue(Uniform* uniform, const Texture** values, UINT count);

		void Bind();

		VertexAttribute getVertexAttribute(const char* name) const;

	private:
		void BindSampler(const Texture* pTexture,FilterOptions eFilter,Wrap eWrap);

	private:

		GLuint m_program;

		std::map<std::string, VertexAttribute> m_vertexAttributes;
	};


}

#endif
