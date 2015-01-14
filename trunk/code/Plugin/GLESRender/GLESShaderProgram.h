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

		void Bind();

		VertexAttribute getVertexAttribute(const char* name) const;

		void Destory();

	private:

		GLuint m_program;

		std::map<std::string, VertexAttribute> m_vertexAttributes;
	};


}

#endif
