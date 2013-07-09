#include "GLESRenderTarget.h"
#include "GLESTexture.h"

namespace ma
{

	GLESRenderTarget::GLESRenderTarget()
	{
		m_handle = 0;
		m_pTexture = NULL;
	}


	GLESRenderTarget::~GLESRenderTarget()
	{

	}

	bool GLESRenderTarget::Create(int nWidth,int nHeight,FORMAT format)
	{
		m_pTexture = new GLESTexture();
		m_pTexture->CreateRT(nWidth,nHeight,format);
		
		return true;
	}

	Texture* GLESRenderTarget::GetTexture()
	{	
		return m_pTexture;
	}

}


