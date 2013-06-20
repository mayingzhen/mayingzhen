#include "GLESRenderTarget.h"
#include "GLESTexture.h"

namespace ma
{

	GLESRenderTarget::GLESRenderTarget()
	{
		m_handle = 0;
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

}


