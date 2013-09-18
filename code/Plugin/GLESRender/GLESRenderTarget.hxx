#include "GLESRenderTarget.h"
#include "GLESTexture.h"

namespace ma
{

	GLESRenderTarget::GLESRenderTarget(int nWidth,int nHeight,FORMAT format)
	{
		m_handle = 0;
		m_pTexture = new GLESTexture(nWidth,nHeight,format);
	}


	GLESRenderTarget::~GLESRenderTarget()
	{

	}

	void GLESRenderTarget::Create()
	{
		m_pTexture->CreateRT();		
	}

	Texture* GLESRenderTarget::GetTexture()
	{	
		return m_pTexture;
	}

}


