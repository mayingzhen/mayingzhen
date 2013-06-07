#include "ShaderProgram.h"

#define OPENGL_ES_DEFINE  "#define OPENGL_ES\n"

namespace ma
{

	// Cache of unique effects.
	static std::map<std::string, ShaderProgram*> __effectCache;
	static ShaderProgram* __currentEffect = NULL;

	static void replaceDefines(const char* defines, std::string& out)
	{
		if (defines && strlen(defines) != 0)
		{
			out = defines;
			size_t pos;
			out.insert(0, "#define ");
			while ((pos = out.find(';')) != std::string::npos)
			{
				out.replace(pos, 1, "\n#define ");
			}
			out += "\n";
		}
#if PLATFORM_ANDROID == 1 || PLAFTORM_IOS == 1
		out.insert(0, "#define Open");
#endif
	}

	static void replaceIncludes(const char* filepath, const char* source, std::string& out)
	{
		// Replace the #include "xxxx.xxx" with the sourced file contents of "filepath/xxxx.xxx"
		std::string str = source;
		size_t lastPos = 0;
		size_t headPos = 0;
		size_t fileLen = str.length();
		size_t tailPos = fileLen;
		while (headPos < fileLen)
		{
			lastPos = headPos;
			if (headPos == 0)
			{
				// find the first "#include"
				headPos = str.find("#include");
			}
			else
			{
				// find the next "#include"
				headPos = str.find("#include", headPos + 1);
			}

			// If "#include" is found
			if (headPos != std::string::npos)
			{
				// append from our last position for the legth (head - last position) 
				out.append(str.substr(lastPos,  headPos - lastPos));

				// find the start quote "
				size_t startQuote = str.find("\"", headPos) + 1;
				if (startQuote == std::string::npos)
				{
					// We have started an "#include" but missing the leading quote "
					GP_ERROR("Compile failed for shader '%s' missing leading \".", filepath);
					return;
				}
				// find the end quote "
				size_t endQuote = str.find("\"", startQuote);
				if (endQuote == std::string::npos)
				{
					// We have a start quote but missing the trailing quote "
					GP_ERROR("Compile failed for shader '%s' missing trailing \".", filepath);
					return;
				}

				// jump the head position past the end quote
				headPos = endQuote + 1;

				// File path to include and 'stitch' in the value in the quotes to the file path and source it.
				std::string filepathStr = filepath;
				std::string directoryPath = filepathStr.substr(0, filepathStr.rfind('/') + 1);
				size_t len = endQuote - (startQuote);
				std::string includeStr = str.substr(startQuote, len);
				directoryPath.append(includeStr);
				const char* includedSource = FileSystem::readAll(directoryPath.c_str());
				if (includedSource == NULL)
				{
					GP_ERROR("Compile failed for shader '%s' invalid filepath.", filepathStr.c_str());
					return;
				}
				else
				{
					// Valid file so lets attempt to see if we need to append anything to it too (recurse...)
					replaceIncludes(directoryPath.c_str(), includedSource, out);
					SAFE_DELETE_ARRAY(includedSource);
				}
			}
			else
			{
				// Append the remaining
				out.append(str.c_str(), lastPos, tailPos);
			}
		}
	}

	static void writeShaderToErrorFile(const char* filePath, const char* source)
	{
		std::string path = filePath;
		path += ".err";
		std::auto_ptr<Stream> stream(FileSystem::open(path.c_str(), FileSystem::WRITE));
		if (stream.get() != NULL && stream->canWrite())
		{
			stream->write(source, 1, strlen(source));
		}
	}

	static std::string prePareShaderSource(const char* shPath,const char* defines)
	{
		// Read source from file.
		char* shSource = FileSystem::readAll(shPath);
		if (shSource == NULL)
		{
			GP_ERROR("Failed to read vertex shader from file '%s'.", vshPath);
			return NULL;
		}

		const UINT SHADER_SOURCE_LENGTH = 3;
		const char* shaderSource[SHADER_SOURCE_LENGTH];
		// Replace all comma separated definitions with #define prefix and \n suffix
		std::string definesStr = "";
		replaceDefines(defines, definesStr);

		shaderSource[0] = definesStr.c_str();
		shaderSource[1] = "\n";
		std::string shSourceStr = "";
		if (shPath)
		{
			// Replace the #include "xxxxx.xxx" with the sources that come from file paths
			replaceIncludes(shPath, shSource, shSourceStr);
			if (shSource && strlen(shSource) != 0)
				shSourceStr += "\n";

			//writeShaderToErrorFile(vshPath, vshSourceStr.c_str());   // Debugging
		}
		shaderSource[2] = shPath ? shSourceStr.c_str() :  shSource;

		return std::string(shaderSource[0]) + shaderSource[1] + shaderSource[2];
	}


	
	ShaderProgram::ShaderProgram() 
	{
	}

	ShaderProgram::~ShaderProgram()
	{
		// Remove this effect from the cache.
		__effectCache.erase(m_id);

		// Free uniforms.
		for (std::map<std::string, Uniform*>::iterator itr = m_uniforms.begin(); itr != m_uniforms.end(); ++itr)
		{
			SAFE_DELETE(itr->second);
		}

		if (__currentEffect == this)
		{
			__currentEffect = NULL;
		}
	}

	void ShaderProgram::CreateFromShaderName(const char* shName,const char* defines)
	{
		ASSERT(shName);
		if (shName == NULL)
			return;

		std::string strPath; 
//#if PLATFORM_WIN == 1
		strPath = "Shader/D3D9/";
//#else PLAFTORM_IOS == 1 || PLATFORM_ANDROID == 1
//		strPath = "Shader/GLES/";
//#endif
		strPath += shName;

		std::string strPathVS = strPath + ".vert";
		std::string strPathFS = strPath + ".frag";

		CreateFromFile(strPathVS.c_str(),strPathFS.c_str(),defines);
	}

	void ShaderProgram::CreateFromFile(const char* vshPath, const char* fshPath, const char* defines)
	{
		ASSERT(vshPath);
		ASSERT(fshPath);

		// Search the effect cache for an identical effect that is already loaded.
		std::string uniqueId = vshPath;
		uniqueId += ';';
		uniqueId += fshPath;
		uniqueId += ';';
		if (defines)
		{
			uniqueId += defines;
		}
		std::map<std::string, ShaderProgram*>::const_iterator itr = __effectCache.find(uniqueId);
		if (itr != __effectCache.end())
		{
			// Found an exiting effect with this id, so increase its ref count and return it.
			ASSERT(itr->second);
			//itr->second->addRef();
			//return itr->second;
		}

		std::string strVshSource = prePareShaderSource(vshPath,defines);
		std::string strFshSource = prePareShaderSource(fshPath,defines);
		
		CreateFromSource(strVshSource.c_str(), strVshSource.length(), 
			strFshSource.c_str(), strFshSource.length() );

		ParseUniform();
	}


	Uniform* ShaderProgram::GetUniform(const char* name) const
	{
		std::map<std::string, Uniform*>::const_iterator itr = m_uniforms.find(name);
		return (itr == m_uniforms.end() ? NULL : itr->second);
	}

	Uniform* ShaderProgram::GetUniform(UINT index) const
	{
		UINT i = 0;
		for (std::map<std::string, Uniform*>::const_iterator itr = m_uniforms.begin(); itr != m_uniforms.end(); ++itr, ++i)
		{
			if (i == index)
			{
				return itr->second;
			}
		}
		return NULL;
	}

	UINT ShaderProgram::GetUniformCount() const
	{
		return m_uniforms.size();
	}


// 	ShaderProgram* ShaderProgram::GetCurrentEffect()
// 	{
// 		return __currentEffect;
// 	}

	Uniform::Uniform() :
		_location(-1), /*_type(0),*/ _index(0)
	{
	}

	Uniform::~Uniform()
	{
		// hidden
	}

	const ShaderProgram* Uniform::GetEffect() const
	{
		return _effect;
	}

	const char* Uniform::getName() const
	{
		return _name.c_str();
	}

	int Uniform::getType() const
	{
		return _type;
	}

}
