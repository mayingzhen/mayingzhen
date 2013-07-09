#ifndef  _RenderTarget__H__
#define  _RenderTarget__H__


namespace ma
{

	class ENGINE_API RenderTarget 
	{
	public:
		
		virtual Texture* GetTexture() = 0;
		
	};

	class ENGINE_API DepthStencilTarget
	{
	public:
		virtual Texture* GetTexture() = 0;
	};


}

#endif