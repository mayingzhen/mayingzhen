#include "GLESShaderProgram.h"



namespace ma
{
	GLESShaderProgram::GLESShaderProgram() : m_program(0)
	{
	}

	GLESShaderProgram::~GLESShaderProgram()
	{
		GL_ASSERT( glDeleteProgram(m_program) );
		m_program = 0;
	}


	void GLESShaderProgram::CreateFromSource(const char* vshSource, UINT vshSize, const char* fshSource, UINT fshSize)
	{
		ASSERT(vshSource);
		ASSERT(fshSource);

		//const unsigned int SHADER_SOURCE_LENGTH = 3;
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

			//GP_ERROR("Compile failed for vertex shader '%s' with error '%s'.", vshPath == NULL ? vshSource : vshPath, infoLog == NULL ? "" : infoLog);
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

			//GP_ERROR("Compile failed for fragment shader (%s): %s", fshPath == NULL ? fshSource : fshPath, infoLog == NULL ? "" : infoLog);
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
			GP_ERROR("Linking program failed (%s,%s): %s", vshPath == NULL ? "NULL" : vshPath, fshPath == NULL ? "NULL" : fshPath, infoLog == NULL ? "" : infoLog);
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
		unsigned int samplerIndex = 0;
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

			Uniform* uniform = new Uniform();
			uniform->_effect = this;
			uniform->_name = uniformName;
			uniform->_location = uniformLocation;
			uniform->_type = uniformType;
			if (uniformType == GL_SAMPLER_2D)
			{
				uniform->_index = samplerIndex;
				samplerIndex += uniformSize;
			}
			else
			{
				uniform->_index = 0;
			}

			m_uniforms[uniformName] = uniform;
		}
		SAFE_DELETE_ARRAY(uniformName);
	
	}

	VertexAttribute GLESShaderProgram::getVertexAttribute(const char* name) const
	{
		std::map<std::string, VertexAttribute>::const_iterator itr = m_vertexAttributes.find(name);
		//ASSERT(itr != m_vertexAttributes.end());
		return (itr == m_vertexAttributes.end() ? -1 : itr->second);
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, float value)
	{
		ASSERT(uniform);
		GL_ASSERT( glUniform1f(uniform->_location, value) );
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, const float* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);
		GL_ASSERT( glUniform1fv(uniform->_location, count, values) );
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, int value)
	{
		ASSERT(uniform);
		GL_ASSERT( glUniform1i(uniform->_location, value) );
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, const int* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);
		GL_ASSERT( glUniform1iv(uniform->_location, count, values) );
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, const Matrix4x4& value)
	{
		ASSERT(uniform);
		GL_ASSERT( glUniformMatrix4fv(uniform->_location, 1, GL_FALSE, (GLfloat*)&value) );
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, const Matrix4x4* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);
		GL_ASSERT( glUniformMatrix4fv(uniform->_location, count, GL_FALSE, (GLfloat*)values) );
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, const Vector2& value)
	{
		ASSERT(uniform);
		GL_ASSERT( glUniform2f(uniform->_location, value.x, value.y) );
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, const Vector2* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);
		GL_ASSERT( glUniform2fv(uniform->_location, count, (GLfloat*)values) );
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, const Vector3& value)
	{
		ASSERT(uniform);
		GL_ASSERT( glUniform3f(uniform->_location, value.x, value.y, value.z) );
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, const Vector3* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);
		GL_ASSERT( glUniform3fv(uniform->_location, count, (GLfloat*)values) );
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, const Vector4& value)
	{
		ASSERT(uniform);
		GL_ASSERT( glUniform4f(uniform->_location, value.x, value.y, value.z, value.w) );
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, const Vector4* values, unsigned int count)
	{
		ASSERT(uniform);
		ASSERT(values);
		GL_ASSERT( glUniform4fv(uniform->_location, count, (GLfloat*)values) );
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, const Sampler* sampler)
	{
		ASSERT(uniform);
		ASSERT(uniform->_type == GL_SAMPLER_2D);
		ASSERT(sampler);

		GL_ASSERT( glActiveTexture(GL_TEXTURE0 + uniform->_index) );

		BindSampler(sampler->getTexture(),sampler->m_eFilter,sampler->m_eWrap);

		GL_ASSERT( glUniform1i(uniform->_location, uniform->_index) );
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, const Sampler** values, unsigned int count)
	{
	     ASSERT(uniform);
	     ASSERT(uniform->_type == GL_SAMPLER_2D);
	     ASSERT(values);
	 
	     // Set samplers as active and load texture unit array
	     GLint units[32];
	     for (unsigned int i = 0; i < count; ++i)
	     {
	         GL_ASSERT( glActiveTexture(GL_TEXTURE0 + uniform->_index + i) );
	 
	         // Bind the sampler - this binds the texture and applies sampler state
	         BindSampler(values[i]->getTexture(),values[i]->m_eFilter,values[i]->m_eWrap);
	 
	         units[i] = uniform->_index + i;
	     }
	 
	     // Pass texture unit array to GL
	     GL_ASSERT( glUniform1iv(uniform->_location, count, units) );
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, const Texture* sampler)
	{
		ASSERT(uniform);
		ASSERT(uniform->_type == GL_SAMPLER_2D);
		ASSERT(sampler);

		GL_ASSERT( glActiveTexture(GL_TEXTURE0 + uniform->_index) );

		BindSampler(sampler,Sampler::TFO_BILINEAR,Sampler::CLAMP);

		GL_ASSERT( glUniform1i(uniform->_location, uniform->_index) );
	}

	void GLESShaderProgram::SetValue(Uniform* uniform, const Texture** values, UINT count)
	{
		ASSERT(uniform);
		ASSERT(uniform->_type == GL_SAMPLER_2D);
		ASSERT(values);

		// Set samplers as active and load texture unit array
		GLint units[32];
		for (unsigned int i = 0; i < count; ++i)
		{
			GL_ASSERT( glActiveTexture(GL_TEXTURE0 + uniform->_index + i) );

			// Bind the sampler - this binds the texture and applies sampler state
			BindSampler(values[i],Sampler::TFO_BILINEAR,Sampler::CLAMP);

			units[i] = uniform->_index + i;
		}

		// Pass texture unit array to GL
		GL_ASSERT( glUniform1iv(uniform->_location, count, units) );
	}

	void GLESShaderProgram::Bind()
	{
	   GL_ASSERT( glUseProgram(m_program) );
	}

	void GLESShaderProgram::BindSampler(const Texture* pTex,Sampler::FilterOptions eFilter,Sampler::Wrap eWrap)
	{
		GLESTexture* pTexture = (GLESTexture*)pTex;
		ASSERT(pTexture);
		if (pTexture == NULL)
			return;
		
		GL_ASSERT( glBindTexture(GL_TEXTURE_2D, pTexture->GetTexture() ) );

		GLenum minFilter,magFilter;
		GLESMapping::GetGLESFilter(eFilter,minFilter,magFilter);
		GLenum wrapS = GLESMapping::GetGLESWrap(eWrap);
		GLenum wrapT = GLESMapping::GetGLESWrap(eWrap);

		GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter) );
		GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter) );	

		GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS) );

		GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT) );
	}


}
