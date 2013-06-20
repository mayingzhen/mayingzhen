#ifndef  _RenderTarget__H__
#define  _RenderTarget__H__


namespace ma
{

	class ENGINE_API RenderTarget : public RefCountObject 
	{
	public:
		RenderTarget()
		{
			m_nWidth = 0;
			m_nHeight = 0;
			m_eFormat = FMT_A8R8G8B8;
			m_pTexture = NULL;
		}

		virtual bool Create(int nWidth,int nHeight,FORMAT format = FMT_A8R8G8B8) = 0;

		Texture* GetTexture() {return m_pTexture;}

	protected:
		int m_nWidth;
		int m_nHeight;
		FORMAT m_eFormat;
		Texture* m_pTexture;
		
	};

	DeclareSmartPtr(RenderTarget);

}

#endif