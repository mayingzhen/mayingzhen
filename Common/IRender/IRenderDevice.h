#ifndef  _IRenderDevice__H__
#define  _IRenderDevice__H__

namespace ma
{
	class IRendMesh;
	class IRendTexture;
	class Texture;

	class COMMON_API IRenderDevice
	{
	public:
		virtual void BeginRender() = 0;

		virtual void EndRender() = 0;
	
		virtual IRendMesh* CreateRendMesh(const char* pResPath) = 0;

		//virtual IRendTexture* CreateRendTexture(const char* pResPath) = 0;

		virtual Texture* Load2DTexture(const char* pRespath) = 0;
	};

	COMMON_API void SetRenderDevice(IRenderDevice* pRenderDevice);

	COMMON_API IRenderDevice* GetRenderDevice();
}


#endif