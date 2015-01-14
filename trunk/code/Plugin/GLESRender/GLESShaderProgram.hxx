#include "GLESShaderProgram.h"



namespace ma
{
	GLESShaderProgram::GLESShaderProgram()
		:ShaderProgram(NULL)
	{
		m_program = 0;
	}

	GLESShaderProgram::~GLESShaderProgram()
	{
		Destory();
	}

	void GLESShaderProgram::Destory()
	{
		GL_ASSERT( glDeleteProgram(m_program) );
		m_program = 0;
		m_vertexAttributes.clear();
	}

	void GLESShaderProgram::CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize)
	{
		Destory();

		ASSERT(vshSource);
		ASSERT(fshSource);

		//const UINT SHADER_SOURCE_LENGTH = 3;
		const GLchar* shaderSource[1];
		char* infoLog = NULL;
		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint program;
		GLint length;
		GLint success;

		shaderSource[0] = vshSource;
		GL_ASSERT( vertexShader = glCreateShader(GL_VERTEX_SHADER) );
		GL_ASSERT( glShaderSource(vertexShader, 1, shaderSource, NULL) );
		GL_ASSERT( glCompileShader(vertexShader) );
		GL_ASSERT( glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success) );
		if (success != GL_TRUE)
		{
			GL_ASSERT( glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length) );
			if (length == 0)
			{
				length = 4096;
			}
			if (length > 0)
			{
				infoLog = new char[length];
				GL_ASSERT( glGetShaderInfoLog(vertexShader, length, NULL, infoLog) );
				infoLog[length-1] = '\0';
				ASSERT(false);
			}

			// Write out the expanded shader file.
			//if (vshPath)
			//	writeShaderToErrorFile(vshPath, shaderSource[2]);

			//LogError("Compile failed for vertex shader '%s' with error '%s'.", vshPath == NULL ? vshSource : vshPath, infoLog == NULL ? "" : infoLog);
			SAFE_DELETE_ARRAY(infoLog);

			// Clean up.
			GL_ASSERT( glDeleteShader(vertexShader) );

			return /*NULL*/;
		}
	
		shaderSource[0] = fshSource;
		GL_ASSERT( fragmentShader = glCreateShader(GL_FRAGMENT_SHADER) );
		GL_ASSERT( glShaderSource(fragmentShader, 1, shaderSource, NULL) );
		GL_ASSERT( glCompileShader(fragmentShader) );
		GL_ASSERT( glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success) );
		if (success != GL_TRUE)
		{
			GL_ASSERT( glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length) );
			if (length == 0)
			{
				length = 4096;
			}
			if (length > 0)
			{
				infoLog = new char[length];
				GL_ASSERT( glGetShaderInfoLog(fragmentShader, length, NULL, infoLog) );
				infoLog[length-1] = '\0';
				ASSERT(false);
			}
	        
			// Write out the expanded shader file.
			//if (fshPath)
			//	writeShaderToErrorFile(fshPath, shaderSource[2]);

			//LogError("Compile failed for fragment shader (%s): %s", fshPath == NULL ? fshSource : fshPath, infoLog == NULL ? "" : infoLog);
			SAFE_DELETE_ARRAY(infoLog);

			// Clean up.
			GL_ASSERT( glDeleteShader(vertexShader) );
			GL_ASSERT( glDeleteShader(fragmentShader) );

			return /*NULL*/;
		}

		// Link program.
		GL_ASSERT( program = glCreateProgram() );
		GL_ASSERT( glAttachShader(program, vertexShader) );
		GL_ASSERT( glAttachShader(program, fragmentShader) );
		GL_ASSERT( glLinkProgram(program) );
		GL_ASSERT( glGetProgramiv(program, GL_LINK_STATUS, &success) );

		// Delete shaders after linking.
		GL_ASSERT( glDeleteShader(vertexShader) );
		GL_ASSERT( glDeleteShader(fragmentShader) );

		// Check link status.
		if (success != GL_TRUE)
		{
			GL_ASSERT( glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length) );
			if (length == 0)
			{
				length = 4096;
			}
			if (length > 0)
			{
				infoLog = new char[length];
				GL_ASSERT( glGetProgramInfoLog(program, length, NULL, infoLog) );
				infoLog[length-1] = '\0';
			}
			LogError("Linking program failed (%s,%s): %s", GetVSFile(),GetPSFile(),infoLog);
			SAFE_DELETE_ARRAY(infoLog);

			// Clean up.
			GL_ASSERT( glDeleteProgram(program) );

			return /*NULL*/;
		}

		// Create and return the new GLESShaderProgram.
		//GLESShaderProgram* effect = new GLESShaderProgram();
		//effect->m_program = program;

		// Query and store vertex attribute meta-data from the program.
		// NOTE: Rather than using glBindAttribLocation to explicitly specify our own
		// preferred attribute locations, we're going to query the locations that were
		// automatically bound by the GPU. While it can sometimes be convenient to use
		// glBindAttribLocation, some vendors actually reserve certain attribute indices
		// and therefore using this function can create compatibility issues between
		// different hardware vendors.
// 		GL_ASSERT( glBindAttribLocation(program,DU_POSITION,VERTEX_ATTRIBUTE_POSITION_NAME) );
// 		GL_ASSERT( glBindAttribLocation(program,DU_TEXCOORD,VERTEX_ATTRIBUTE_TEXCOORD0_NAME) );
// 		GL_ASSERT( glBindAttribLocation(program,DU_BLENDINDICES,VERTEX_ATTRIBUTE_BLENDINDICES_NAME) );
// 		GL_ASSERT( glBindAttribLocation(program,DU_BLENDWEIGHT,VERTEX_ATTRIBUTE_BLENDWEIGHTS_NAME) );
// 		GL_ASSERT( glBindAttribLocation(program,DU_NORMAL,VERTEX_ATTRIBUTE_NORMAL_NAME) );
// 		GL_ASSERT( glBindAttribLocation(program,DU_TANGENT,VERTEX_ATTRIBUTE_TANGENT_NAME) );
		GLint activeAttributes;
		GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &activeAttributes) );
		if (activeAttributes > 0)
		{
			GL_ASSERT( glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length) );
			if (length > 0)
			{
				GLchar* attribName = new GLchar[length + 1];
				GLint attribSize;
				GLenum attribType;
				GLint attribLocation;
				for (int i = 0; i < activeAttributes; ++i)
				{
					// Query attribute info.
					GL_ASSERT( glGetActiveAttrib(program, i, length, NULL, &attribSize, &attribType, attribName) );
					attribName[length] = '\0';

					// Query the pre-assigned attribute location.
					GL_ASSERT( attribLocation = glGetAttribLocation(program, attribName) );

					// Assign the vertex attribute mapping for the effect.
					m_vertexAttributes[attribName] = attribLocation;
				}
				SAFE_DELETE_ARRAY(attribName);
			}
		}

		m_program = program;	

		return /*effect*/;
	}

	void GLESShaderProgram::ParseUniform()
	{
	
		GLint activeUniforms;
		GL_ASSERT( glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &activeUniforms) );
		if (activeUniforms <= 0)
			return;

		GLint length = 0;
		GL_ASSERT( glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length) );
		if (length <= 0)
			return;

		GLchar* uniformName = new GLchar[length + 1];
		GLint uniformSize;
		GLenum uniformType;
		GLint uniformLocation;
		UINT samplerIndex = 0;
		for (int i = 0; i < activeUniforms; ++i)
		{
			// Query uniform info.
			GL_ASSERT( glGetActiveUniform(m_program, i, length, NULL, &uniformSize, &uniformType, uniformName) );
			uniformName[length] = '\0';  // null terminate
			if (length > 3)
			{
				// If this is an array uniform, strip array indexers off it since GL does not
				// seem to be consistent across different drivers/implementations in how it returns
				// array uniforms. On some systems it will return "u_matrixArray", while on others
				// it will return "u_matrixArray[0]".
				char* c = strrchr(uniformName, '[');
				if (c)
				{
					*c = '\0';
				}
			}

			// Query the pre-assigned uniform location.
			GL_ASSERT( uniformLocation = glGetUniformLocation(m_program, uniformName) );

			RefPtr<Uniform> uniform = this->AddUniform(uniformName);
			uniform->m_location = uniformLocation;
			uniform->m_type = uniformType;
			if (uniformType == GL_SAMPLER_2D)
			{
				uniform->m_index = samplerIndex;
				samplerIndex += uniformSize;
			}
			else
			{
				uniform->m_index = 0;
			}
		}
		SAFE_DELETE_ARRAY(uniformName);
	
	}

	VertexAttribute GLESShaderProgram::getVertexAttribute(const char* name) const
	{
		std::map<std::string, VertexAttribute>::const_iterator itr = m_vertexAttributes.find(name);
		//ASSERT(itr != m_vertexAttributes.end());
		return (itr == m_vertexAttributes.end() ? -1 : itr->second);
	}

	void GLESShaderProgram::Bind()
	{
	   GL_ASSERT( glUseProgram(m_program) );
	}
}
