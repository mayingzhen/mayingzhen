#include "GLESTechnique.h"

namespace ma
{
	GLESTechnique::GLESTechnique()
	{
		m_pEffect = NULL;
		m_pEffectSkin = NULL;
		m_pCurEffect = NULL;
	}

	void GLESTechnique::Load(const char* pFileName, const char* pTechBaseName)
	{
		ASSERT(pTechBaseName);
		if (pTechBaseName == NULL)
			return;

		m_strTechName = pTechBaseName;

		std::string strPath;
		if (pFileName == NULL)
		{
			strPath = std::string("../Data/Shader/GLES/") + pTechBaseName ;
		}
		else
		{
			strPath = pFileName;
		}
		
		std::string strPathVS = strPath + ".vert";
		std::string strPathFS = strPath + ".frag";
		m_pEffect = Effect::createFromFile(strPathVS.c_str(), strPathFS.c_str(), NULL);
		m_pEffectSkin = Effect::createFromFile(strPathVS.c_str(), strPathFS.c_str(), "SKINNING;SKINNING_JOINT_COUNT 55");


		//m_pEffect->ge
		//glBindAttribLocation(m_hProgram,SA_POSITION,"rm_Vertex");
		//glBindAttribLocation(m_hProgram,SA_TEXCOORD0,"rm_Texcoord");
	}

	void GLESTechnique::Begin(UINT uShaderFlg)
	{
		m_pCurEffect = m_pEffect;
		if (uShaderFlg & ShderFLag_SKIN)
		{
			m_pCurEffect = m_pEffectSkin;
		}

		m_pCurEffect->bind();
	}

	void GLESTechnique::CommitChanges()
	{

	}

	void GLESTechnique::End()
	{
	}
	
	void GLESTechnique::SetMatrixArray(const char* pParameter,Matrix4x4* ppMatrix, UINT Count) 
	{
		Uniform* pUniform = m_pCurEffect->getUniform(pParameter);
		ASSERT(pUniform);
		m_pCurEffect->setValue(pUniform,ppMatrix,Count);
	}

	void GLESTechnique::SetMatrix(const char* pParameter,Matrix4x4* pMatrix)
	{
		Uniform* pUniform = m_pCurEffect->getUniform(pParameter);
		ASSERT(pUniform);
		m_pCurEffect->setValue(pUniform,*pMatrix);
	}

	void GLESTechnique::SetVector(const char* pParameter, Vector4* pVector)
	{
		Uniform* pUniform = m_pCurEffect->getUniform(pParameter);
		ASSERT(pUniform);
		m_pCurEffect->setValue(pUniform,*pVector);
	}

	void GLESTechnique::SetTexture(const char* pParameter, Texture* pTexture)
	{
		Uniform* pUniform = m_pCurEffect->getUniform(pParameter);
		ASSERT(pUniform);
		m_pCurEffect->setValue(pUniform,pTexture);
	}

}