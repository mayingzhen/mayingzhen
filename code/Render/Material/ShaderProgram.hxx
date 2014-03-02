#include "ShaderProgram.h"


namespace ma
{


	static void replaceDefines(const char* defines, std::string& out)
	{
		
		std::vector<std::string> arrTok;
		Tokenize(defines,arrTok,";",true);

		for (UINT i = 0; i < arrTok.size(); ++i)
		{
			out += "#define " + arrTok[i] + "\n";
		}

#if PLATFORM_ANDROID == 1 || PLAFTORM_IOS == 1
		out.insert(0, "#define OPENGL_ES\n");
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
				DataStream* pDataStream = FileSystem::readAll(directoryPath.c_str());
				const char* includedSource = (const char*)pDataStream->GetData();
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

	static std::string prePareShaderSource(const char* shPath,const char* defines)
	{
		// Read source from file.
		//char* shSource = FileSystem::readAll(shPath);
		DataStream* pDataStream = FileSystem::readAll(shPath);
		if (pDataStream == NULL)
			return "";

		const char* shSource = (const char*)pDataStream->GetData();
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


	
	ShaderProgram::ShaderProgram(Technique* pTech,const char* pVSFile,const char* pPSFile,const char* defines) 
	{
		m_pTech = pTech;
		//m_ShaderName = shName ? shName : "";
		m_strVSFile = pVSFile ? pVSFile : "";
		m_strPSFile = pPSFile ? pPSFile : "";
		m_shaderDefine = defines ? defines: "";
	}

	ShaderProgram::~ShaderProgram()
	{
	}

	void ShaderProgram::Create()
	{
		//std::string sTechName = m_pTech->GetTechName(); 
		//m_name = sTechName + "_" + m_strVSFile + "_" + m_shaderDefine;

		std::string strPath = GetRenderDevice()->GetShaderPath();
		//strPath += m_ShaderName;

		std::string strPathVS = strPath + m_strVSFile + ".vert";
		std::string strPathFS = strPath + m_strPSFile + ".frag";

		CreateFromFile(strPathVS.c_str(),strPathFS.c_str(),m_shaderDefine.c_str());
	}

	void ShaderProgram::CreateFromFile(const char* vshPath, const char* fshPath, const char* defines)
	{
		ASSERT(vshPath);
		ASSERT(fshPath);

		std::string strVshSource = prePareShaderSource(vshPath,defines);
		std::string strFshSource = prePareShaderSource(fshPath,defines);

		CreateFromSource(strVshSource.c_str(), strVshSource.length(), 
			strFshSource.c_str(), strFshSource.length());

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

}
