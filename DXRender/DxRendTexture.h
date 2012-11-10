#ifndef  _DXRENDTEXTURE__H__
#define  _DXRENDTEXTURE__H__



namespace ma
{
	class DXRENDER_API DxRendTexture : public IRendTexture
	{
	public:
		bool Load(const char* resPath);
		
	private:
		IDirect3DTexture9* m_pD3DTex;
	};

}



#endif