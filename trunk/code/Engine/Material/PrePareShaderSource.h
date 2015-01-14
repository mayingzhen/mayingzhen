# ifndef _PrePareShaderSource_H_
# define _PrePareShaderSource_H_

namespace ma
{
	void ReplaceDefines(const char* defines, std::string& out)
	{

#if PLATFORM_ANDROID == 1 || PLAFTORM_IOS == 1
		out.insert(0, "#define OPENGL_ES\n");
#endif

		if (defines == NULL || strcmp(defines,"") == 0 )
			return;

		std::vector<std::string> arrTok = StringUtil::split(defines,";");

		for (UINT i = 0; i < arrTok.size(); ++i)
		{
			out += "#define " + arrTok[i] + "\n";
		}

		// global macro
		uint32 nNumMacros = GetRenderSystem()->GetNumMacros();
		for (uint32 i = 0;i< nNumMacros;++i)
		{
			const char* pszValue = NULL;
			const char* pszMacro = GetRenderSystem()->GetMacroByIndex(i, pszValue);
			out += string("#define ") + string(pszMacro) + " " + pszValue + "\n";
		}
	}

	void ReplaceIncludes(const char* filepath, const char* source, std::string& out)
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
					LogError("Compile failed for shader '%s' missing leading \".", filepath);
					return;
				}
				// find the end quote "
				size_t endQuote = str.find("\"", startQuote);
				if (endQuote == std::string::npos)
				{
					// We have a start quote but missing the trailing quote "
					LogError("Compile failed for shader '%s' missing trailing \".", filepath);
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
				MemoryStreamPtr pDataStream = GetArchiveMananger()->ReadAll( directoryPath.c_str() , 1 );	
				char* includedSource = (char*)pDataStream->GetPtr();
				if (includedSource == NULL)
				{
					LogError("Compile failed for shader '%s' invalid filepath.", filepathStr.c_str());
					return;
				}
				else
				{
					// Valid file so lets attempt to see if we need to append anything to it too (recurse...)
					ReplaceIncludes(directoryPath.c_str(), includedSource, out);
					//SAFE_DELETE_ARRAY(includedSource);
				}
			}
			else
			{
				// Append the remaining
				out.append(str.c_str(), lastPos, tailPos);
			}
		}
	}

	std::string PrePareShaderSource(const char* shPath,const char* defines)
	{
		// Read source from file.
		MemoryStreamPtr pDataStream = GetArchiveMananger()->ReadAll(shPath,1); 
		if (pDataStream == NULL)
			return "";

		char* shSource = (char*)pDataStream->GetPtr();
		if (shSource == NULL)
		{
			LogError("Failed to read vertex shader from file '%s'.", shPath);
			return NULL;
		}

		const UINT SHADER_SOURCE_LENGTH = 3;
		const char* shaderSource[SHADER_SOURCE_LENGTH];
		// Replace all comma separated definitions with #define prefix and \n suffix
		std::string definesStr = "";
		ReplaceDefines(defines, definesStr);

		shaderSource[0] = definesStr.c_str();
		shaderSource[1] = "\n";
		std::string shSourceStr = "";
		if (shPath)
		{
			// Replace the #include "xxxxx.xxx" with the sources that come from file paths
			ReplaceIncludes(shPath, shSource, shSourceStr);
			if (shSource && strlen(shSource) != 0)
				shSourceStr += "\n";

			//writeShaderToErrorFile(vshPath, vshSourceStr.c_str());   // Debugging
		}
		shaderSource[2] = shPath ? shSourceStr.c_str() :  shSource;

		return std::string(shaderSource[0]) + shaderSource[1] + shaderSource[2];
	}

}


#endif
