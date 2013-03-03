#ifndef  _IRenderDevice__H__
#define  _IRenderDevice__H__

namespace ma
{
	class IRendMesh;
	class IRendTexture;
	class Texture;

	class ENGINE_API IRenderDevice
	{
	public:
		virtual void BeginRender() = 0;

		virtual void EndRender() = 0;
	
		virtual IRendMesh* CreateRendMesh() = 0;

		virtual IRendTexture* CreateRendTexture() = 0;
	};

	ENGINE_API void SetRenderDevice(IRenderDevice* pRenderDevice);

	ENGINE_API IRenderDevice* GetRenderDevice();
}


#endif