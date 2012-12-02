#ifndef  _DXRENDTEXTURE__H__
#define  _DXRENDTEXTURE__H__



namespace ma
{
	class DXRENDER_API DxRendTexture : public IRendTexture
	{
	public:
		DxRendTexture();

		~DxRendTexture();

		bool Load(const char* resPath);

		IDirect3DTexture9* GetD3DTexture() {return m_pD3DTex;}
		
	private:
		IDirect3DTexture9* m_pD3DTex;
	};

}



#endif