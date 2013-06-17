#ifndef  _RenderTarget__H__
#define  _RenderTarget__H__


namespace ma
{

	class ENGINE_API RenderTarget : public RefCountObject 
	{
	public:
		RenderTarget()
		{
			mWidth = 0;
			mHeight = 0;
			mFormat = FMT_A8R8G8B8;
			m_pTexture = NULL;
		}

		virtual bool Create(int nWidth,int nHeight,FORMAT format = FMT_A8R8G8B8) = 0;

		Texture* GetTexture() {return m_pTexture;}

	protected:
		int mWidth;
		int mHeight;
		FORMAT mFormat;
		Texture* m_pTexture;
		
	};

	DeclareSmartPtr(RenderTarget);

}

#endif