#pragma once

#include "ShaderProgram.h"

namespace ma
{
	class ShaderManager 
	{
	public:
		ShaderManager();

		~ShaderManager();

		RefPtr<ShaderProgram> CreateShader(const ShaderCreateInfo& createInfo);

		void Clear() {m_resMap.clear();}

	private:
        
		typedef std::map<ShaderCreateInfo, RefPtr<ShaderProgram> > ResourceMap;

		ResourceMap			m_resMap;
	};

	extern ShaderManager* g_pShaderManager;
}

